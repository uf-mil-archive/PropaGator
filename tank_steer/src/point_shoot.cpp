#include <ros/ros.h>
#include <ros/rate.h>
#include <actionlib/server/simple_action_server.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/WrenchStamped.h>
#include <tf/LinearMath/Quaternion.h>
#include <tf/LinearMath/Transform.h>

#include <cmath>

#include "uf_common/MoveToAction.h"

/****************************************************************************
 * 							Point and shoot for tank steer					*
 * 	This node implements the moveit simple action server					*
 * 		It takes a desired pose from the moveit action and generates the 	*
 * 		required torque and force to achieve the desired goal				*
 * 	As the name suggest the robot will move by first pointing towards its 	*
 * 		endpoint then moving towards it linearly once reaching its goal xyz	*
 * 		it will stop and rotate to the desired orientation					*
 * 	When there is no goal to achieve the node will station hold				*
 * 	Since the robot can only control linear motion in one direction at a 	*
 * 		time, the robot will station hold by mantaining only the 			*
 * 		controllable direction and orientation until the robot drifts out 	*
 * 		of the allowable orthogonal direction, at this point the robot will	*
 * 		send itself a new goal to return to the previously specified 		*
 * 		waypoint															*
 * 	When moving in the linear direction a K * sqrt(error) controller will 	*
 * 		be used, as it will move the robot towards the goal at max speed 	*
 * 		and then ease off as it approaches (more details are disscused in 	*
 * 		implementation)														*
 ****************************************************************************/

#define PI 3.14159265359

// Class for the controller
// TODO: move to separate header and cpp

class PointShoot
{
	// Public vars
public:

	// Private vars
private:
	// Position and orientation
	geometry_msgs::Pose current_pose_;
	geometry_msgs::Pose desired_pose_;

	// Velocity and angular velocity
	geometry_msgs::Twist current_twist_;
	geometry_msgs::Twist desired_twist_;

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
	ros::NodeHandle nh_;

	// Public functions
public:
	void run_();

	PointShoot();

	// Private functions
private:
	void update_();

	void getCurrentPoseTwist_(const nav_msgs::Odometry::ConstPtr &msg);

	geometry_msgs::WrenchStamped calculateForce_();

	geometry_msgs::WrenchStamped calculateTorque_();

	void updateErrors_();

	void clearErrors_();

	double get2DDistance_(geometry_msgs::Point p1, geometry_msgs::Point p2);

	double get2DAngle_(geometry_msgs::Quaternion q1, geometry_msgs::Quaternion q2);

	double getYaw_(geometry_msgs::Quaternion q);

	double scaleRads_(double rads);

	void newGoal_(const uf_common::MoveToGoal::ConstPtr &goal);

	void goalPreempt_();

};

/*
 * 			Constructor
 * 	Grabs all params, initializes vars, subs, pubs
 */
PointShoot::PointShoot() :
	current_pose_(), current_twist_(),
	current_linear_error_(0), last_linear_error_(0), diff_linear_error_(0), int_linear_error_(0),
	current_angular_error_(0), last_angular_error_(0), diff_angular_error_(0), int_angular_error_(0),
	is_oriented_to_path_(true), was_oriented_to_path_(true),
	last_error_update_time_(0),
	moveit_(nh_, "moveit", boost::bind(&PointShoot::newGoal_, this, _1), false)
{
	std::string topic = nh_.resolveName("odom");
	// Setup subscribers and publishers
	odom_sub_ = nh_.subscribe<nav_msgs::Odometry>(topic.c_str(), 10, &PointShoot::getCurrentPoseTwist_, this); // what Kevin wrote

	thrust_pub_ = nh_.advertise<geometry_msgs::WrenchStamped>("wrench", 10);

	// Make sure we have odometry before moving on
	// TODO: add timeout
	ROS_INFO("Waiting for odom publisher");
	while(odom_sub_.getNumPublishers() <= 0 && ros::ok());

	// Wait till the first position is found then set that to desired position
	ROS_INFO("Waiting for first odom msg");
	while(current_pose_.position.x == 0)		// The likelihood of x actually being zero is very small
	{
		ros::spinOnce();
	}
	desired_pose_ = current_pose_;

	// Now that we have our current pose start the action server
	moveit_.registerPreemptCallback(boost::bind(&PointShoot::goalPreempt_, this));
	moveit_.start();

}

/*
 * 			Run
 * 	This function handles the main ros loop
 * 		such as sleep rate and ros::ok()
 */
void PointShoot::run_()
{
	while(ros::ok())
	{
		update_();
		ros::spinOnce();
	}
}

/*
 * 			Update
 * 	This function updates the controller
 */
void PointShoot::update_()
{
	/*
	geometry_msgs::Wrench msg;
	msg.force = localPlanner->getForce();
	msg.torque = localPlanner->getTorque();
	thrust_pub_.publish(msg);
	*/
}

/*
 * 			getCurrentPoseTwist
 * 	This function listens to odometry and updates the current pose,
 * 		twist and calls the update errors function
 */
void PointShoot::getCurrentPoseTwist_(const nav_msgs::Odometry::ConstPtr &msg)
{
	current_pose_ = msg->pose.pose;
	current_twist_ = msg->twist.twist;

	updateErrors_();
}

/*
 * 			updateErrors
 * 	This function keeps a running tab on the error, derivative of error,
 * 		and integral of error
 */
