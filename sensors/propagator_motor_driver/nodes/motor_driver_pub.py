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
import math,time,serial


rospy.init_node('motor_driver')

motor_driver_statistics_publisher = rospy.Publisher('motor_driver_statistics', motor_driver_statistics)

message_received = False
thruster_id = rospy.get_param('~id')
min_force = rospy.get_param('~min_force')
max_force = rospy.get_param('~max_force')
thruster_position = rospy.get_param('~position')
thruster_direction = rospy.get_param('~direction')

motordriver = MotorDriver.MotorDriver(thruster_id)
'''
while True:
	try:
		motordriver = MotorDriver.MotorDriver(thruster_id)
	except serial.SerialException:
		rospy.logerr("Could not open all thruster ports, will keep trying to reconnect")
		time.sleep(1)
		continue
	except :
		break
	break
	
'''
def map_thruster_curve(direction,force):
        if direction="forward":
                return (.0266171798*np.exp(.0311079805*force))
                         
        else:
                return (.0328061857*force - 1.8306035094) 

def apply_command(force):
	
	global message_received
	message_received = True
	#print 'speed: ',str(int(force*200/max_force)),' motor driver: ',thruster_id
	if (force > 0):
                thrust = map_thruster_curve("forward",force)
                motordriver.set_forward_speed(str(int(thrust)))
      		#motordriver.set_forward_speed(str(int(force*200/max_force)))
	elif (force < 0):
                thrust = map_thruster_curve("reverse",force)
                motordriver.set_reverse_speed(str(int(thrust)))
		#motordriver.set_reverse_speed(str(int(-force*200/min_force)))
	else:
		motordriver.stop()
	'''	
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
	'''

	
lifetime = rospy.Duration(1.)	
print (thruster_id, lifetime, thruster_position, thruster_direction, -min_force, max_force,apply_command)
thruster_broadcaster = ThrusterBroadcaster('/base_link', thruster_id, lifetime, thruster_position, thruster_direction, -min_force, max_force,apply_command)

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
