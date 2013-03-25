#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('propagator_motor_driver')
from propagator_motor_driver.msg import motor_driver_statistics
from thruster_handling.msg import ThrusterInfo, ThrusterCommand
from thruster_handling.broadcaster import ThrusterBroadcaster
from geometry_msgs.msg import WrenchStamped, Wrench, Vector3, Point
import MotorDriver
from std_msgs.msg import Header
import math,time


rospy.init_node('motor_drivers')

motor_driver_statistics_publisher = rospy.Publisher('motor_driver_statistics', motor_driver_statistics)

min_force = 1
max_force = 1
thruster_angle = math.radians(30)
message_received = False

while True:
	try:
	   motordrivers = {
		'FR': MotorDriver.MotorDriver('FR'),
		'FL': MotorDriver.MotorDriver('FL'),
		'BR': MotorDriver.MotorDriver('BR'),
		'BL': MotorDriver.MotorDriver('BL'),
	   }
	except :
		rospy.logerr("Could not open all thruster ports, will keep trying to reconnect to")
		time.sleep(1)
		continue
	break

def apply_command(thruster_id, force):
	motordriver = motordrivers[thruster_id]
	
	global message_received
	message_received = True
	
	if (force > 0):
		motordriver.set_forward_speed(str(int(force*200/max_force)))
	elif (force < 0):
		motordriver.set_reverse_speed(str(int(-force*200/min_force)))
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
	motor_driver_statistics_publisher.publish(motordriverstat_msg)
	
lifetime = rospy.Duration(1.)
thrusterbroadcasters = [
	ThrusterBroadcaster('/base_link', 'FR', lifetime, [.729401, -.23495, -.241308], [math.cos(thruster_angle), math.sin(thruster_angle), 0], -min_force, max_force, lambda force: apply_command('FR', force)),
	ThrusterBroadcaster('/base_link', 'FL', lifetime, [.729401, .23495, -.241308], [math.cos(thruster_angle), -math.sin(thruster_angle), 0], -min_force, max_force, lambda force: apply_command('FL', force)),
	ThrusterBroadcaster('/base_link', 'BR', lifetime, [-.821269, -.23495, -.241308], [math.cos(thruster_angle), -math.sin(thruster_angle), 0], -min_force, max_force, lambda force: apply_command('BR', force)),
	ThrusterBroadcaster('/base_link', 'BL', lifetime, [-.821269, .23495, -.241308], [math.cos(thruster_angle), math.sin(thruster_angle), 0], -min_force, max_force, lambda force: apply_command('BL', force)),
]

def thrusterinfo_callback(event):
	global message_received
	
	for thrusterbroadcaster in thrusterbroadcasters:
		thrusterbroadcaster.send()
	
	if not message_received:
		for motordriver in motordrivers.values():
			motordriver.stop()
	else:
		message_received = False
rospy.Timer(lifetime/2., thrusterinfo_callback)

rospy.spin()

for motordriver in motordrivers.values():
	motordriver.stop()
