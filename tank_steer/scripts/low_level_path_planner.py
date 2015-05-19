#! /usr/bin/env python

## 			Low Level Path Planner
## This node plans a strait line path and generates wrenches to keep the boat on the path

import rospy

import actionlib
from uf_common.msg import PoseTwistStamped, MoveToAction
from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Pose
from geometry_msgs.msg import Twist

class low_level_path_planner:
	def __init__(self, name):
		# Desired pose twist
		self.desired_pose = self.current_pose = Pose()
		self.desired_twist = self.current_twist = Twist()

		# Set current Pose to 0
		self.current_pose.position.x = self.current_pose.position.y = self.current_pose.position.z = 0
		self.current_pose.orientation.x = self.current_pose.orientation.y = self.current_pose.orientation.z = 0

		# Set desired twist to 0
		self.desired_twist.linear.x = self.desired_twist.linear.y = self.desired_twist.linear.z = 0
		self.desired_twist.angular.x = self.desired_twist.angular.y = self.desired_twist.angular.z = 0

		# Wait for current position and set as desired position
		rospy.loginfo('Waiting for /odom')
		self.odom_sub = rospy.Subscriber('/odom', Odometry, self.odom_cb, queue_size = 10)
		while self.current_pose.position.x == 0:	# The x will be 0 until an odom pulishes (if its still 0 it will drift very very soon)
			pass
		self.desired_pose = self.current_pose
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

		# Implement the moveto action server
		self.moveto_as = actionlib.SimpleActionServer('moveto', MoveToAction, self.new_goal, False)
		self.moveto_as.start()

	def new_goal(self, goal):
		self.moveto_as.set_succeeded()

	def set_kill(self):
		rospy.logwarn('Tank steer low level path planner killed because: %s' % self.kill_listener.get_kills())

	def clear_kill(self):
		rospy.loginfo('Tank steer low level path planner unkilled')

	def on_shutdown(self):
		self.kill_broadcaster.send(True)

	# Update pose and twist
	def odom_cb(self, msg):
		self.current_pose = msg.pose.pose
		self.current_twist = msg.twist.twist


if __name__ == '__main__':
	rospy.init_node('low_level_path_planner')
	node = low_level_path_planner(rospy.get_name())
	rospy.spin()