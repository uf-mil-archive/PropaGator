#!/usr/bin/env python

import roslib
roslib.load_manifest('path_planner')

import rospy
import actionlib
from uf_common.msg import MoveToAction, MoveToGoal
from visualization_msgs.msg import Marker,MarkerArray
from nav_msgs.msg import Odometry
from std_msgs.msg import Header,ColorRGBA
from geometry_msgs.msg import Pose,Quaternion,Point,PointStamped
from path_planner.msg import TraverseBuoysAction
import random,numpy,threading,math,time
from helper_functions import *
from uf_common.orientation_helpers import lookat, get_perpendicular,PoseEditor


rospy.init_node('buoy_repulsor')
global current_position,channel_width,end_position,ecef_position
channel_width = 8
current_position = [0,0]
end_position = [0,0,0]
ecef_position = [1000,10000,0]

waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
print 'connecting to action client'
#waypoint.wait_for_server()


def find_closest_buoy(msg,color):
	global current_position,green_buoy,red_buoy
	
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

def find_best_pair_center(msg):
        global current_position
        red = ColorRGBA(1.0,0,0,1.0)
        green = ColorRGBA(0,1.0,0,1.0)
        green_buoy = []
        red_buoy = []
        ans = []
        min_dist = 100
	
	for marker in msg.markers:
			if (marker.color == red):
				red_buoy.append([marker.pose.position.x,marker.pose.position.y])
			if (marker.color == green):
				green_buoy.append([marker.pose.position.x,marker.pose.position.y])
        for i in red_buoy:
                for j in green_buoy:
                        dist = distance(i,j)
                        if (dist > 1.0 and dist < channel_width and distance(current_position,center_of_points((i,j))) < min_dist):
                                ans = [i,j]
			
	if (not ans):	
		return [False,[0,0]]
	else: 
	        return [True,ans]

def send_waypoint_wait(point,orientation):
	waypoint.send_goal_and_wait(current_pose_editor.relative(numpy.array([point[0], point[1], 0])).as_MoveToGoal(speed = .2))

def send_waypoint(point,orientation):
	waypoint.send_goal(current_pose_editor.relative(numpy.array([point[0], point[1], 0])).as_MoveToGoal(speed = .2))
	
def buoy_callback(msg):
        global running
        if (running):
                global current_position
                red = ColorRGBA(1.0,0,0,1.0)
                green = ColorRGBA(0,1.0,0,1.0)
                yellow = ColorRGBA(1.0,1.0,0,1.0)
                blue = ColorRGBA(0,0,1.0,1.0)

                red_pos = find_closest_buoy(msg,red)
                green_pos = find_closest_buoy(msg,green)
                blue_pos = find_closest_buoy(msg,blue)
                yellow_pos = find_closest_buoy(msg,yellow)

                goal = find_best_pair_center(msg)
                '''
                if (green_pos[0] and red_pos[0]):
	                goal = find_pair_center(red_pos[1],green_pos[1])
                        if (goal[0]):
                                
                                mid_goal = goal[1] + 1.5*get_perp(red_pos[1],green_pos[1])
                                print 'going to center of channel', mid_goal
                                waypoint.send_goal_and_wait(current_pose_editor.look_at_without_pitching(current_pose_editor.relative(numpy.array([mid_goal[0].mid_goal[1],0])).position))
	                        send_waypoint_wait(mid_goal,0)
                                waypoint.send_goal_and_wait(current_pose_editor.look_at_without_pitching(goal[1]))
                                waypoint.send_goal(current_pose_editor.forward(3).as_MoveToGoal(speed = .4))
                '''
                if (goal[0]):
                        point = center_of_points((goal[1][0],goal[1][1]))
                        mid_goal = point + 1.5*get_perp(goal[1][0],goal[1][1])
                        print 'going to center of channel', mid_goal
                
                        waypoint.send_goal_and_wait(current_pose_editor.look_at_without_pitching(current_pose_editor.relative(numpy.array([mid_goal[0],mid_goal[1],0])).position))
                        print 'aligned'
                        print 'going for mid_goal',mid_goal
                        send_waypoint_wait(mid_goal,0)
                        print 'align again'
                        waypoint.send_goal_and_wait(current_pose_editor.look_at_without_pitching(point))
                        print 'open loop'
                        waypoint.send_goal(current_pose_editor.forward(3).as_MoveToGoal(speed = .4))
                        print 'done'
                elif(green_pos[0]):
                        print 'going to green buoy: ',green_pos[1]
                        #send_waypoint((green_pos[1][0],green_pos[1][0] + 1),0)
                elif(red_pos[0]):
                        print 'going to red buoy: ',red_pos[1]
                        #send_waypoint((red_pos[1][0],red_pos[1][0] - 1),0)
rospy.Subscriber('buoy_markers',MarkerArray,buoy_callback)

#-----------------------------------------------------------------------------------------
def pose_callback(msg):
	global current_position,current_pose_editor
	current_pose_editor = PoseEditor.from_Odometry(msg)
	current_position = (msg.pose.pose.position.x,msg.pose.pose.position.y)
rospy.Subscriber('/odom', Odometry, pose_callback)

def ecef_callback(msg):
	global ecef_position
	ecef_position = [msg.point.x,msg.point.y,msg.point.z]
rospy.Subscriber('/gps2_parser/pos', PointStamped, ecef_callback)

def task_pos_callback(msg):
        global end_position
        end_position = [msg.point.x,msg.point.y,msg.point.z]
rospy.Subscriber("/task_waypoints",PointStamped,task_pos_callback)

#-----------------------------------------------------------------------------------------

class TraverseBuoysServer:

 def __init__(self):
        self.server = actionlib.SimpleActionServer('traverse_buoys', TraverseBuoysAction, self.execute, False)
        global running
        running = False
        self.server.start()

 def execute(self,goal):
        global running
        while ((numpy.linalg.norm(numpy.array(ecef_position)-numpy.array(end_position)) > 5) and not(self.server.is_preempt_requested())):
             running = True
        running = False

        if (numpy.linalg.norm(numpy.array(ecef_position)-numpy.array(end_position)) < 5):
                self.server.set_succeeded()
        else:
                self.server.set_preempted()                  

server = TraverseBuoysServer()
rospy.spin()


