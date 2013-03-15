#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('propagator_motor_driver')
from propagator_motor_driver.msg import motor_driver_statistics
from thruster_handling.msg import ThrusterInfo, ThrusterCommand
from geometry_msgs.msg import WrenchStamped, Wrench, Vector3, Point
import MotorDriver
from std_msgs.msg import Header
import math


rospy.init_node('motor_drivers')

thrusterinfo_publisher = rospy.Publisher('thrusters/info', ThrusterInfo)
motor_driver_statistics_publisher = rospy.Publisher('motor_driver_statistics', motor_driver_statistics)

min_force = 1
max_force = 1
thruster_angle = math.radians(30)
message_received = False
thrusterinfo = {
'FR': [.729401, -.23495, -.241308, math.cos(thruster_angle), math.sin(thruster_angle), 0],
'FL': [.729401, .23495, -.241308, math.cos(thruster_angle), -math.sin(thruster_angle), 0],
'BR': [-.821269, -.23495, -.241308, math.cos(thruster_angle),-math.sin(thruster_angle),0],
'BL': [-.821269, .23495, -.241308, math.cos(thruster_angle), math.sin(thruster_angle), 0],
}

MotorDriver_fr = MotorDriver.MotorDriver('FR')
MotorDriver_fl = MotorDriver.MotorDriver('FL')
MotorDriver_br = MotorDriver.MotorDriver('BR')
MotorDriver_bl = MotorDriver.MotorDriver('BL')

def thrusterinfo_callback(event):
	global message_received

	print ('sending thruster info')	
	for key,value in thrusterinfo.iteritems():
		thrusterinfo_publisher.publish(ThrusterInfo(
			header=Header(
				stamp=rospy.Time.now(),
				frame_id="/base_link",
			),
			id=key,
			lifetime = rospy.Duration(2),
			position=Point(value[0],value[1],value[2]),
			direction=Vector3(value[3],value[4],value[5]),
			min_force = -min_force,
			max_force = max_force,
			active=True,
			))	
	if (not message_received):
		MotorDriver_fr.stop()
		MotorDriver_fl.stop()
		MotorDriver_br.stop()
		MotorDriver_bl.stop()
	else:
		message_received = False
	
rospy.Timer(rospy.Duration(.5), thrusterinfo_callback)


def command_callback(msg):
	global message_received
	message_received = True

	if (msg.id == "FR"):
		if (msg.force > 0):
			MotorDriver_fr.set_forward_speed(str(int(msg.force*200/max_force)))
		elif (msg.force < 0):
			MotorDriver_fr.set_reverse_speed(str(int(-msg.force*200/min_force)))
		else:
			MotorDriver_fr.stop()
		
		motordriverstat_msg = motor_driver_statistics(
			header=Header(
				stamp=rospy.Time.now(),
				frame_id="/base_link",
			),
			id=msg.id,
			current 	 = MotorDriver_fr.get_current(),
			out_voltage  = "0",#MotorDriver_fr.get_out_voltage(),
			batt_voltage = "0",#MotorDriver_fr.get_batt_voltage(),
			)		
	elif (msg.id == "FL"):
		if (msg.force > 0):
			MotorDriver_fl.set_forward_speed(str(int(msg.force*200/max_force)))
		elif (msg.force < 0):
			MotorDriver_fl.set_reverse_speed(str(int(-msg.force*200/min_force)))
		else:
			MotorDriver_fl.stop()

		motordriverstat_msg = motor_driver_statistics(
			header=Header(
				stamp=rospy.Time.now(),
				frame_id="/base_link",
			),
			id=msg.id,
			current 	 = MotorDriver_fl.get_current(),
			out_voltage  = "0",#MotorDriver_fr.get_out_voltage(),
			batt_voltage = "0",#MotorDriver_fr.get_batt_voltage(),
			)	
	elif (msg.id == "BR"):
		if (msg.force > 0):
			MotorDriver_br.set_forward_speed(str(int(msg.force*200/max_force)))
		elif (msg.force < 0):
			MotorDriver_br.set_reverse_speed(str(int(-msg.force*200/min_force)))
		else:
			MotorDriver_br.stop()

		motordriverstat_msg = motor_driver_statistics(
			header=Header(
				stamp=rospy.Time.now(),
				frame_id="/base_link",
			),
			id=msg.id,
			current 	 = MotorDriver_br.get_current(),
			out_voltage  = "0",#MotorDriver_fr.get_out_voltage(),
			batt_voltage = "0",#MotorDriver_fr.get_batt_voltage(),
			)
	elif (msg.id == "BL"):
		if (msg.force > 0):
			MotorDriver_bl.set_forward_speed(str(int(msg.force*200/max_force)))
		elif (msg.force < 0):
			MotorDriver_bl.set_reverse_speed(str(int(-msg.force*200/min_force)))
		else:
			MotorDriver_bl.stop()
			
		motordriverstat_msg = motor_driver_statistics(
			header=Header(
				stamp=rospy.Time.now(),
				frame_id="/base_link",
			),
			id=msg.id,
			current 	 = MotorDriver_bl.get_current(), 
			out_voltage  = "0",#MotorDriver_fr.get_out_voltage(),
			batt_voltage = "0",#MotorDriver_fr.get_batt_voltage(),
			)	
	print('xbox')
	motor_driver_statistics_publisher.publish(motordriverstat_msg)	
		
			
rospy.Subscriber('thrusters/command', ThrusterCommand, command_callback)
rospy.spin()
	

