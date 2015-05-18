#! /usr/bin/env python

import roslib
import rospy

# Brings in the SimpleActionaction
import actionlib
from uf_common.msg import PoseTwistStamped, MoveToAction

class send_action:

    def __init__(self):
        self.client = actionlib.SimpleActionServer('moveto', MoveToAction, self.new_goal, False)
        self.trajectory_pub = rospy.Publisher('/trajectory', PoseTwistStamped, queue_size=10)
        self.client.start()
        self.to_goal_posetwist = PoseTwistStamped()
        self.to_goal_header = PoseTwistStamped()
        self.to_pose = PoseTwistStamped()

    def new_goal(self, goal):
        self.client.set_succeeded()
        self.to_pose.header = goal.header
        self.to_pose.posetwist = goal.posetwist


    def over_and_over(self):
        r = rospy.Rate(1)
        self.trajectory_pub.publish(self.to_pose)
        print self.to_pose
        r.sleep()



if __name__ == '__main__':
    
        # Initializes a rospy node so that the SimpleActionaction can
        # publish and subscribe over ROS.
        rospy.init_node('azi_waypoint')
        action = send_action()
        while(1):
            action.over_and_over()

            