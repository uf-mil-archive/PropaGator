#!/usr/bin/env python

import math,time
import traceback

import serial

import rospy
from geometry_msgs.msg import WrenchStamped, Wrench, Vector3, Point
from std_msgs.msg import Header

from thruster_handling.msg import ThrusterInfo, ThrusterCommand
from thruster_handling.broadcaster import ThrusterBroadcaster

from propagator_motor_driver import MotorDriver
from propagator_motor_driver.msg import motor_driver_statistics


rospy.init_node('motor_driver')

motor_driver_statistics_publisher = rospy.Publisher('motor_driver_statistics', motor_driver_statistics)


message_received = False
port = rospy.get_param('~port')
thruster_id = rospy.get_param('~id')
thruster_position = rospy.get_param('~position')
thruster_direction = rospy.get_param('~direction')
output_list = rospy.get_param('~output_list')
assert sorted(output_list) == output_list
force_list = rospy.get_param('~force_list')
assert sorted(force_list) == force_list
assert output_list[0] == -1
assert output_list[-1] == 1

while True:
	try:
		motordriver = MotorDriver.MotorDriver(port)
	except:
		traceback.print_exc()
		time.sleep(1)
	else:
		break

def map_curve(force):
	if force < force_list[0]: force = force_list[0]
	if force > force_list[-1]: force = force_list[-1]
	pairs = zip(force_list, output_list)
	for (left_force, left_output), (right_force, right_output) in zip(pairs[:-1], pairs[1:]):
		if left_force <= force <= right_force:
			x = (force - left_force)/(right_force - left_force)
			return x * (right_output - left_output) + left_output
	assert False

def apply_command(force):
	
	global message_received
	message_received = True
	#print 'speed: ',str(int(force*200/max_force)),' motor driver: ',thruster_id
	output = map_curve(force)
	if output > 0:
                motordriver.set_forward_speed(thrust)
	elif output < 0:
                motordriver.set_reverse_speed(-thrust)
	else:
		motordriver.stop()
		
	motordriverstat_msg = motor_driver_statistics(
		header=Header(
			stamp=rospy.Time.now(),
			frame_id="/base_link",
		),
		id=thruster_id,
		current 	 = "0",#motordriver.get_current(),
		out_voltage  = "0",#motordriver.get_out_voltage(),
		batt_voltage = "0",#motordriver.get_batt_voltage(),
		)	
	#motor_driver_statistics_publisher.publish(motordriverstat_msg)


	
lifetime = rospy.Duration(1.)	
thruster_broadcaster = ThrusterBroadcaster('/base_link', thruster_id, lifetime, thruster_position, thruster_direction, -(1 - reverse_c0)/reverse_c1, (1 - forward_c0)/forward_c1, [0, 0, 0], apply_command)

def thrusterinfo_callback(event):
	global message_received
	
	thruster_broadcaster.send()
	
	if not message_received:
		motordriver.stop()
	else:
		message_received = False
		
rospy.Timer(lifetime/2., thrusterinfo_callback)

rospy.spin()
motordriver.stop()
