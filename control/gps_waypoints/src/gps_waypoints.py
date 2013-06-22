#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('gps_waypoints')
from uf_common.msg import MoveToAction, MoveToGoal
from sensor_msgs.msg import NavSatFix
from nav_msgs.msg import Odometry
from rawgps_common.gps import ecef_from_latlongheight,enu_from_ecef
import threading
import actionlib

rospy.init_node('gps_waypoints')


waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
print 'connecting to action client'
waypoint.wait_for_server()

global pos
pos = []

def waypoint_callback(msg):
        #waypoint.send_goal(current_pose_editor.relative(numpy.array([point[0], point[1], 0])).as_MoveToGoal(speed = .1))
        global pos

        ecef = ecef_from_latlongheight(msg.latitude, msg.longitude, msg.altitude)
        diff = ecef - pos

        goal = enu_from_ecef(diff)
        waypoint.send_goal_and_wait('world',goal)


def pos_callback(msg):
        global pos
        pos = [msg.x,msg.y,msg.z]

rospy.Subscriber('/gps_latlong_waypoint',NavSatFix,waypoint_callback)
rospy.Subscriber('/gps_pos',NavSatFix,pos_callback)
rospy.spin()
