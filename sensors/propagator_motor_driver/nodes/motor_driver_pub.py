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
forward_c0 = rospy.get_param('~forward_c0')
forward_c1 = rospy.get_param('~forward_c1')
reverse_c0 = rospy.get_param('~reverse_c0')
reverse_c1 = rospy.get_param('~reverse_c1')

while True:
	try:
		motordriver = MotorDriver.MotorDriver(port)
	except:
		traceback.print_exc()
		time.sleep(1)
	else:
		break

def map_thruster_curve(direction,force):
	assert force >= 0
        if direction == "forward":
        	output = forward_c1 * force + forward_c0
        else:
        	assert direction == "reverse"
        	output = reverse_c1 * force + reverse_c0
	assert 0 <= output <= 1.1
	if output > 1: output = 1
        return output 

def apply_command(force):
	
	global message_received
	message_received = True
	#print 'speed: ',str(int(force*200/max_force)),' motor driver: ',thruster_id
	if (force > 0):
               	thrust = map_thruster_curve("forward",force)
		print "forward force = ",force,"forward thrust = ",thrust
                motordriver.set_forward_speed(thrust)
#      		motordriver.set_forward_speed(str(int(force*200/max_force)))
	elif (force < 0):
                thrust = map_thruster_curve("reverse",-force)
		print "reverse force = ",force,"reverse thrust = ",thrust		
                motordriver.set_reverse_speed(thrust)
	#	motordriver.set_reverse_speed(str(int(-force*200/min_force)))
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
