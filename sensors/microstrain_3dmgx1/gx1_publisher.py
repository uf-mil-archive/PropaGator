#!/usr/bin/python
from __future__ import print_function
import roslib; roslib.load_manifest('microstrain_3dmgx1')
import rospy
import os
from std_msgs.msg import Header
from std_msgs.msg import String
from sensor_msgs.msg import Imu
from geometry_msgs.msg import Quaternion
from geometry_msgs.msg import Vector3
from src.micro_3dm_gx1 import *
#from constants import *
from geometry_msgs.msg import Vector3, Vector3Stamped
from geometry_msgs.msg import Quaternion
def talker():
    
    rospy.init_node('imu_node')
    
    frame_id = rospy.get_param("~frame_id", "/imu")
    port = rospy.get_param("~port", "/dev/IMU")
    #baud = rospy.get_param("~baud", 115200)
    
    pub = rospy.Publisher('/imu/data_raw', Imu)
    mag_pub = rospy.Publisher('/imu/mag_raw', Vector3Stamped)
    
    #imu = micro_3dm_gx1(port, baud)
    imu = micro_3dm_gx1(port)
    
    while(not imu.open_port()):
        rospy.logerr('failed to open imu serial port')
    rospy.loginfo('successfully opened port')
    
    while(not imu.init_EEPROM()):
        rospy.logwarn('failed to init the IMU EEPROM')
    rospy.loginfo('successfully initialized the EEPROM')    
        
    while(not imu.set_continuous_command(inst_vectors)):
        rospy.logwarn('failed to set imu to stream inst. vectors')
    rospy.loginfo('successfully set imu to stream continuosly')
        
    while not rospy.is_shutdown():
        values = imu.get_packet(inst_vectors_bytes)
        now = rospy.Time.now()
        
        if not values:
            rospy.logwarn("packet receiving timed out")
            while(not imu.set_continuous_command(inst_vectors)):
                rospy.logwarn('failed to set imu to stream inst. vectors')
            continue
        
        try:
            pub.publish(Imu(
                header=Header(
                    stamp=now,
                    frame_id=frame_id,
                ),
                orientation=Quaternion(0, 0, 0, 0),
                orientation_covariance=[-1] + [0]*8,
                angular_velocity=Vector3(values[6]*imu.ang_scale, values[7]*imu.ang_scale, values[8]*imu.ang_scale),
                angular_velocity_covariance=[.03**2,0,0, 0,.03**2,0, 0,0,.03**2],
                linear_acceleration=Vector3(values[3]*imu.acc_scale, values[4]*imu.acc_scale, values[5]*imu.acc_scale),
                linear_acceleration_covariance=[.02**2,0,0, 0,.02**2,0, 0,0,.02**2],
            ))
            
            mag_pub.publish(Vector3Stamped(
                header=Header(
                    stamp=now,
                    frame_id=frame_id,
                ),
                vector=Vector3(values[0]*imu.mag_scale, values[1]*imu.mag_scale, values[2]*imu.mag_scale),
            ))
        except rospy.ROSException:
            rospy.logerr('could not publish to topic')
            
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException: pass
