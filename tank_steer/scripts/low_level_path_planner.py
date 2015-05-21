#! /usr/bin/env python

## 			Low Level Path Planner
## This node plans a strait line path to the goal and publishes it on the trajectory topic

import rospy

import actionlib
from uf_common.msg import PoseTwistStamped, MoveToAction
from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster
from nav_msgs.msg import Odometry
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
		self.linear_tolerance = 1
		self.angular_tolerance = np.pi / 10
		self.line = line(np.zeros(3), np.zeros(3))

		# Trajectory
		self.max_traj_step = 1

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
		self.desired_position = self.current_position
		self.desired_orientation = self.current_orientation
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
		
		self.line = line(self.current_position, self.desired_position)

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

	# Check if current pose is in range of desired pose
	def in_range(self):
		# TODO: orientation considerations
		dis = np.linalg.norm((self.desired_position - self.current_position))
		#rospy.loginfo('dis: ' + str(dis))
		if dis < self.linear_tolerance:
			return True
		else:
			return False


	# Update loop
	def update(self, event):
		# Project current position onto trajectory line
		proj = self.line.proj_pt(self.current_position)
		rospy.loginfo('Projection: ' + str(proj))
		# Add a step along the traj line
		carrot = proj + self.max_traj_step * self.line.norm
		rospy.loginfo('Carrot: ' + str(carrot))

		# Publish trajectory
		# TODO: Actually publish trajectory
		traj = PoseTwistStamped()
		traj.posetwist.pose.position = vector3_from_xyz_array(carrot)
		traj.posetwist.pose.orientation = quaternion_from_rotvec([0, 0, self.line.angle])
		self.traj_pub.publish(traj)

		# Check if goal is reached
		if self.moveto_as.is_active():
			if self.in_range():
				self.moveto_as.set_succeeded(None)



	# Get the distance between two xyz points or Poses
	# 
	#def dis(p1, p2):
	#	if hasattr(p1, )



if __name__ == '__main__':
	rospy.init_node('low_level_path_planner')
	node = low_level_path_planner(rospy.get_name())
	rospy.spin()