#! /usr/bin/env python

import rospy
from uf_common.msg import PoseTwistStamped
from uf_common.orientation_helpers import xyz_array
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Vector3Stamped, Vector3
from geometry_msgs.msg import WrenchStamped, Wrench
from std_msgs.msg import Header
import tools
from tools import line
import numpy as np
import math
import tf


class wrench_generator:
	def __init__(self, name):
		# Current pose
		self.carrot_position = self.current_position = np.zeros(3)
		self.carrot_position = self.current_orientation = np.zeros(3)
		self.current_velocity = np.zeros(3)
		self.last_perp_velocity = 0

		# Grab gains
		self.p = rospy.get_param('p', 10)
		self.i = rospy.get_param('i', 5)
		self.d = rospy.get_param('d', 0)

		# Controller vars
		self.last_time = rospy.get_rostime()

		# Tf stuff
		self.tf_listener = tf.TransformListener()
		#self.tf_listener.waitForTransform('/enu', '/base_link', rospy.Time(0), rospy.Time(10000))

		# Set up publishers
		self.wrench_pub = rospy.Publisher('wrench', WrenchStamped, queue_size = 10)

		# Wait for current position and set as desired position
		rospy.loginfo('Waiting for /odom')
		self.odom_sub = rospy.Subscriber('/odom', Odometry, self.odom_cb, queue_size = 10)
		while not self.current_position.any():	# Will be 0 until an odom publishes (if its still 0 it will drift very very soon)
			pass

		# Start traj subscriber
		self.traj_sub = rospy.Subscriber('/trajectory', PoseTwistStamped, self.traj_cb, queue_size = 10)

	# Calculate error
	def traj_cb(self, traj):
		# Timing
		now = rospy.get_rostime()
		dt = (now - self.last_time).to_sec()
		self.last_time = now

		# Get vectors related to the orientation of the trajectory
		o = tools.normal_vector_from_posetwist(traj.posetwist)
		#rospy.loginfo('o : ' + str(o))
		o_hat = o / np.linalg.norm(o)
		#rospy.loginfo('o hat: ' + str(o_hat))
		o_norm = np.cross([0, 0, 1], o_hat)
		#rospy.loginfo('o norm: ' + str(o_norm))

		# Overshoot = 1 if the boat is behind a line drawn perpendicular to the trajectory orientation
		#		and through the trajectories position, it is -1 if it is on the other side of the before mentioned line
		traj_position = tools.position_from_posetwist(traj.posetwist)
		boat_proj = line(traj_position, traj_position + o_hat).proj_pt(self.current_position)
		#rospy.loginfo('Boat_proj: ' + str(boat_proj))
		#rospy.loginfo('traj: ' + str(traj_position))
		#rospy.loginfo('traj - Boat_proj: ' + str(traj_position - boat_proj))
		boat_to_traj = traj_position - self.current_position
		boat_to_traj_hat = boat_to_traj / np.linalg.norm(boat_to_traj)
		boat_to_traj_norm = np.cross([0,0,1], boat_to_traj_hat)
		overshoot = np.dot(boat_to_traj, o_hat)
		overshoot = overshoot / abs(overshoot)
		rospy.loginfo('overshoot: ' + str(overshoot))
		if math.isnan(overshoot):
			return

		# Method 2:
		#
		##		P error = - angle(between boat and trajectory)
		#
		enu_angle_to_traj = np.arccos(np.dot(boat_to_traj / np.linalg.norm(boat_to_traj), np.array([1, 0, 0])))
		enu_angle_to_traj = math.copysign(enu_angle_to_traj, np.dot(boat_to_traj_norm, self.current_position))
		boat_angle_to_traj = (self.current_orientation[2] - enu_angle_to_traj) % (np.pi)

		boat_orientation_normal = tools.normal_vector_from_rotvec(self.current_orientation)
		angle_error = np.arccos(np.dot(boat_orientation_normal, boat_to_traj_hat)) * np.dot(boat_to_traj_norm, boat_orientation_normal) * -1 * overshoot
		#rospy.loginfo('enu_angle_to_traj:\t' + str(enu_angle_to_traj * 180 / np.pi))
		#rospy.loginfo('Boat orientation:\t' + str(self.current_orientation[2] * 180 / np.pi))
		#rospy.loginfo('Boat angle to traj\t' + str((boat_angle_to_traj) * 180 / np.pi ))
		rospy.loginfo('Angle error: ' + str(angle_error * 180 / np.pi))
		torque = angle_error * self.p

		""" Method 1:
		#
		##		P error = - perpendicular_velocity * p_gain * overshoot
		#
		# Velocity error = velocity perpendicular to trajectory orientation
		velocity_error = self.current_velocity - (np.dot(self.current_velocity, o_hat) * o_hat)
		v_dir = np.dot(velocity_error, o_norm)
		velocity_error = math.copysign(np.linalg.norm(velocity_error), v_dir)
		rospy.loginfo('V_perp = ' + str(velocity_error))
		velocity_error = -1 * velocity_error * self.p * overshoot
		rospy.loginfo('P error = ' + str(velocity_error))

		#
		##		I error = - position * i_gain * overshoot * ???
		#
		# Positional error
		#				boat_position		parralel portion of boat position
		pos_error = self.current_position - boat_proj
		p_dir = np.dot(pos_error, o_norm)
		pos_error = math.copysign(np.linalg.norm(pos_error), p_dir)
		rospy.loginfo('Perp_position = ' + str(pos_error))
		pos_error = -1 * pos_error * self.i * overshoot
		rospy.loginfo('I error = ' + str(pos_error))

		#
		##		D error
		#
		# Acceleration error
		acceleration_error = self.d * (velocity_error - self.last_perp_velocity) / dt
		self.last_perp_velocity = velocity_error

		torque = velocity_error + pos_error + acceleration_error

		"""

		# Reverse force if overshoot
		force = traj.posetwist.twist.linear.x * overshoot
		rospy.loginfo('torque: ' + str(torque))

		# Output a wrench!
		self.wrench_pub.publish(
			WrenchStamped(
				header = Header(
					frame_id = '/base_link',
					stamp = now),
				wrench = Wrench(
					force = Vector3(force, 0, 0),
					torque = Vector3(0, 0, torque))))


	# Update pose and twist
	def odom_cb(self, msg):
		# Pose is relative to /enu!!!!!!!!
		# Twist is relative to /base_link!!!!!!!!!
		self.current_position = tools.position_from_pose(msg.pose.pose)
		self.current_orientation = tools.orientation_from_pose(msg.pose.pose)

		# Transform velocity from boat frame of reference to enu
		vec = Vector3Stamped(
				header = Header(
					frame_id = '/base_link',
					stamp = msg.header.stamp + rospy.Duration(-0.1)),		# Look a tenth of a second into the past
				vector = msg.twist.twist.linear)
		try:
			#rospy.loginfo('Vec before transform: ' + str(vec))
			vec = self.tf_listener.transformVector3('/enu', vec)
			#rospy.loginfo('Vec after transform: ' + str(vec))
		except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException) as err:
			rospy.logwarn('Tf error: ' + str(err))
			return

		self.current_velocity = xyz_array(vec.vector)

if __name__ == '__main__':
	rospy.init_node('wrench_generator')
	node = wrench_generator(rospy.get_name())
	rospy.spin()