#! /usr/bin/env python

## 			Low Level Path Planner
## This node plans a strait line path to the goal and publishes it on the trajectory topic

import rospy

import actionlib
from uf_common.msg import PoseTwistStamped, MoveToAction, PoseTwist
from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster
from nav_msgs.msg import Odometry
from std_msgs.msg import Header
from geometry_msgs.msg import Pose, Twist
from geometry_msgs.msg import Quaternion, Vector3
from uf_common.orientation_helpers import xyz_array, xyzw_array, quat_to_rotvec
from uf_common.orientation_helpers import rotvec_to_quat
import numpy as np

# Conversion functions
def position_from_posetwiststamped(pts):
	return position_from_posetwist(pts.posetwist)

def position_from_posetwist(pt):
	return position_from_pose(pt.pose)

def position_from_pose(p):
	return xyz_array(p.position)

def orientation_from_posetwiststamped(pts):
	return orientation_from_posetwist(pts.posetwist)

def orientation_from_posetwist(pt):
	return orientation_from_pose(pt.pose)

def orientation_from_pose(p):
	return quat_to_rotvec(xyzw_array(p.orientation))

def vector3_from_xyz_array(xyz):
	return Vector3(xyz[0], xyz[1], xyz[2])

def quaternion_from_xyzw_array(xyzw):
	return Quaternion(xyzw[0], xyzw[1], xyzw[2], xyzw[3])

def quaternion_from_rotvec(rot):
	return quaternion_from_xyzw_array(rotvec_to_quat(rot))

# Converts rotation vector to [x, y, z] to unit vector in the pointed to direction
#	Since we only care about orientation in the x y plane we ignore the x y components of the rotation vector
def normal_vector_from_rotvec(rot):
	theta = rot[2] - np.pi		# Shift the angle pi degrees since pi is considered strait forward
	return np.array([np.cos(theta), np.sin(theta), 0])

class line:
	def __init__(self, p1, p2):
		self.p1 = p1
		self.p2 = p2
		self.s = p2 - p1	# Shifted vector
		if self.s.any():
			self.norm = self.s / np.linalg.norm(self.s)
			x_hat = np.zeros_like(self.norm)
			x_hat[0] = 1
			self.angle = np.arccos(np.dot(x_hat, self.norm))
		else:
			rospy.logerr('0 length line in tank steer trajectory generator')
			self.s = np.array([1, 0, 0])
			self.norm = np.array([1, 0, 0])
			self.angle = 0

	# Projects a point onto the line
	# Returns a vector to the projection
	def proj_pt(self, pt):
		# Shift everything p1
		pt = pt - self.p1
		# Project in the shifted cords then shift back
		return ( np.dot(pt, self.s) / np.dot(self.s, self.s) ) * self.s + self.p1


