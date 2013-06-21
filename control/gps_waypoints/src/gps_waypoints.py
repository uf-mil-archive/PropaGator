#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('gps_waypoints')
from uf_common.msg import MoveToAction, MoveToGoal
from sensor_msgs.msg import NavSatFix
from nav_msgs.msg import Odometry
import threading
import actionlib

rospy.init_node('gps_waypoints')

waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
print 'connecting to action client'
waypoint.wait_for_server()

def waypoint_callback(msg):
        #waypoint.send_goal(current_pose_editor.relative(numpy.array([point[0], point[1], 0])).as_MoveToGoal(speed = .1))
        waypoint.send_goal_and_wait('world',(msg.pose.pose.position.x,msg.pose.pose.position.y,0))


rospy.Subscriber('/gps_odom_waypoint',Odometry,waypoint_callback)
rospy.spin()
