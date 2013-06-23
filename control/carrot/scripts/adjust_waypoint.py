#!/usr/bin/env python

import roslib
roslib.load_manifest('carrot')
import rospy
import numpy
import actionlib
from c3_trajectory_generator.msg import MoveToAction, MoveToGoal
from uf_common.orientation_helpers import lookat, get_perpendicular
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point

rospy.init_node('adjust_carrot')



#-----------------------------------------------------------------------------------

waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
print 'connecting to action client'
#waypoint.wait_for_server()

def send_waypoint(point):
       

def distance (p1,p2):
	return (math.sqrt((p2[1]-p1[1])**2 + (p2[0]-p1[0])**2))

#-----------------------------------------------------------------------------------
def pose_callback(msg):
	global current_position,orientation
        orientation = msg.pose.pose.orientation
	current_position = (msg.pose.pose.position.x,msg.pose.pose.position.y)
        
rospy.Subscriber('/odom', Odometry, pose_callback)
#-----------------------------------------------------------------------------------
def carrot_callback(msg):
        

rospy.Subscriber('/carrot', Point,carrot_callback)
rospy.spin()
