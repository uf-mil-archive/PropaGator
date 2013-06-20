#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('path_planner')
from std_msgs.msg import Header
import math,time
from visualization_msgs.msg import Marker,MarkerArray
from std_msgs.msg import ColorRGBA
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Pose,Quaternion,Point
import random,numpy
import actionlib
from uf_common.msg import MoveToAction, MoveToGoal
from uf_common.orientation_helpers import lookat, get_perpendicular,PoseEditor

#from sim.vector import v, V

rospy.init_node('buoy_repulsor')

global current_position,buoy 
current_position = [0,0]
buoy = []

waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
print 'connecting to action client'
waypoint.wait_for_server()

	
def find_closest_buoy(msg):
	global current_position,green_buoy,red_buoy,traversed_buoys,buoy
	red = ColorRGBA(1.0,0,0,1.0)
	green = ColorRGBA(0,1.0,0,1.0)
        yellow = ColorRGBA(1.0,1.0,0,1.0)
	
	for marker in msg.markers:
			if ((not buoy) and (marker.color == yellow)):
				buoy = (marker.pose.position.x,marker.pose.position.y)
			else:
				if (distance((marker.pose.position.x,marker.pose.position.y),current_position) < distance(buoy,current_position) and (marker.color == yellow)):
					buoy = (marker.pose.position.x,marker.pose.position.y)
			
	if (not buoy):	
		return [100,100]
	else: 
	        return buoy
		

def distance (p1,p2):
	return (math.sqrt((p2[1]-p1[1])**2 + (p2[0]-p1[0])**2))
	
def center_of_points((p1,p2)):
	return numpy.mean([p1,p2],0)
	
def three_d(x):
	return numpy.array([x[0], x[1], 0])

def send_waypoint(point,orientation):
	waypoint.send_goal(current_pose_editor.relative(numpy.array([point[0], point[1], 0])).as_MoveToGoal(speed = .8))
	'''
	waypoint.send_goal(MoveToGoal(
			header=Header(
				frame_id='/world',
			),
			pose=Pose(
				position=Point(
					x=point[0],
					y=point[1],
					z=0,
				),
				orientation=Quaternion(*orientation),
			),
	))
	'''	
def buoy_callback(msg):
	global current_position
	yellow = ColorRGBA(1.0,1.0,0,1.0)	
	pos = find_closest_buoy(msg)
	#waypoint.cancel_goal()
	#pos = three_d((marker.pose.position.x,marker.pose.position.y)) +(2,2,2)
	if (pos[0] != 100):
		print 'going to: ',pos
		send_waypoint(pos,lookat(three_d(pos)))


rospy.Subscriber('buoy_markers',MarkerArray,buoy_callback)


def pose_callback(msg):
	global current_position,current_pose_editor
	current_pose_editor = PoseEditor.from_Odometry(msg)
	current_position = (msg.pose.pose.position.x,msg.pose.pose.position.y)
rospy.Subscriber('/odom', Odometry, pose_callback)
rospy.spin()

