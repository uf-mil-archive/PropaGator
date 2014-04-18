#!/usr/bin/env python
import roslib
roslib.load_manifest('laser_tf_py')
import rospy
from std_msgs.msg import Bool
from laser_tf_py.msg import ScanAngle
import tf
import math


def scan_angle_callback(angle_msg):
    
    min_angle = math.radians(angle_msg.min_angle)
    max_angle = math.radians(angle_msg.max_angle)

rospy.Subscriber("/lidar_angle", ScanAngle, scan_angle_callback)

if __name__ == '__main__':
    rospy.init_node('calib')
    
    listener = tf.TransformListener()
    
    f = open("laser_data.txt","w")
    
    while not rospy.is_shutdown():
        try:
            (trans,rot) = listener.lookupTransform('/base_link', '/laser', rospy.Time(0))
        #f.write(str(rospy.Time.now())+str(min_angle) +
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
            continue
    
    
    f.close()
