#ifndef _POINT_SHOOT_H_
#define _POINT_SHOOT_H_

#include <ros/ros.h>
#include <ros/rate.h>
#include <actionlib/server/simple_action_server.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/WrenchStamped.h>
#include <tf/LinearMath/Quaternion.h>
#include <tf/LinearMath/Transform.h>

#include <cmath>

#include "uf_common/MoveToAction.h"
//#include <uf_common/param_helpers.h>

#define PI 3.14159265359

// Class for the controller
class PointShoot{
	// Public vars
	public:

		geometry_msgs::WrenchStamped zero_wrench_;


	// Private vars
	private:
		// Tolerance variables
		double pose_theta_;
		double angle_moving_theta_;
		double angle_theta_;

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

		//Action Server
		actionlib::SimpleActionServer<uf_common::MoveToAction> moveit_;

		// ROS node handle
		ros::NodeHandle nh_, private_nh_;

	// Public functions
	public:
		PointShoot();

		void run_();

	// Private functions
	private:
		void update_();

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

		int normToPoseTheta_ (geometry_msgs::Pose pose);

		int normToAngleMovingTheta_ (geometry_msgs::Vector3 move_angle); // angular from Twist

		int normToAngleTheta_ (geometry_msgs::Quaternion angle); // orientation from Pose

};

#endif
