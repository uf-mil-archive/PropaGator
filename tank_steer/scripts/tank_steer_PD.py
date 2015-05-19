#!/usr/bin/python

#				Tank Steer Primative Driver
# This node maps a wrench message to a thruster configuration with the thrusters locked in the reverse position
#	Note any refrence to a cordinate system in this node unless otherwise specified is to the robot frame
#		with x looking forward towards the bow, z is up and standard right hand conventions apply

from __future__ import division

import math
import itertools
import signal
import traceback

from twisted.internet import reactor, defer
import numpy as np
import scipy.optimize

from uf_common.orientation_helpers import xyz_array
from uf_common import interpolate

from geometry_msgs.msg import WrenchStamped

from motor_control.msg import thrusterNewtons
from dynamixel_servo.msg import DynamixelFullConfig
from std_msgs.msg import Float64

import rospy

class Node(object):

# Constructor
	def __init__(self):
		rospy.init_node('tank_steer_pd', anonymous=True)

	# Grab params
		if rospy.has_param('~simulate'):
			self.simulate = rospy.get_param('~simulate')
		else:
			self.simulate = 0


		self.max_thrust = rospy.get_param('max_thrust', 100.0)
		self.min_thrust = rospy.get_param('min_thrust', -100.0)

		self.L = rospy.get_param('distance_between_thrusters', 0.6096) #meters

	# Set up publishers for servo position and prop speed
		self.thrust_pub = rospy.Publisher('thruster_config', thrusterNewtons, queue_size = 10)
		self.servo_pub = rospy.Publisher('dynamixel/dynamixel_full_config', DynamixelFullConfig, queue_size = 10)

	# Get subscriber to wrench topic (from trajectory generator)
		self.wrench_sub = rospy.Subscriber('wrench', WrenchStamped, self._wrench_cb, queue_size = 10)

	# Setup some constants
		# Define thrustors and there position
		# TODO: should be retrieved and not hard coded
		"""
		self.thrusters = [
			dict( # port
				id=3,
				pwm_pub=		rospy.Publisher('stm32f3discovery_imu_driver/pwm1', Float64, queue_size = 10),
				angle_pub=		rospy.Publisher('port_angle', Float64, queue_size = 10),
			),
			dict( # starboard
				id=2,
				position=		np.array([-.5239, -.3048, -.5]),
				angle=			0,
				#thrust_range=	(min(calib_data[0]), max(calib_data[0])),
				dangle_range=	(-3.2e-0, +3.2e-0),
				angle_range=	(-math.pi*.75, +math.pi*.75), # at most (-6*math.pi, +6*math.pi)
				effort=			0,
				dangle=			0,
				pwm_pub=		rospy.Publisher('stm32f3discovery_imu_driver/pwm2', Float64, queue_size = 10),
				angle_pub=		rospy.Publisher('starboard_angle', Float64, queue_size = 10),
			),
		]
		"""

		# See Wrench Callback for deffinition of wrench_tf
		self.wrench_tf = np.matrix([[0.5, -1 / self.L], [0.5, 1 / self.L]])

	# Wait for the dynamixel node to initilze
		if not self.simulate:
			rospy.loginfo('Checking/waiting for dynamixel server')
			while(self.servo_pub.get_num_connections() <= 0 and not rospy.is_shutdown):
				pass
			rospy.loginfo('dynamixel server found')

	# Zero servos
		for x in range(2,4):
			self.servo_pub.publish(DynamixelFullConfig(
				id=						x,
				goal_position= 			math.pi,
				moving_speed=			12, # near maximum, not actually achievable ...
				torque_limit=			1023,
				goal_acceleration=		38,
				control_mode=			DynamixelFullConfig.JOINT,
				goal_velocity=			10,
			))

	# Wait for wrench msgs
		rospy.spin()

#Callback for wrench input
#		
# Equation:
#	Fx is desired force in x direction
#	T is desired torque
#	Fmx is motor force x
#	L is the length between thrusters
#
#	| Fx |   | 1     1 | | Fm1 |
#	| T  | = | L/2  -L/2| | Fm2 |
#
# Logic:
#	Multiply by inverse(wrench_tf) of 2X2 matrix to solve for required motor force
#	Send force to another node to convert thrust to pwm signal for the motor controllers
#
#
	def _wrench_cb(self, msg):
		# Since it is impossible to move in the y direction with this thruster config this direction is completly ignored
		# Torque is equal to the z component of the wrench.torque
		desired = np.matrix([[msg.wrench.force.x], [msg.wrench.torque.z]])

		# Multiple by precalculated wrench
		thrust = self.wrench_tf * desired

		# Check limits
		# TODO: get actual limits
		# Preserve torque but adjust force if outside of limits
		diff = abs(thrust[0, 0] - thrust[1,0])
		max_diff = self.max_thrust - self.min_thrust
		if diff > max_diff:
			# Can't satisfy torque
			rospy.logwarn("Torque request can not be satisfied!")
			if thrust[0,0] > self.max_thrust:
				thrust[0,0] = self.max_thrust
				thrust[1,0] = self.min_thrust

			elif thrust[0,0] < self.min_thrust:
				thrust[0,0] = self.min_thrust
				thrust[1,0] = self.max_thrust

			elif thrust[1,0] > self.max_thrust:
				thrust[1,0] = self.max_thrust
				thrust[0,0] = self.min_thrust

			elif thrust[1,0] < self.min_thrust:
				thrust[1,0] = self.min_thrust
				thrust[0,0] = self.max_thrust

		else:
			if thrust[0,0] > self.max_thrust:
				rospy.logwarn("Port thrust above max, preserving torque and reducing force")
				offset = thrust[0,0] - self.max_thrust
				thrust[0,0] = self.max_thrust
				thrust[1,0] -= offset

			elif thrust[0,0] < self.min_thrust:
				rospy.logwarn("Port thrust below minimum, preserving torque and reducing force")
				offset = self.min_thrust - thrust[0,0] 
				thrust[0,0] = self.min_thrust
				thrust[1,0] += offset

			elif thrust[1,0] > self.max_thrust:
				rospy.logwarn("Starboard thrust above max, preserving torque and reducing force")
				offset = thrust[1,0] - self.max_thrust
				thrust[1,0] = self.max_thrust
				thrust[0,0] -= offset

			elif thrust[1,0] < self.min_thrust:
				rospy.logwarn("Starboard thrust below minimum, preserving torque and reducing force")
				offset = self.min_thrust - thrust[1,0]
				thrust[1,0] = self.min_thrust
				thrust[0,0] += offset

	# Zero servos
		for x in range(2,4):
			self.servo_pub.publish(DynamixelFullConfig(
				id=						x,
				goal_position= 			math.pi,
				moving_speed=			12, # near maximum, not actually achievable ...
				torque_limit=			1023,
				goal_acceleration=		38,
				control_mode=			DynamixelFullConfig.JOINT,
				goal_velocity=			10,
			))

		# Output thrust
		self.thrust_pub.publish(thrusterNewtons(
                id = 3,
                thrust = thrust[0,0]
            ))
		self.thrust_pub.publish(thrusterNewtons(
                id = 2,
                thrust = thrust[1,0]
            ))

if __name__ == '__main__':
	try:
		Node()
	except rospy.ROSInterruptException:
		pass
