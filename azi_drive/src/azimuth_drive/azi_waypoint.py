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

def smallest_coterminal_angle(x):
    # Bounded between [-pi, pi]
    return (x + math.pi) % (2*math.pi) - math.pi

class trajectory_generator:
    def __init__(self, name):
        # Desired pose
        self.desired_position = self.current_position = np.zeros(3)
        self.desired_orientation = self.current_orientation = np.zeros(3)
        #self.desired_twist = self.current_twist = Twist()

        # Goal tolerances before seting succeded
        self.linear_tolerance = rospy.get_param('linear_tolerance', 0.5)
        self.angular_tolerance = rospy.get_param('angular_tolerance', np.pi / 10)
        self.orientation_radius = rospy.get_param('orientation_radius', 0.75)
        self.slow_down_radius = rospy.get_param('slow_down_radius', 5)

        # Speed parameters
        self.max_tracking_distance = rospy.get_param('max_tracking_distance', 1.0)
        self.min_tracking_distance = rospy.get_param('min_tracking_distance', .1)
        self.tracking_to_speed_conv = rospy.get_param('tracking_to_speed_conv', 1)
        self.tracking_slope = (self.max_tracking_distance - self.min_tracking_distance) / (self.slow_down_radius - self.orientation_radius)
        self.tracking_intercept = self.tracking_slope * self.orientation_radius + self.min_tracking_distance

        # Publishers
        self.traj_pub = rospy.Publisher('/trajectory', PoseTwistStamped, queue_size = 10)
        self.traj_debug_pub = rospy.Publisher('/trajectory_debug', PoseStamped, queue_size = 10)

        # Set desired twist to 0
        #self.desired_twist.linear.x = self.desired_twist.linear.y = self.desired_twist.linear.z = 0
        #self.desired_twist.angular.x = self.desired_twist.angular.y = self.desired_twist.angular.z = 0

        # Initilize a line
        self.line = line(np.zeros(3), np.ones(3))

        # Wait for current position and set as desired position
        rospy.loginfo('Waiting for /odom')
        self.odom_sub = rospy.Subscriber('/odom', Odometry, self.odom_cb, queue_size = 10)
        while not self.current_position.any():  # Will be 0 until an odom publishes (if its still 0 it will drift very very soon)
            pass

        # Initlize Trajectory generator with current position as goal
        #   Set the line to be along our current orientation
        self.desired_position = self.current_position
        self.desired_orientation = self.current_orientation
        #   Make a line along the orientation
        self.line = line(self.current_position, tools.normal_vector_from_rotvec(self.current_orientation) + self.current_position)
        self.redraw_line = False
        rospy.loginfo('Got current pose from /odom')

        # Kill handling
        self.killed = False
        self.kill_listener = KillListener(self.set_kill, self.clear_kill)
        self.kill_broadcaster = KillBroadcaster(id=name, description='Tank steer trajectory_generator shutdown')
        try:
            self.kill_broadcaster.clear()           # In case previously killed
        except rospy.service.ServiceException, e:
            rospy.logwarn(str(e))
        rospy.on_shutdown(self.on_shutdown)

        # RC handling

        # Start the main update loop
        rospy.Timer(rospy.Duration(0.1), self.update)

        # Implement the moveto action server
        self.moveto_as = actionlib.SimpleActionServer('moveto', MoveToAction, None, False)
        self.moveto_as.register_goal_callback(self.new_goal)
        self.moveto_as.register_preempt_callback(self.goal_preempt)
        self.moveto_as.start()

    def new_goal(self):
        goal = self.moveto_as.accept_new_goal()
        self.desired_position = tools.position_from_posetwist(goal.posetwist)
        self.desired_orientation = tools.orientation_from_posetwist(goal.posetwist)
        #self.linear_tolerance = goal.linear_tolerance
        #self.angular_tolerance = goal.angular_tolerance

        rospy.loginfo('Desired position:' + str(self.desired_position))
        rospy.loginfo('Current position:' + str(self.current_position))
        rospy.loginfo('Desired orientation:' + str(self.desired_orientation * 180 / np.pi))
        rospy.loginfo('Current orientation:' + str(self.current_orientation * 180 / np.pi))
        rospy.loginfo('linear_tolerance:' + str(self.linear_tolerance))
        rospy.loginfo('angular_tolerance:' + str(self.angular_tolerance))
        # This controller doesn't handle desired twist
        #self.desired_twist = goal.posetwist.twist
        if (xyz_array(goal.posetwist.twist.linear).any() or 
            xyz_array(goal.posetwist.twist.angular).any() ):
            rospy.logwarn('None zero are not handled by the tank steer trajectory generator. Setting twist to 0')
        
        if np.linalg.norm(self.current_position - self.desired_position) > self.orientation_radius:
            self.line = line(self.current_position, self.desired_position)
            self.redraw_line = True
        else:
            self.line = line(self.desired_position, tools.normal_vector_from_rotvec(self.desired_orientation) + self.desired_position)
            self.redraw_line = False

    def goal_preempt(self):
        self.desired_position = self.current_position
        self.desired_orientation = self.current_orientation
        self.moveto_as.set_preempted()
        rospy.loginfo('Goal preempted')
        

    def set_kill(self):
        self.killed = True
        rospy.logwarn('Tank steer trajectory_generator killed because: %s' % self.kill_listener.get_kills())

    def clear_kill(self):
        self.killed = False
        rospy.loginfo('Tank steer trajectory_generator unkilled')

    def on_shutdown(self):
        self.kill_broadcaster.send(True)

    # Update pose and twist
    def odom_cb(self, msg):
        self.current_position = tools.position_from_pose(msg.pose.pose)
        self.current_orientation = tools.orientation_from_pose(msg.pose.pose)
        # Get distance to the goal
        vector_to_goal = self.desired_position - self.current_position
        self.distance_to_goal = np.linalg.norm(vector_to_goal)
        self.angle_to_goal_orientation = map(smallest_coterminal_angle, self.desired_orientation - self.current_orientation)
        # overshoot is 1 if behind line drawn perpendicular to the goal line and through the desired position, -1 if on the other
        #       Side of said line
        #print 'Distance to goal: ', self.distance_to_goal
        #print 'line.hat: ', self.line.hat
        #print 'Vector to goal: ', vector_to_goal
        
        overshoot = np.dot(vector_to_goal / self.distance_to_goal, self.line.hat)

        self.overshoot = overshoot / abs(overshoot) 

        if math.isnan(self.overshoot):
            self.overshoot = 1

        #print 'Overshoot: ', self.overshoot

    # Get the speed setting of the trajectory
    #               Value                   :   Condition
    #               max                     :   d > slow_down
    #               min                     :   d < orientation
    #   tracking_slope * d + intercept      : orientation < d< slow_down
    def get_tracking_distance(self):
        if self.distance_to_goal < self.orientation_radius:
            return self.min_tracking_distance
        elif self.distance_to_goal > self.slow_down_radius:
            return self.max_tracking_distance
        else:
            return self.tracking_slope * self.distance_to_goal + self.tracking_intercept

    # Get a PoseTwistStamped that represents the desired point along the trajectory that we need
    def get_carrot(self):
        # Project current position onto trajectory line
        Bproj = self.line.proj_pt(self.current_position)
        parallel_distance = np.linalg.norm(self.desired_position - Bproj)
        #rospy.loginfo('Projection: ' + str(Bproj))

        # Default carrot to desired position
        tracking_step = self.get_tracking_distance()
        c_pos = Bproj + self.overshoot * self.line.hat * tracking_step

        if parallel_distance < self.orientation_radius:
            c_pos = self.desired_position

        header = Header(
                stamp = rospy.get_rostime(),
                frame_id = '/enu'
            )
        pose = Pose(
                    position = tools.vector3_from_xyz_array(c_pos),
                    orientation = tools.quaternion_from_rotvec([0, 0, self.line.angle]))

        self.traj_debug_pub.publish(PoseStamped(header=header, pose=pose))        

        # Fill up PoseTwistStamped
        carrot = PoseTwistStamped(
            header = header,
            posetwist = PoseTwist(
                pose = pose,
                twist = Twist(
                    linear = Vector3(tracking_step * self.tracking_to_speed_conv * self.overshoot, 0, 0),        # Wrench Generator handles the sine of the velocity
                    angular = Vector3())
                )
            )
        return carrot

    # Update loop
    def update(self, event):
        
        # Publish trajectory
        traj = self.get_carrot()
        #rospy.loginfo('Trajectory: ' + str(traj))
        if not self.killed:
            self.traj_pub.publish(traj)

        if self.redraw_line and self.distance_to_goal < self.orientation_radius:
            self.redraw_line = False
            rospy.loginfo('Redrawing trajectory line')
            self.line = line(self.desired_position, tools.normal_vector_from_rotvec(self.desired_orientation) + self.desired_position)

        #rospy.loginfo('Angle to goal: ' + str(self.angle_to_goal_orientation[2] * 180 / np.pi) + '\t\t\tDistance to goal: ' + str(self.distance_to_goal))

        # Check if goal is reached
        if self.moveto_as.is_active():
            if self.distance_to_goal < self.linear_tolerance:
                if abs(self.angle_to_goal_orientation[2]) < self.angular_tolerance:
                    rospy.loginfo('succeded')
                    self.moveto_as.set_succeeded(None)


if __name__ == '__main__':
    rospy.init_node('trajectory_generator')
    node = trajectory_generator(rospy.get_name())
    rospy.spin()