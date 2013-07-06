#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('gps_waypoints')
from uf_common.msg import MoveToAction, MoveToGoal
from geometry_msgs.msg import PointStamped,Point
from sensor_msgs.msg import NavSatFix
from nav_msgs.msg import Odometry
from rawgps_common.gps import ecef_from_latlongheight,enu_from_ecef
import threading,numpy
import actionlib
from uf_common.orientation_helpers import lookat, get_perpendicular,PoseEditor
from uf_common.msg import MoveToAction, MoveToGoal
from gps_waypoints.msg import GoToWaypointAction
rospy.init_node('gps_waypoints')



#waypoint.wait_for_server()

global pos,origin
pos = [0,0,0]
origin = [0,0,0]

'''
def waypoint_ecef_callback(msg):
        global pos,origin,current_position

        ecef = [msg.point.x,msg.point.y,msg.point.z]
        diff = numpy.array(ecef) - numpy.array(pos)
        print 'diff',diff
        goal = enu_from_ecef(diff,origin)
        print 'ecef',ecef
        print 'pos',pos
        print 'goal',goal
        #waypoint.send_goal_and_wait(current_pose_editor.look_at_without_pitching(current_pose_editor.relative(numpy.array([goal[0],goal[1],0])).position))
        #waypoint.send_goal_and_wait(current_pose_editor.relative(numpy.array([goal[0], goal[1], 0])).as_MoveToGoal(speed = .8))

        final_goal = current_position + goal        
        waypoint.send_goal_and_wait(current_pose_editor.look_at_without_pitching([final_goal[0],final_goal[1],0]))
        waypoint.send_goal_and_wait(current_pose_editor.set_position([final_goal[0],final_goal[1],0]))
rospy.Subscriber('/gps_ecef_waypoint',PointStamped,waypoint_ecef_callback)
'''

def pos_callback(msg):
        global pos,origin
        
        if (origin == [0,0,0]):
                origin = [msg.point.x,msg.point.y,msg.point.z]
        pos = [msg.point.x,msg.point.y,msg.point.z]
rospy.Subscriber('/gps_conv/pos',PointStamped,pos_callback)

def pose_callback(msg):
	global current_pose_editor,current_position
        current_position = [msg.pose.pose.position.x,msg.pose.pose.position.y,0]
	current_pose_editor = PoseEditor.from_Odometry(msg)
rospy.Subscriber('/odom', Odometry, pose_callback)


class GoToWaypointServer:

 def __init__(self):
        self.server = actionlib.SimpleActionServer('go_waypoint', GoToWaypointAction, self.execute, False)
        self.server.start()
        self.waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
        print 'connecting to action client'     
        print "gps waypoint server started"

 def execute(self,goal):

        global pos,origin,current_position
   
        ecef = [goal.waypoint.x,goal.waypoint.y,goal.waypoint.z]
        diff = numpy.array(ecef) - numpy.array(pos)
        goal = enu_from_ecef(diff,origin)
        final_goal = current_position + goal      

        self.waypoint.send_goal_and_wait(current_pose_editor.look_at_without_pitching([final_goal[0],final_goal[1],0]))
        self.waypoint.send_goal_and_wait(current_pose_editor.set_position([final_goal[0],final_goal[1],0]))

        self.server.set_succeeded()
                

server = GoToWaypointServer()
rospy.spin()

