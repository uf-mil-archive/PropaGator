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
from geometry_msgs.msg import Vector3
from geometry_msgs.msg import Quaternion
def talker():
	#frame_id = rospy.get_param("~frameid", "base_footprint")
	#port = rospy.get_param("~port", "IMU")
	#baud = rospy.get_param("~baud", 115200)
	
	pub = rospy.Publisher('imu_data', Imu)
	rospy.init_node('imu_node')
	
	header = Header()
	header.stamp = rospy.Time.now()
	orientation = Quaternion()
	ang_vel = Vector3()
	linear_accel = Vector3()
	#imu = micro_3dm_gx1(port, baud)
	imu = micro_3dm_gx1()
	
	
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
		header.stamp = rospy.Time.now()
		header.frame_id = 'base_footprint'
		values = imu.get_packet(inst_vectors_bytes)
		if(not values):
			rospy.logwarn("packet recieving timed out")
			#reopen the port ?
		else:
			orientation.x = values[0]	
			orientation.y = values[1]
			orientation.z = values[2]
			orientation.w = 1
			orientation_cov = [0.01,0.,0.,0.,0.01,0.,0.,0.,0.01]
			linear_accel.x = values[3]
			linear_accel.y = values[4]
			linear_accel.z = values[5]
			linear_accel_cov = [0.01,0.,0.,0.,0.01,0.,0.,0.,0.01]
			ang_vel.x = values[6]
			ang_vel.y = values[7]
			ang_vel.z = values[8]
			ang_vel_cov = [0.01,0.,0.,0.,0.01,0.,0.,0.,0.01]

			msg = Imu(header, orientation, orientation_cov, ang_vel, ang_vel_cov, linear_accel, linear_accel_cov)
			#pub.publish(msg)
		
			try:
				pub.publish(msg)
			except rospy.ROSException:
				rospy.logerr('could not publish to topic')
			#pub.publish(Imu(header = None, orientation = None, orientation_covariance = None, angular_velocity = None,angular_velocity_covariance = None, linear_acceleration = None, linear_acceleration_covariance = None))
			
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException: pass
