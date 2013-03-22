#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('path_planner')
from std_msgs.msg import Header,ColorRGBA
import math,time
from visualization_msgs.msg import Marker,MarkerArray
from c3_trajectory_generator.msg import MoveToAction, MoveToGoal
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Pose,Quaternion,Point
import numpy
import actionlib
from tf import transformations
from uf_common.orientation_helpers import lookat, get_perpendicular


rospy.init_node('path_planner')
waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
print 'connecting to action client'
waypoint.wait_for_server()

global current_position,green_buoy,red_buoy,traversed_buoys,all_buoys
current_position = (0,0)
green_buoy = []
red_buoy = []
traversed_buoys = []
all_buoys = []

def distance (p1,p2):
	return (math.sqrt((p2[1]-p1[1])**2 + (p2[0]-p1[0])**2))
	
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
		if (distance(red_buoy,green_buoy) < 4):
			return red_buoy,green_buoy
		else:
			return [],[]
		
			
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
	
def check_obstacle_in_path(msg,p1,p2):
	global current_position,traversed_buoys
	yellow = ColorRGBA(1.0,1.0,0,1.0)
	
	for marker in msg.markers:
		if (marker.color == yellow):
			center_pos = center_of_points((p1,p2))
			if (distance(current_position,center_pos) <= distance(current_position,(marker.pose.position.x,marker.pose.position.y))):
				print 'yellow buoy detected in path. redirecting'
				
				point1_pos = three_d(center_of_points((marker.pose.position.x,marker.pose.position.y),traversed_buoys[-1]))
				print 'going to point1',point1_pos
				send_waypoint(point1_pos,lookat(get_perpendicular(three_d((marker.pose.position.x,marker.pose.position.y))-three_d(traversed_buoys[-1]),(0,0,1)))) 
				
				point2_pos = three_d(center_of_points((marker.pose.position.x,marker.pose.position.y),p2))
				print 'going to point2',point2_pos
				send_waypoint(point2_pos,lookat(get_perpendicular(three_d((marker.pose.position.x,marker.pose.position.y))-three_d(p2),(0,0,1)))) 
				
				point3_pos = three_d(center_pos) - 1.5*get_perpendicular(three_d(p1)-three_d(p2),(0,0,1))
				print 'going to point3',point3_pos
				send_waypoint(point3_pos,lookat(get_perpendicular(three_d(p1)-three_d(p2),(0,0,1))))
				print 'done'
				return True
	return False
				
	
def buoy_callback(msg):
	global current_position,green_buoy,red_buoy,traversed_buoys
	
	[p1,p2] = find_closest_buoys(msg)
	
	if ((not p1) or (not p2)):
		pass
	else:
		if (traversed_buoys):
			previous_center = center_of_points((traversed_buoys[-2],traversed_buoys[-1]))
			point1_pos = three_d(previous_center) + get_perpendicular(three_d(p1)-three_d(p2),(0,0,1))
			print 'going to point1',point1_pos
			send_waypoint(point1_pos,lookat(get_perpendicular(three_d(traversed_buoys[-2])-three_d(traversed_buoys[-1]),(0,0,1))))
	
		
		center_pos = center_of_points((p1,p2))
		point2_pos = three_d(center_pos) - 1.5*get_perpendicular(three_d(p1)-three_d(p2),(0,0,1))
		
		
		print 'going to point2',point2_pos
		send_waypoint(point2_pos,lookat(get_perpendicular(three_d(p1)-three_d(p2),(0,0,1))))
		print 'done'
	
		traversed_buoys.append(p1)
		traversed_buoys.append(p2)
		red_buoy = []
		green_buoy = []

	
rospy.Subscriber('buoys',MarkerArray,buoy_callback)

def pose_callback(msg):
	global current_position
	current_position = (msg.pose.pose.position.x,msg.pose.pose.position.y)
rospy.Subscriber('/sim_odom', Odometry, pose_callback)
rospy.spin()