void PointShoot::updateErrors_()
{
	// Update time
	ros::Duration time_step = ros::Time::now() - last_error_update_time_;
	last_error_update_time_ = ros::Time::now();

	// Calculate path orientation
	double angle_to_path = atan2(desired_pose_.position.y - current_pose_.position.y,
								 desired_pose_.position.x - current_pose_.position.x);

	// Get current boat orientation
	double boat_orientation = getYaw_(current_pose_.orientation);

	// Find angular error to path
	double angular_error_to_path = scaleRads_(angle_to_path - boat_orientation);

	// Move current to last
	last_linear_error_ = current_linear_error_;
	last_angular_error_ = current_angular_error_;

	// Get new errors
	current_linear_error_ = get2DDistance_(current_pose_.position, desired_pose_.position);

	// Set the sign of the current_linear_error_ based on the orientation of the boat
	if(abs(angular_error_to_path) > (PI / 2))
	{
		current_linear_error_ *= -1;		// The boat overshot so we need to reverse
	}

	// Debug
	//ROS_INFO("Angle between x1:%f, y1:%f, z1:%f, w1:%f, and x1:%f, y1:%f, z1:%f, w1:%f, is: \n\t%f",
	//		desired_pose_.orientation.x, desired_pose_.orientation.y, desired_pose_.orientation.z, desired_pose_.orientation.w,
	//		current_pose_.orientation.x, current_pose_.orientation.y, current_pose_.orientation.z, current_pose_.orientation.w,
	//		get2DAngle(desired_pose_.orientation, current_pose_.orientation));

	// Based on what stage of travel the system is in defines the
	//		angular error
	if(is_oriented_to_path_)
	{
		current_angular_error_ = angular_error_to_path;
	}
	else	// Keep orientation to desired goal
	{
		current_angular_error_ = scaleRads_(getYaw_(desired_pose_.orientation) - getYaw_(current_pose_.orientation));
	}

	// Calculate derivative and integral errors
	//		Derivitive = (new - last) / time_step
	//		Integral += (last + new) * time_step / 2		(trapizodal integration)
	diff_linear_error_ = (current_linear_error_ - last_linear_error_) / time_step.toSec();
	diff_angular_error_= (current_angular_error_ - last_angular_error_) / time_step.toSec();

	int_linear_error_ += (current_linear_error_ + last_linear_error_) * time_step.toSec() / 2;
	int_angular_error_ += (current_angular_error_ + last_angular_error_) * time_step.toSec() / 2;
}

/*
 * 		get2DDistance
 * 	Find the distance in the xy plane
 */
double PointShoot::get2DDistance_(geometry_msgs::Point p1, geometry_msgs::Point p2)
{
	return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

/*
 *		get2DAngle
 *	Finds the difference in yaw of two quaternions
 */
double PointShoot::get2DAngle_(geometry_msgs::Quaternion q1, geometry_msgs::Quaternion q2)
{
	double yaw1 = getYaw_(q1);
	double yaw2 = getYaw_(q2);

	//Debug
	//ROS_INFO("yaw1: %f, yaw2: %f", yaw1, yaw2);

	// always choose the small angle
	double delta = scaleRads_(yaw2 - yaw1);

	// Return the angle between the two
	return delta;
}
/*
 * 		getYaw
 * 	Function to get yaw from geometry_msgs::Quaternion
 */
double PointShoot::getYaw_(geometry_msgs::Quaternion q)
{
	tf::Matrix3x3 q_tf(tf::Quaternion(q.x, q.y, q.z, q.w));
	double dummy, yaw;
	q_tf.getRPY(dummy, dummy, yaw);
	return yaw;
}

/*
 *		scaleRads
 *	Make sure that a rad is between -PI and PI
 */
double PointShoot::scaleRads_(double rads)
{
	if(rads > PI)
	{
		rads -= PI;
	}
	else if(rads < -PI)
	{
		rads += PI;
	}

	return rads;
}

/*
 * 			accept a new goal
 *	Set the desired position and reset the errors
 */
void PointShoot::newGoal_(const uf_common::MoveToGoal::ConstPtr &goal)
{
	// Drop the last goal and set a new one
	desired_pose_ = goal->posetwist.pose;
	desired_twist_ = goal->posetwist.twist;		// We don't use this for anything yet
}

/*
 * 			Preempt the goal
 * 	The goal has been canceled so stop and set current position
 * 		as desired position
 */
void PointShoot::goalPreempt_()
{
	// TODO: Is there anything else we need to do???
	desired_pose_ = current_pose_;
	clearErrors_();

}

void PointShoot::clearErrors_()
{
	// Linear error = strait line distance between current_xyz_ and desired_xyz_
	 last_linear_error_ = 0;
	current_linear_error_ = 0;
	diff_linear_error_ = 0;
	int_linear_error_ = 0;

	// Angular error = desired orientation - current_orientation
	last_angular_error_ = 0;
	current_angular_error_ = 0;
	diff_angular_error_ = 0;
	int_angular_error_ = 0;

	// Error timing vars
	last_error_update_time_ = ros::Time::now();
}

/************************
 * 			Main		*
 ************************/
int main(int argc, char** argv)
{
	//Initialize ROS
	ros::init(argc, argv, "point_shoot");

	// Initialize controller
	PointShoot ps;

	// Run the node
	ps.run_();

	return 0;
}
