#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('propagator_motor_driver')
from propagator_motor_driver.msg import motor_driver_statistics
from thruster_mapper.msg import ThrusterInfo, ThrusterCommand
import MotorDriver
import tf
from tf import transformations

MotorDriver_fr = MotorDriver.MotorDriver('MotorDriver1')
#MotorDriver_fl = MotorDriver.MotorDriver('MotorDriver2')
#MotorDriver_br = MotorDriver.MotorDriver('MotorDriver3')
#MotorDriver_bl = MotorDriver.MotorDriver('MotorDriver4')

rospy.init_node('motor_drivers')

thrusterinfo_publisher = rospy.Publisher('thrusters/info', ThrusterInfo)
motor_driver_statistics_publisher = rospy.Publisher('motor_driver_statistics', motor_driver_statistics)
	
def command_callback(msg):
	if (msg.id == "fr"):
		if (msg.velocity > 0):
			#MotorDriver_fr.set_forward_speed(str(msg.velocity))
			pass	
		elif (msg.velocity < 0):
			#MotorDriver_fr.set_reverse__speed(str(-msg.velocity))
			pass
		else:
			#MotorDriver_fr.stop()
			pass
		thruserinfo_msg = ThrusterInfo(
			header=Header(
				stamp=msg.header.stamp,
				frame_id=ti.header.frame_id,
			),
			id="fr",
			position=[.23495,.729401,-.241308],
			direction=[1,1,1],
			min_force = -1,
			max_force = 2,
			)
		motordriverstat_msg = motor_driver_statistics(
			header=Header(
				stamp=msg.header.stamp,
				frame_id=ti.header.frame_id,
			),
			id="fr",
			current 	 = MotorDriver_fr.get_current(),
			out_voltage  = MotorDriver_fr.get_out_voltage(),
			batt_voltage = MotorDriver_fr.get_batt_voltage(),
			)			
	elif (msg.id == "fl"):
		if (msg.velocity > 0):
			#MotorDriver_fl.set_forward_speed(str(msg.velocity))
			pass	
		elif (msg.velocity < 0):
			#MotorDriver_fl.set_reverse__speed(str(-msg.velocity))
			pass
		else:
			#MotorDriver_fl.stop()
			pass
		thruserinfo_msg = ThrusterInfo(
			header=Header(
				stamp=msg.header.stamp,
				frame_id=ti.header.frame_id,
			),
			id="fl",
			position=[-.23495,.729401,-.241308],
			direction=[1,1,1],
			min_force = -1,
			max_force = 2,
			)
		motordriverstat_msg = motor_driver_statistics(
			header=Header(
				stamp=msg.header.stamp,
				frame_id=ti.header.frame_id,
			),
			id="fl",
			current 	 = MotorDriver_fr.get_current(),
			out_voltage  = MotorDriver_fr.get_out_voltage(),
			batt_voltage = MotorDriver_fr.get_batt_voltage(),
			)	
	elif (msg.id == "br"):
		if (msg.velocity > 0):
			#MotorDriver_br.set_forward_speed(str(msg.velocity))
			pass	
		elif (msg.velocity < 0):
			#MotorDriver_br.set_reverse__speed(str(-msg.velocity))
			pass
		else:
			#MotorDriver_br.stop()
			pass
		thruserinfo_msg = ThrusterInfo(
			header=Header(
				stamp=msg.header.stamp,
				frame_id=ti.header.frame_id,
			),
			id="br",
			position=[.23495,-.821269,-.241308],
			direction=[1,1,1],
			min_force = -1,
			max_force = 2,
			)
		motordriverstat_msg = motor_driver_statistics(
			header=Header(
				stamp=msg.header.stamp,
				frame_id=ti.header.frame_id,
			),
			id="br",
			current 	 = MotorDriver_fr.get_current(),
			out_voltage  = MotorDriver_fr.get_out_voltage(),
			batt_voltage = MotorDriver_fr.get_batt_voltage(),
			)
	elif (msg.id == "bl"):
		if (msg.velocity > 0):
			#MotorDriver_bl.set_forward_speed(str(msg.velocity))
			pass	
		elif (msg.velocity < 0):
			#MotorDriver_bl.set_reverse__speed(str(-msg.velocity))
			pass
		else:
			#MotorDriver_bl.stop()
			pass
		thruserinfo_msg = ThrusterInfo(
			header=Header(
				stamp=msg.header.stamp,
				frame_id=ti.header.frame_id,
			),
			id="bl",
			position=[-.23495,-.821269,-.241308],
			direction=[1,1,1],
			min_force = -1,
			max_force = 2,
			)
		motordriverstat_msg = motor_driver_statistics(
			header=Header(
				stamp=msg.header.stamp,
				frame_id=ti.header.frame_id,
			),
			id="bl",
			current 	 = MotorDriver_fr.get_current(),
			out_voltage  = MotorDriver_fr.get_out_voltage(),
			batt_voltage = MotorDriver_fr.get_batt_voltage(),
			)	
			
						
	thrusterinfo_publisher.publish(thrusterinfo_msg)
	motor_driver_statistics_publisher.publish(motordriverstat_msg) 							
			
rospy.Subscriber('thrusters/command', ThrusterCommand, command_callback)
rospy.spin()
	

