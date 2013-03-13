#!/usr/bin/env python

import roslib
roslib.load_manifest('gps_to_vel_py')
import rospy
from math import *
from gps_common.msg import GPSFix
from geometry_msgs.msg import Vector3Stamped
from geometry_msgs.msg import Vector3
from std_msgs.msg import Header


rospy.init_node('gps_to_vel_publisher')

mag_heading = 0
vel_pub = rospy.Publisher('/gps_vel', Vector3Stamped)

def get_gps_vel(data):
    speed = data.speed
    direction = data.track #degrees from north
    
    #must convert speed and direction to x and y velocities in a NED frame
    #x points north and y points east, i think
    y_vel = speed*sin(radians(direction))
    x_vel = speed*cos(radians(direction))
    
    now = rospy.Time.now()
    vel_pub.publish(Vector3Stamped(
                 header=Header(
                    stamp=now,
                    frame_id='/NED',
                ),
                vector=Vector3(
                    x = x_vel,
                    y = y_vel,
                    z = 0,
                ),
    ))
                
#def get_imu_mag(data):       
gps_vel_sub = rospy.Subscriber('/extended_fix', GPSFix, get_gps_vel)
#imu_mag_sub = rospy.Subscribter('/imu/mag_raw', Vector3Stamped, get_imu_mag)
while not rospy.is_shutdown():
    rospy.spin()
    #publish velocity in NED frame here

    
     
    
