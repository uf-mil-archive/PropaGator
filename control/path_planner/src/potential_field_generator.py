#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('path_planner')
from std_msgs.msg import Header
import math,time
from visualization_msgs.msg import Marker,MarkerArray
from std_msgs.msg import ColorRGBA
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Pose,Quaternion,Point,WrenchStamped,Wrench,Vector3
import random,numpy
import actionlib
from c3_trajectory_generator.msg import MoveToAction, MoveToGoal
from uf_common.orientation_helpers import lookat, get_perpendicular
from sim.vector import v, V

rospy.init_node('potential_field_generator')
wrench_publisher = rospy.Publisher('wrench',WrenchStamped)

#-----------------------------------------------------------------------

global traversed_buoys,red_buoys,green_buoys,current_position
traversed_buoys = []
red_buoy = []
green_buoy = []
current_position = []


#-----------------------------------------------------------------------

def distance (p1,p2):
	return (math.sqrt((p2[1]-p1[1])**2 + (p2[0]-p1[0])**2))

#-----------------------------------------------------------------------

def three_d(x):
	return numpy.array([x[0], x[1], 0])
	
#-----------------------------------------------------------------------

def find_closest_buoys(msg):
	global current_position,green_buoy,red_buoy,traversed_buoys
	red = ColorRGBA(1.0,0,0,1.0)
	green = ColorRGBA(0,1.0,0,1.0)
	
	for marker in msg.markers:
		if (not ((marker.pose.position.x,marker.pose.position.y) in traversed_buoys)):
			if ((not red_buoy) and (marker.color == red)):
				red_buoy = (marker.pose.position.x,marker.pose.position.y)
			elif (marker.color == red):                                                                                                 se.position.y),current_position) < distance(red_buoy,current_position)):
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

#-----------------------------------------------------------------------

def attractive_vector(msg):
        global current_position,green_buoy,red_buoy,traversed_buoys
	red = ColorRGBA(1.0,0,0,1.0)
	green = ColorRGBA(0,1.0,0,1.0)
	attractive_points = []

	for marker in msg.markers:
		if (not ((marker.pose.position.x,marker.pose.position.y) in traversed_buoys)):
                        if ((marker.color == red) or (marker.color == green)):
                                pos = [marker.pose.position.x,marker.pose.position.y] 
                                attractive_points.append(((pos[0]-current_position[0])/(distance(current_position,pos)**2),(pos[1] - current_position[1])/(distance(current_position,pos)**2)))
        print attractive_points
        return numpy.mean(attractive_points,0)
                                
			
#-----------------------------------------------------------------------

def center_of_points((p1,p2)):
	return numpy.mean([p1,p2],0)

#-----------------------------------------------------------------------

def buoy_callback(msg):
	global current_position,traversed_buoys
	red = ColorRGBA(1.0,0,0,1.0)
	green = ColorRGBA(0,1.0,0,1.0)
	yellow = ColorRGBA(1.0,1.0,0,1.0)
	obstacles = []
	
	for marker in msg.markers:
		if (marker.color == yellow):
			obstacles.append((marker.pose.position.x,marker.pose.position.y))
	
	#lookat(get_perpendicular(three_d(p1)-three_d(p2),(0,0,1)))

        attractive_force = attractive_vector(msg)
        print 'attractive force', attractive_force

	if (numpy.mean(attractive_force) < .1):
                print 'at buoy'
                [p1,p2] = find_closest_buoys(msg)
		traversed_buoys.append(p1)
		traversed_buoys.append(p2)
	else:
		waypoint.send_goal(current_pose_editor.relative(numpy.array([point[0], point[1], 0])).as_MoveToGoal(speed = .1))	

#-----------------------------------------------------------------------

rospy.Subscriber('buoys',MarkerArray,buoy_callback)

#-----------------------------------------------------------------------

def pose_callback(msg):
	global current_position
	current_position = (msg.pose.pose.position.x,msg.pose.pose.position.y)

#-----------------------------------------------------------------------

rospy.Subscriber('/odom', Odometry, pose_callback)
rospy.spin()






