#!/usr/bin/env python
from __future__ import print_function
import roslib; roslib.load_manifest('microstrain_3dmgx1')
import rospy
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
	pub = rospy.Publisher('imutopic', Imu)
	testpub = rospy.Publisher('test', String)
	rospy.init_node('IMU_talker')
	
	header = Header()
	header.stamp = rospy.Time.now()
	orientation = Quaternion()
	ang_vel = Vector3()
	linear_accel = Vector3()
	imu = micro_3dm_gx1()	
	#if(not imu.open_port()):
		#print error opening port here
	#	pass
	#imu.init_EEPROM()
	#if(not imu.set_continuous_command(inst_vectors)):
		#print error not setting continous command here
	#	pass
	while not rospy.is_shutdown():
		header.stamp = rospy.Time.now()
		header.frame_id = 'imu'
		#values = imu.get_packet(inst_vectors_bytes)
		'''	print('mag_x = ' + str(values[0]),end = '      ')
		print('mag_y = ' + str(values[1]),end = '      ')	
		print('mag_z = ' + str(values[2]))	
		print('acc_x = ' + str(values[3]),end = '      ')
		print('acc_y = ' + str(values[4]),end = '      ')
		print('acc_z = ' + str(values[5]))
		print('angrate_x = ' + str(values[6]),end = '      ')
		print('angrate_y = ' + str(values[7]),end = '      ')
		print('angrate_z = ' + str(values[8]))	
		print('ticks = ' + str(values[9]))'''
		orientation.x = 2.# float(values[0])
		orientation.y = 3.# float(values[1])
		orientation.z = 5.#float(values[2])
		orientation.w = 0.#float(1)
		orientation_cov = [-1.,0.,0.,0.,0.,0.,0.,0.,0.]
		linear_accel.x = 3.#float(values[3])
		linear_accel.y = 3.#float(values[4])
		linear_accel.z = 3.#float(values[5])
		linear_accel_cov = [-1.,0.,0.,0.,0.,0.,0.,0.,0.]
		ang_vel.x = 3.#float(values[6])
		ang_vel.y = 3.#float(values[7])
		ang_vel.z = 3.#float(values[8])
		ang_vel_cov = [-1.,0.,0.,0.,0.,0.,0.,0.,0.]
		
		#testpub.publish(String('hi'))
		#print('publishing!')
		#print('publishing!')
		msg = Imu(header, orientation, orientation_cov, ang_vel, ang_vel_cov, linear_accel, linear_accel_cov)
        #pub.publish(Imu(header, orientation, orientation_cov, ang_vel, ang_vel_cov, linear_accel, linear_accel_cov))
		#msg = Imu(header = None, orientation = None, orientation_covariance = None, angular_velocity = None,angular_velocity_covariance = None, linear_acceleration = None, linear_acceleration_covariance = None)
		pub.publish(msg)
		#try:
		#	pub.publish(msg)
		#except rospy.ROSException:
		#	print('error')
        #pub.publish(Imu(header = None, orientation = None, orientation_covariance = None, angular_velocity = None,angular_velocity_covariance = None, linear_acceleration = None, linear_acceleration_covariance = None))
        print('publishing!')
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException: pass