class low_level_path_planner:
	def __init__(self, name):
		# Desired pose
		self.desired_position = self.current_position = np.zeros(3)
		self.desired_orientation = self.orientation = np.zeros(3)
		#self.desired_twist = self.current_twist = Twist()

		# Goal tolerances before seting succeded
		self.linear_tolerance = rospy.get_param('linear_tolerance', 0.25)
		self.angular_tolerance = rospy.get_param('angular_tolerance', np.pi / 10)
		self.orientation_radius = rospy.get_param('orientation_radius', 0.5)
		self.slow_down_radius = rospy.get_param('slow_down_radius', 5)

		# Speed parameters
		self.max_tracking_distance = rospy.get_param('max_tracking_distance', 5)
		self.min_tracking_distance = rospy.get_param('min_tracking_distance', 0.5)
		self.tracking_to_speed_conv = rospy.get_param('tracking_to_speed_conv', 10)
		self.tracking_slope = (self.max_tracking_distance - self.min_tracking_distance) / (self.slow_down_radius - self.orientation_radius)
		self.tracking_intercept = self.tracking_slope * self.orientation_radius + self.min_tracking_distance

		# Publishers
		self.traj_pub = rospy.Publisher('/trajectory', PoseTwistStamped, queue_size = 10)

		# Set current Pose to 0
		self.current_position = np.zeros(3)
		self.current_orientation = np.zeros(3)

		# Set desired twist to 0
		#self.desired_twist.linear.x = self.desired_twist.linear.y = self.desired_twist.linear.z = 0
		#self.desired_twist.angular.x = self.desired_twist.angular.y = self.desired_twist.angular.z = 0

		# Wait for current position and set as desired position
		rospy.loginfo('Waiting for /odom')
		self.odom_sub = rospy.Subscriber('/odom', Odometry, self.odom_cb, queue_size = 10)
		while not self.current_position.any():	# Will be 0 until an odom publishes (if its still 0 it will drift very very soon)
			pass

		# Initlize Trajectory generator with current position as goal
		# 	Set the line to be along our current orientation
		self.desired_position = self.current_position
		self.desired_orientation = self.current_orientation
		# 	Make a line along the orientation
		self.line = line(self.current_position, normal_vector_from_rotvec(self.current_orientation) + self.current_position)
		self.redraw_line = False
		rospy.loginfo('Got current pose from /odom')

		# Kill handling
		self.killed = False
		self.kill_listener = KillListener(self.set_kill, self.clear_kill)
		self.kill_broadcaster = KillBroadcaster(id=name, description='Tank steer low level path planner shutdown')
		try:
			self.kill_broadcaster.clear()			# In case previously killed
		except rospy.service.ServiceException, e:
			rospy.logwarn(str(e))
		rospy.on_shutdown(self.on_shutdown)

		# RC handling

		# Start the main update loop
		rospy.Timer(rospy.Duration(0.1), self.update)

		# Implement the moveto action server
		self.moveto_as = actionlib.SimpleActionServer('moveto', MoveToAction, None, False)
		self.moveto_as.register_goal_callback(self.new_goal)
		self.moveto_as.register_preempt_callback(self.goal_preempt)
		self.moveto_as.start()

	def new_goal(self):
		goal = self.moveto_as.accept_new_goal()
		self.desired_position = position_from_posetwist(goal.posetwist)
		self.desired_orientation = orientation_from_posetwist(goal.posetwist)
		#self.linear_tolerance = goal.linear_tolerance
		#self.angular_tolerance = goal.angular_tolerance

		rospy.loginfo('Desired position:' + str(self.desired_position))
		rospy.loginfo('Current position:' + str(self.current_position))
		rospy.loginfo('Desired orientation:' + str(self.desired_orientation))
		rospy.loginfo('Current orientation:' + str(self.current_orientation))
		rospy.loginfo('linear_tolerance:' + str(self.linear_tolerance))
		rospy.loginfo('angular_tolerance:' + str(self.angular_tolerance))
		# This controller doesn't handle desired twist
		#self.desired_twist = goal.posetwist.twist
		if (xyz_array(goal.posetwist.twist.linear).any() or 
			xyz_array(goal.posetwist.twist.angular).any() ):
			rospy.logwarn('None zero are not handled by the tank steer trajectory generator. Setting twist to 0')
		
		if np.linalg.norm(self.current_position - self.desired_position) > self.orientation_radius:
			self.line = line(self.current_position, self.desired_position)
			self.redraw_line = True
		else:
			self.line = line(self.current_position, normal_vector_from_rotvec(self.desired_orientation) + self.current_position)
			self.redraw_line = False

	def goal_preempt(self):
		self.desired_position = self.current_position
		self.desired_orientation = self.current_orientation
		self.moveto_as.set_preempted()
		rospy.loginfo('Goal preempted')
		

	def set_kill(self):
		rospy.logwarn('Tank steer low level path planner killed because: %s' % self.kill_listener.get_kills())

	def clear_kill(self):
		rospy.loginfo('Tank steer low level path planner unkilled')

	def on_shutdown(self):
		self.kill_broadcaster.send(True)

	# Update pose and twist
	def odom_cb(self, msg):
		self.current_position = position_from_pose(msg.pose.pose)
		self.current_orientation = orientation_from_pose(msg.pose.pose)
		# Get distance to the goal
		self.distance_to_goal = np.linalg.norm((self.desired_position - self.current_position))
		self.angle_to_goal = abs((self.desired_orientation % (2 * np.pi)) - (self.current_orientation % (2 * np.pi)))

	# Get the speed setting of the trajectory
	#				Value					:	Condition
	#				max 					:	d > slow_down
	#				min 					:	d < orientation
	#	tracking_slope * d + intercept 		: orientation < d< slow_down
	def get_tracking_distance(self):
		if self.distance_to_goal < self.orientation_radius:
			return self.min_tracking_distance
		elif self.distance_to_goal > self.slow_down_radius:
			return self.max_tracking_distance
		else:
			return self.tracking_slope * self.distance_to_goal + self.tracking_intercept

	# Get a PoseTwistStamped that represents the desired point along the trajectory that we need
	def get_carrot(self):
		# Project current position onto trajectory line
		Bproj = self.line.proj_pt(self.current_position)
		#rospy.loginfo('Projection: ' + str(Bproj))

		# Default carrot to desired position
		c_pos = Bproj + self.line.norm * self.get_tracking_distance()
		tracking_step = self.get_tracking_distance()

		if self.distance_to_goal < self.orientation_radius:
			c_pos = self.desired_position

		# Fill up PoseTwistStamped
		carrot = PoseTwistStamped(
			header = Header(
				stamp = rospy.get_rostime(),
				frame_id = '/base_link'
			),
			posetwist = PoseTwist(
				pose = Pose(
					position = vector3_from_xyz_array(c_pos),
					orientation = quaternion_from_rotvec([0, 0, self.line.angle])),
				twist = Twist(
					linear = Vector3(tracking_step * self.tracking_to_speed_conv, 0, 0),
					angular = Vector3())
				)
			)
		return carrot

	# Update loop
	def update(self, event):
		
		# Publish trajectory
		traj = self.get_carrot()
		#rospy.loginfo('Trajectory: ' + str(traj))
		self.traj_pub.publish(traj)

		if self.redraw_line and self.distance_to_goal < self.orientation_radius:
			self.redraw_line = False
			rospy.loginfo('Redrawing trajectory line')
			self.line = line(self.current_position, normal_vector_from_rotvec(self.desired_orientation) + self.current_position)

		rospy.loginfo('Angle to goal: ' + str(self.angle_to_goal[2] * 180 / np.pi) + '\t\t\tDistance to goal: ' + str(self.distance_to_goal))

		# Check if goal is reached
		if self.moveto_as.is_active():
			if self.distance_to_goal < self.linear_tolerance:
				if self.angle_to_goal[2] < self.angular_tolerance:
					rospy.loginfo('succeded')
					self.moveto_as.set_succeeded(None)


if __name__ == '__main__':
	rospy.init_node('low_level_path_planner')
	node = low_level_path_planner(rospy.get_name())
	rospy.spin()