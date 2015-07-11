#! /usr/bin/env python

##          Trajectory generator
## This node plans a strait line path to the goal and publishes it on the trajectory topic

import rospy

import actionlib
from uf_common.msg import PoseTwistStamped, MoveToAction, PoseTwist
from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster
from nav_msgs.msg import Odometry
from std_msgs.msg import Header
from geometry_msgs.msg import Pose, Twist, PoseStamped
from geometry_msgs.msg import Quaternion, Vector3
from uf_common.orientation_helpers import xyz_array, xyzw_array, quat_to_rotvec
from uf_common.orientation_helpers import rotvec_to_quat
import numpy as np
import tools
from tools import line
import math
import threading
from point_shoot_pp import point_shoot_pp
from point_shoot_2_pp import point_shoot_2_pp
from a_star_rpp import a_star_rpp
from azi_drive.srv import *

class azi_waypoint:
    def __init__(self, name):
        # Implement the moveto action server
        self.moveto_as = actionlib.SimpleActionServer('moveto', MoveToAction, None, False)
        # Thread Locking
        self.as_lock = self.moveto_as.action_server.lock
        self.moveto_as.register_goal_callback(self.new_goal)
        self.moveto_as.register_preempt_callback(self.goal_preempt)

        # Trajectory generator (using stratagy design pattern where traj_gen is the stratagy)
        self.traj_gen = None

        # Desired pose
        self.desired_position = self.current_position = np.zeros(3)
        self.desired_orientation = self.current_orientation = np.zeros(3)
        #self.desired_twist = self.current_twist = Twist()

        # Goal tolerances before seting succeded
        self.linear_tolerance = rospy.get_param('linear_tolerance', 0.75)
        self.angular_tolerance = rospy.get_param('angular_tolerance', 20 * np.pi / 180)

        # Publishers
        self.traj_pub = rospy.Publisher('/trajectory', PoseTwistStamped, queue_size = 10)
        self.traj_debug_pub = rospy.Publisher('/trajectory_debug', PoseStamped, queue_size = 10)

        # Set desired twist to 0
        #self.desired_twist.linear.x = self.desired_twist.linear.y = self.desired_twist.linear.z = 0
        #self.desired_twist.angular.x = self.desired_twist.angular.y = self.desired_twist.angular.z = 0

        # Wait for current position and set as desired position
        rospy.loginfo('Waiting for /odom')
        self.odom_sub = rospy.Subscriber('/odom', Odometry, self.odom_cb, queue_size = 10)
        while not self.current_position.any(): pass # Will be 0 until an odom publishes (if its still 0 it will drift very very soon)
        self.desired_position = self.current_position
        self.desired_orientation = self.current_orientation
        rospy.loginfo('Got current pose from /odom')

        # Kill handling
        self.killed = False
        self.kill_listener = KillListener(self.set_kill, self.clear_kill)
        self.kill_broadcaster = KillBroadcaster(id=name, description='Azi waypoint shutdown')
        try:
            self.kill_broadcaster.clear()           # In case previously killed
        except rospy.service.ServiceException, e:
            rospy.logwarn(str(e))
        rospy.on_shutdown(self.on_shutdown)

        # Trajectory mode dictionary
        self.modes ={
            trajectory_modeRequest.POINT_SHOOT_PP:         point_shoot_pp,
            trajectory_modeRequest.POINT_SHOOT_2_PP:       point_shoot_2_pp,
            trajectory_modeRequest.A_STAR_RPP:             a_star_rpp
        }

        # Initilize Trajectory generator with current position as goal
        self.mode = trajectory_modeRequest.POINT_SHOOT_2_PP
        self.traj_gen = point_shoot_2_pp()
        self.traj_gen.start(self.get_current_posetwist())

        # Initilize the switch modes service
        mode_srv = rospy.Service('azi_waypoint_mode', trajectory_mode, self.switch_mode)

        # Start the main update loop
        rospy.Timer(rospy.Duration(0.1), self.update)

        self.moveto_as.start()

    def switch_mode(self, mode):
        if self.mode is not mode.mode:
            if mode.mode not in self.modes.keys():
                rospy.logerr('Attempt to put Azi waypoint into an undefined mode: ' + str(mode.mode))
                return False
            # Stop the current mode
            self.traj_gen.stop()

            # Start new mode
            self.mode = mode.mode
            self.traj_gen = self.modes[self.mode]()
            self.traj_gen.start(self.get_current_posetwist())

            rospy.loginfo('Azi waypoint mode switched to ' + str(self.mode) + ' mode')

            return True

        else:
            rospy.loginfo('Azi waypoint already in requested mode')
            return True

    def new_goal(self):
        # Lock on odom cb
        #print 'New goal: wait for lock'
        self.as_lock.acquire()
        #print 'New goal lock'

        goal = self.moveto_as.accept_new_goal()
        pt = goal.posetwist
        self.desired_position = tools.position_from_posetwist(pt)
        self.desired_orientation = tools.orientation_from_posetwist(pt)

        #self.linear_tolerance = goal.linear_tolerance
        #self.angular_tolerance = goal.angular_tolerance

        rospy.logdebug('Desired position:' + str(self.desired_position))
        rospy.logdebug('Current position:' + str(self.current_position))
        rospy.logdebug('Desired orientation:' + str(self.desired_orientation * 180 / np.pi))
        rospy.logdebug('Current orientation:' + str(self.current_orientation * 180 / np.pi))
        rospy.logdebug('linear_tolerance:' + str(self.linear_tolerance))
        rospy.logdebug('angular_tolerance:' + str(self.angular_tolerance))
        
        # Pass the new goal to the trajectory generator
        if self.traj_gen is not None:
            self.traj_gen.new_goal(pt)

        # Release lock
        self.as_lock.release()
        #print 'new_goal released'

    def goal_preempt(self):
        self.as_lock.acquire()
        #print 'goal_preempt lock'
        rospy.loginfo('Goal preempted')

        # Set current position to desired position
        self.desired_position = self.current_position
        self.desired_orientation = self.current_orientation

        # Tell trajectory generator to preempt
        if self.traj_gen is not None:
            self.traj_gen.preempt_goal()

        # Set action as preempted
        self.moveto_as.set_preempted()
        
        # Release lock
        self.as_lock.release()
        #print 'preempt released'

    # Update pose and twist
    def odom_cb(self, msg):
        # Lock on odom cb
        self.as_lock.acquire()
        #print 'odom lock'

        self.current_position = tools.position_from_pose(msg.pose.pose)
        # Zero the Z
        self.current_position[2] = 0
        self.current_orientation = tools.orientation_from_pose(msg.pose.pose)

        # Give trajectory generator feedback
        if self.traj_gen is not None:
            self.traj_gen.feedback(self.get_current_posetwist())

        # Get distance to the goal
        vector_to_goal = self.desired_position - self.current_position
        self.distance_to_goal = np.linalg.norm(vector_to_goal)
        self.angle_to_goal_orientation = map(tools.smallest_coterminal_angle, self.desired_orientation - self.current_orientation)

        # Release lock
        self.as_lock.release()
        #print 'odom released'


    # Update loop
    def update(self, event):
        # Lock on odom cb
        self.as_lock.acquire()
        #print 'update lock'

        # Make a header
        header = Header(
                frame_id = '/enu',
                stamp = rospy.get_rostime())

        # Get a posetwist
        posetwist = PoseTwist()
        if self.traj_gen is not None:
            posetwist = self.traj_gen.update()

        # Publish trajectory
        traj = PoseTwistStamped(
            header = header,
            posetwist = posetwist
            )

        # Publish desired pose for RVIZ
        self.traj_debug_pub.publish(PoseStamped(header=header, pose=traj.posetwist.pose))

        # Killed logic
        if not self.killed:
            self.traj_pub.publish(traj)

        rospy.logdebug('Angle to goal: ' + str(self.angle_to_goal_orientation[2] * 180 / np.pi) + '\t\t\tDistance to goal: ' + str(self.distance_to_goal))

        # Check if goal is reached
        if self.moveto_as.is_active():
            if self.distance_to_goal < self.linear_tolerance:
                if abs(self.angle_to_goal_orientation[2]) < self.angular_tolerance:
                    rospy.loginfo('Reached goal')
                    self.moveto_as.set_succeeded(None)

        # Release lock
        self.as_lock.release()
        #print 'update released'


    def get_current_posetwist(self):
        return PoseTwist(
                pose = Pose(
                    position = tools.vector3_from_xyz_array(self.current_position),
                    orientation = tools.quaternion_from_rotvec(self.current_orientation)),

                twist = Twist(
                    linear = Vector3(),
                    angular = Vector3())
                )

    # Kill handling and shutdown
    def set_kill(self):
        self.killed = True
        rospy.logwarn('Azi waypoint killed because: %s' % self.kill_listener.get_kills())

    def clear_kill(self):
        self.killed = False
        rospy.loginfo('Azi waypoint unkilled')

    def on_shutdown(self):
        self.kill_broadcaster.send(True)

if __name__ == '__main__':
    rospy.init_node('azi_waypoint')#, log_level=rospy.DEBUG)
    node = azi_waypoint(rospy.get_name())
    rospy.spin()
