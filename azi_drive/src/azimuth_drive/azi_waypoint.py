#! /usr/bin/env python

import roslib
import rospy
import os

# Brings in the SimpleActionaction
import actionlib
from uf_common.msg import PoseTwistStamped, MoveToAction
from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster
from std_msgs.msg import Bool

'''

TODO: Right now values are oonly coming in from the waypoint node
      Still need to send those values to azi_drive

'''

class send_action:

    def __init__(self, name):
        self.action = actionlib.SimpleActionServer('moveto', MoveToAction, self.new_goal, False)
        self.trajectory_pub = rospy.Publisher('/trajectory', PoseTwistStamped, queue_size=10)
        self.waypoint_progress = rospy.Subscriber('/waypoint_progress', Bool, self.callback)
        self.kill_listener = KillListener(self.set_kill, self.clear_kill)

        self.action.start()
        self.to_pose = PoseTwistStamped()
        self.temp_pose = PoseTwistStamped()
        self.killed = False
        self.waypoint = False

    def callback(self, msg):
        self.waypoint = msg.data


    def set_kill(self):

        self.to_pose = PoseTwistStamped()

        # TODO --> Verify that all values are at 0

        #members = [attr for attr in dir(self.to_pose.posetwist.pose) if attr.startswith("position") or attr.startswith("orientation")]
        #getattr(self.to_pose.posetwist.pose.position, "x") == 0
        rospy.logwarn('Azi_Drive waypoint kill flag off -- All trajectories at 0: %s' % self.kill_listener.get_kills())
        self.killed = True
        
    def clear_kill(self):
        self.killed = False
        rospy.logwarn('Azi_Drive waypoint kill flag off -- Waypoints enabled: %s' % self.kill_listener.get_kills())

    def new_goal(self, goal):
        self.waypoint = False
        self.temp_pose = PoseTwistStamped()
        self.temp_pose.posetwist = goal.posetwist
        self.temp_pose.header = goal.header

        while self.waypoint != True:
            None

        self.action.set_succeeded()
        self.waypoint = True

        

    def over_and_over(self):
        r = rospy.Rate(1)
        if self.killed == True:
            rospy.logwarn('Azi_Drive waypoint kill flag on  -- Waypoints disabled: %s' % self.kill_listener.get_kills())
        if self.killed == False:
            self.to_pose = self.temp_pose
            self.trajectory_pub.publish(self.to_pose)
            print self.to_pose.posetwist.pose
            self.to_pose = PoseTwistStamped()
        r.sleep()

if __name__ == '__main__':
        rospy.init_node('azi_waypoint')
        action = send_action(rospy.get_name())
        while not rospy.is_shutdown():
            action.over_and_over()

            