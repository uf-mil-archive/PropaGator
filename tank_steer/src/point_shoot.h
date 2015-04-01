#ifndef _POINT_SHOOT_H_
#define _POINT_SHOOT_H_

#include <ros/ros.h>
#include <ros/rate.h>
#include <ros/timer.h>
#include <actionlib/server/simple_action_server.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/WrenchStamped.h>
#include <tf/LinearMath/Quaternion.h>
#include <tf/LinearMath/Transform.h>

#include <cmath>

#include "uf_common/MoveToAction.h"
//#include <uf_common/param_helpers.h>
#include "dynamixel_servo/DynamixelFullConfig.h"

#define PI 3.14159265359

// Class for the controller
class PointShoot{
	// Public vars
	public:


	// Private vars
	private:
		// Tolerance variables
		double distance_tol_;
		double angle_to_path_tol_;
		double angle_to_goal_tol_;
		double angle_vel_tol_;
		double linear_vel_tol_;

		// Position and orientation
		geometry_msgs::Pose current_pose_;
		geometry_msgs::Pose desired_pose_;

		// Velocity and angular velocity
		geometry_msgs::Twist current_twist_;
		geometry_msgs::Twist desired_twist_;

		bool has_goal_;

		// Linear error = strait line distance between current_xyz_ and desired_xyz_
		double last_linear_error_;
		double current_linear_error_;
		double diff_linear_error_;
		double int_linear_error_;

		// Angular error = desired orientation - current_orientation
		double last_angular_error_;
		double current_angular_error_;
		double diff_angular_error_;
		double int_angular_error_;

		// Error timing vars
		ros::Time last_error_update_time_;

		// What to orient to
		//		true for path false for desired orientation
		bool is_oriented_to_path_;
		// What was it oriented to last
		//		used to clear the integral error on change of state
		bool was_oriented_to_path_;

		// Odometry subscriber
		ros::Subscriber odom_sub_;

		// Thruster Mapper publisher
		ros::Publisher thrust_pub_;

		// Trajectory pub
		ros::Publisher trajectory_pub_;

		// Servo publisher
		ros::Publisher servo_pub_;

		// Replacement update timer for goal callback
		ros::Timer update_timer_;

		//Action Server
		actionlib::SimpleActionServer<uf_common::MoveToAction> moveto_;

		// ROS node handle
		ros::NodeHandle nh_, private_nh_;

		// Update frequency
		ros::Duration update_freq_;

		// Zero wrench and twist
		geometry_msgs::WrenchStamped zero_wrench_;
		geometry_msgs::Twist zero_twist_;

		// Controller constants
		double linear_gain_;
		double angle_int_gain_;
		double angle_diff_gain_;
		double angle_current_gain_;

	// Public functions
	public:
		PointShoot();

	// Private functions
	private:
		void update_(const ros::TimerEvent&);

		void getCurrentPoseTwist_(const nav_msgs::Odometry::ConstPtr &msg);

		void updateErrors_();

		geometry_msgs::Vector3 calculateForce_();

		geometry_msgs::Vector3 calculateTorque_();

		void clearErrors_();

		double get2DDistance_(geometry_msgs::Point p1, geometry_msgs::Point p2);

		double get2DAngle_(geometry_msgs::Quaternion q1, geometry_msgs::Quaternion q2);

		double getYaw_(geometry_msgs::Quaternion q);

		double scaleRads_(double rads);

		void newGoal_(const uf_common::MoveToGoal::ConstPtr &goal);

		void goalPreempt_();
};

#endif
