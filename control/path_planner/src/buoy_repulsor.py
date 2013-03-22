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
from c3_trajectory_generator.msg import MoveToAction, MoveToGoal
from uf_common.orientation_helpers import lookat, get_perpendicular

from sim.vector import v, V

rospy.init_node('buoy_repulsor')

global current_position 
current_position = [0,0]

waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
print 'connecting to action client'
waypoint.wait_for_server()

	
def find_closest_buoys(msg):
	global current_position,green_buoy,red_buoy,traversed_buoys
	red = ColorRGBA(1.0,0,0,1.0)
	green = ColorRGBA(0,1.0,0,1.0)
	
	for marker in msg.markers:
		if (not ((marker.pose.position.x,marker.pose.position.y) in traversed_buoys)):
			if ((not red_buoy) and (marker.color == red)):
				red_buoy = (marker.pose.position.x,marker.pose.position.y)
			elif (marker.color == red):
				if (distance((marker.pose.position.x,marker.pose.position.y),current_position) < distance(red_buoy,current_position)):
					red_buoy = (marker.pose.position.x,marker.pose.position.y)
				
			if ((not green_buoy) and (marker.color == green)):
				green_buoy = (marker.pose.position.x,marker.pose.position.y)
			elif (marker.color == green):
				if (distance((marker.pose.position.x,marker.pose.position.y),current_position) < distance(green_buoy,current_position)):
					green_buoy = (marker.pose.position.x,marker.pose.position.y)
					
	if ((not red_buoy) or (not green_buoy)):	
		return [],[]
	else: 
		if (distance(red_buoy,green_buoy) < .7):
			return red_buoy,green_buoy
		else:
			return [],[]

def distance (p1,p2):
	return (math.sqrt((p2[1]-p1[1])**2 + (p2[0]-p1[0])**2))
	
def center_of_points((p1,p2)):
	return numpy.mean([p1,p2],0)
	
def three_d(x):
	return numpy.array([x[0], x[1], 0])

def send_waypoint(point,orientation):
	waypoint.send_goal_and_wait(MoveToGoal(
			header=Header(
				frame_id='/simmap',
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
	
def buoy_callback(msg):
	global current_position
	yellow = ColorRGBA(1.0,1.0,0,1.0)
	for marker in msg.markers:
		if (distance(current_position,(marker.pose.position.x,marker.pose.position.y)) < 1):
			#waypoint.cancel_goal()
			print 'goal canceled due to collision'
			#[p1,p2] = find_closest_buoys(msg)
			#goal = center_of_points((p1,p2))
			pos = three_d((marker.pose.position.x,marker.pose.position.y)) +(2,2,2)
			print 'correcting trajectory'
			send_waypoint(pos,lookat(three_d(pos)))
			time.sleep(.05)

rospy.Subscriber('buoys',MarkerArray,buoy_callback)


def pose_callback(msg):
	global current_position
	current_position = (msg.pose.pose.position.x,msg.pose.pose.position.y)
rospy.Subscriber('/sim_odom', Odometry, pose_callback)
rospy.spin()

