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

rospy.init_node('gps_waypoints')


waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
print 'connecting to action client'
#waypoint.wait_for_server()

global pos,origin
pos = [0,0,0]
origin = [0,0,0]

def waypoint_latlong_callback(msg):
        global pos,origin

        ecef = ecef_from_latlongheight(msg.latitude, msg.longitude, msg.altitude)
        diff = numpy.array(ecef) - numpy.array(pos)

        goal = enu_from_ecef(diff,origin)
        print goal
        #waypoint.send_goal_and_wait('world',(goal[0],goal[1],0))

def waypoint_ecef_callback(msg):
        global pos,origin

        ecef = [msg.point.x,msg.point.y,msg.point.z]
        diff = numpy.array(ecef) - numpy.array(pos)
        print 'diff',diff
        goal = enu_from_ecef(diff,origin)
        print 'ecef',ecef
        print 'pos',pos
        print 'goal',goal
        #waypoint.send_goal_and_wait('world',(goal[0],goal[1],0))


def pos_callback(msg):
        global pos,origin

        if (origin == [0,0,0]):
                origin = [msg.point.x,msg.point.y,msg.point.z]
        pos = [msg.point.x,msg.point.y,msg.point.z]

rospy.Subscriber('/gps_latlong_waypoint',NavSatFix,waypoint_latlong_callback)
rospy.Subscriber('/gps_ecef_waypoint',PointStamped,waypoint_ecef_callback)
rospy.Subscriber('/gps2_parser/pos',PointStamped,pos_callback)
rospy.spin()
