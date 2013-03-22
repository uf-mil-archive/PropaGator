#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('path_planner')
from std_msgs.msg import Header
import math,time
from visualization_msgs.msg import Marker,MarkerArray
import random

from sim.vector import v, V

rospy.init_node('buoy_generator')

global bouy_array
bouy_publisher=rospy.Publisher('buoys',MarkerArray)
bouy_array=MarkerArray()
    
def append_marker(pos,color):
	marker = Marker()
	marker.header.frame_id = "/simmap"
	marker.type = marker.SPHERE
	marker.id = pos[0]*pos[1]
	marker.lifetime = rospy.Duration(0)
	marker.action = marker.ADD
	marker.scale.x = 0.2
	marker.scale.y = 0.2
	marker.scale.z = 0.2
	marker.color.a = 1.0
	marker.color.r = color[0]
	marker.color.g = color[1]
	marker.color.b = color[2]
	marker.pose.orientation.w = 1.0
	marker.pose.position.x = pos[0]
	marker.pose.position.y = pos[1]
	marker.pose.position.z = 0
	bouy_array.markers.append(marker)
	
def random_obstacle(r,g,b):
	append_marker((random.gauss(10,10),random.gauss(10,10)),(r,g,b))
	
def pub_bouy_pair(pos,angle):
    global bouy_array
    #buoys = [(pos + [0,i*3,0],(i,1-i,0)) for i in xrange(2)]
    channel_gap = 2
    green_pos = (pos[0] + math.cos(math.radians(angle))*(channel_gap/2.),pos[1] + math.sin(math.radians(angle))*(channel_gap/2.))
    red_pos = (pos[0] - math.cos(math.radians(angle))*(channel_gap/2.),pos[1] - math.sin(math.radians(angle))*(channel_gap/2.))
    
    append_marker(green_pos,(0,1.0,0))
    append_marker(red_pos,(1.0,0,0))
    

pub_bouy_pair((1,2),-15)
pub_bouy_pair((5,10),-35)
pub_bouy_pair((10,15),-50)
pub_bouy_pair((15,18),-80)
pub_bouy_pair((20,16),-90)

for i in range(20):
	random_obstacle(1.0,1.0,0)


'''
pub_bouy_pair(v(5, -1, 0))
pub_bouy_pair(v(10, -5, 0))
pub_bouy_pair(v(15, -2, 0))
'''


def bouy_callback(event):
    global bouy_array
    bouy_publisher.publish(bouy_array)

rospy.Timer(rospy.Duration(.5), bouy_callback)
rospy.spin()
