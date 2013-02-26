#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('propagator_motor_driver')
from propagator_motor_driver.msg import motor_driver_statistics
from thruster_mapper.msg import ThrusterInfo, ThrusterCommand
from geometry_msgs.msg import WrenchStamped, Wrench, Vector3, Point
import MotorDriver
import tf
from tf import transformations
from std_msgs.msg import Header
import math

MotorDriver_fr = MotorDriver.MotorDriver('FR')
MotorDriver_fl = MotorDriver.MotorDriver('FL')
MotorDriver_br = MotorDriver.MotorDriver('BR')
MotorDriver_bl = MotorDriver.MotorDriver('BL')

rospy.init_node('motor_drivers')

thrusterinfo_publisher = rospy.Publisher('thrusters/info', ThrusterInfo)
motor_driver_statistics_publisher = rospy.Publisher('motor_driver_statistics', motor_driver_statistics)

thruster_angle = 30
min_force = 1
max_force = 1

def thrusterinfo_callback(event):
	thrusterinfo_publisher.publish(ThrusterInfo(
			header=Header(
				stamp=rospy.Time.now(),
				frame_id="/base_link",
			),
			id="fr",
			lifetime = rospy.Duration(2),
			position=Point(x = .729401, y = -.23495, z = -.241308),
			direction=Vector3(x = math.cos(thruster_angle), y = math.sin(thruster_angle), z = 0),
			min_force = -min_force,
			max_force = max_force,
			))	
	#thrusterinfo_publisher.publish(thrusterinfo_msg)
	
	thrusterinfo_publisher.publish(ThrusterInfo(	
			header=Header(
				stamp=rospy.Time.now(),
				frame_id="/base_link",
			),
			id="fl",
			lifetime = rospy.Duration(2),
			position=Point(.729401,.23495,-.241308),
			direction=Vector3(math.cos(thruster_angle),-math.sin(thruster_angle),0),
			min_force = -min_force,
			max_force = max_force,
			))
	#thrusterinfo_publisher.publish(thrusterinfo_msg)
	
	thrusterinfo_publisher.publish(ThrusterInfo(
			header=Header(
				stamp=rospy.Time.now(),
				frame_id="/base_link",
			),
			id="br",
			lifetime = rospy.Duration(2),
			position=Point(-.821269,-.23495,-.241308),
			direction=Vector3(math.cos(thruster_angle),-math.sin(thruster_angle),0),
			min_force = -min_force,
			max_force = max_force,
			))
	#thrusterinfo_publisher.publish(thrusterinfo_msg)
	
	thrusterinfo_publisher.publish(ThrusterInfo(
			header=Header(
				stamp=rospy.Time.now(),
				frame_id="/base_link",
			),
			id="bl",
			lifetime = rospy.Duration(2),
			position=Point(-.821269,.23495,-.241308),
			direction=Vector3(math.cos(thruster_angle),math.sin(thruster_angle),0),
			min_force = -min_force,
			max_force = max_force,
			))
	#thrusterinfo_publisher.publish(thrusterinfo_msg)
	
rospy.Timer(rospy.Duration(.5), thrusterinfo_callback)
	

def command_callback(msg):

	if (msg.id == "fr"):
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
			id="fr",
			current 	 = MotorDriver_fr.get_current(),
			out_voltage  = "0",#MotorDriver_fr.get_out_voltage(),
			batt_voltage = "0",#MotorDriver_fr.get_batt_voltage(),
			)		
	elif (msg.id == "fl"):
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
			id="fl",
			current 	 = MotorDriver_fl.get_current(),
			out_voltage  = "0",#MotorDriver_fr.get_out_voltage(),
			batt_voltage = "0",#MotorDriver_fr.get_batt_voltage(),
			)	
	elif (msg.id == "br"):
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
			id="br",
			current 	 = MotorDriver_br.get_current(),
			out_voltage  = "0",#MotorDriver_fr.get_out_voltage(),
			batt_voltage = "0",#MotorDriver_fr.get_batt_voltage(),
			)
	elif (msg.id == "bl"):
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
			id="bl",
			current 	 = MotorDriver_bl.get_current(),
			out_voltage  = "0",#MotorDriver_fr.get_out_voltage(),
			batt_voltage = "0",#MotorDriver_fr.get_batt_voltage(),
			)	
		
	motor_driver_statistics_publisher.publish(motordriverstat_msg)					
			
rospy.Subscriber('thrusters/command', ThrusterCommand, command_callback)
rospy.spin()
	

