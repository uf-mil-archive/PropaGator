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
import random,numpy,threading
import actionlib
from uf_common.msg import MoveToAction, MoveToGoal
from uf_common.orientation_helpers import lookat, get_perpendicular,PoseEditor


rospy.init_node('buoy_repulsor')

global current_position,buoy,channel_width,running
running = False
channel_width = 4
current_position = [0,0]
buoy = []

waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
print 'connecting to action client'
#waypoint.wait_for_server()

	
def find_closest_buoy(msg,color):
	global current_position,green_buoy,red_buoy,traversed_buoys,buoy
	
	for marker in msg.markers:
			if ((not buoy) and (marker.color == color)):
				buoy = [marker.pose.position.x,marker.pose.position.y]
			elif (buoy):
				if (distance((marker.pose.position.x,marker.pose.position.y),current_position) < distance(buoy,current_position) and (marker.color == color)):
					buoy = [marker.pose.position.x,marker.pose.position.y]
			
	if (not buoy):	
		return [False,[0,0]]
	else: 
	        return [True,buoy]
		

def distance (p1,p2):
	return (math.sqrt((p2[1]-p1[1])**2 + (p2[0]-p1[0])**2))
	
def center_of_points((p1,p2)):
	return numpy.mean([p1,p2],0)
	
def find_pair_center(red,green):
        if (distance(red,green) < channel_width and distance(red,green) > .3):
                return [True,center_of_points((red,green))]
        else:
                return [False,[0,0]]
def max_dist(x,ref):
        ans = x[0]
        for i in x:
                if (distance(i,ref) > ans):
                        i = ans
        return ans

def get_perp(p1,p2):
    x = p2[0]-p1[0]
    y = p2[1]-p1[1]
    vec = max_dist(([-y,x],[y,-x]),center_of_points((p1,p2))) 
    return vec/numpy.linalg.norm(vec)

def send_waypoint_wait(point,orientation):
	waypoint.send_goal_and_wait(current_pose_editor.relative(numpy.array([point[0], point[1], 0])).as_MoveToGoal(speed = .2))

def send_waypoint(point,orientation):
	waypoint.send_goal(current_pose_editor.relative(numpy.array([point[0], point[1], 0])).as_MoveToGoal(speed = .2))

	
def buoy_callback(msg):

        global current_position
        red = ColorRGBA(1.0,0,0,1.0)
        green = ColorRGBA(0,1.0,0,1.0)
        yellow = ColorRGBA(1.0,1.0,0,1.0)
        blue = ColorRGBA(0,0,1.0,1.0)	

        red_pos = find_closest_buoy(msg,red)
        green_pos = find_closest_buoy(msg,green)
        blue_pos = find_closest_buoy(msg,blue)
        yellow_pos = find_closest_buoy(msg,yellow)

       
        if (green_pos[0] and red_pos[0]):
	        goal = find_pair_center(red_pos[1],green_pos[1])
                if (goal[0]):
                        
                        mid_goal = goal[1] + 1.5*get_perp(red_pos[1],green_pos[1])
                        print 'going to center of channel', mid_goal
                        #waypoint.send_goal_and_wait(current_pose_editor.look_at_without_pitching(mid_goal))
	                #send_waypoint_wait(mid_goal,0)
                        #waypoint.send_goal_and_wait(current_pose_editor.look_at_without_pitching(goal[1]))
                        #waypoint.send_goal(current_pose_editor.forward(3).as_MoveToGoal(speed = .4))

        elif(green_pos[0]):
                print 'going to green buoy: ',green_pos[1]
                #send_waypoint(green_pos[1],0)
        elif(red_pos[0]):
                print 'going to red buoy: ',red_pos[1]
                #send_waypoint(red_pos[1],0)


rospy.Subscriber('buoy_markers',MarkerArray,buoy_callback)


def pose_callback(msg):
	global current_position,current_pose_editor
	current_pose_editor = PoseEditor.from_Odometry(msg)
	current_position = (msg.pose.pose.position.x,msg.pose.pose.position.y)
rospy.Subscriber('/odom', Odometry, pose_callback)

rospy.spin()


