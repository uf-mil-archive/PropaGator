#ifndef _POINT_SHOOT_
#define _POINT_SHOOT_

#include "point_shoot.h"

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

/*
 * 		Constructor
 * 	Grabs all params, initializes vars, subs, pubs
 */
PointShoot::PointShoot() :
	current_pose_(), current_twist_(),
	current_linear_error_(0), last_linear_error_(0), diff_linear_error_(0), int_linear_error_(0),
	current_angular_error_(0), last_angular_error_(0), diff_angular_error_(0), int_angular_error_(0),
	is_oriented_to_path_(true), was_oriented_to_path_(true), has_goal_(false),
	last_error_update_time_(0),
	nh_(), private_nh_("~"),
	moveit_(nh_, "moveit", boost::bind(&PointShoot::newGoal_, this, _1), false)		// Causes seg. fault

{
	zero_wrench_.wrench.force.x = zero_wrench_.wrench.force.y = zero_wrench_.wrench.force.z = 0;
	zero_wrench_.wrench.torque.x = zero_wrench_.wrench.torque.y = zero_wrench_.wrench.torque.z = 0;

	// Makes sure that we have the fully defined domain name
	std::string topic = nh_.resolveName("odom");

	// Setup subscribers and publishers
	odom_sub_ = nh_.subscribe<nav_msgs::Odometry>(topic.c_str(), 10, &PointShoot::getCurrentPoseTwist_, this);
	thrust_pub_ = nh_.advertise<geometry_msgs::WrenchStamped>("wrench", 10);

	//pose_theta_ = ::getParam<int>(private_nh, "pose_theta");
	//angle_moving_theta_ = uf_common::getParam<int>(private_nh, "angle_moving_theta");
	//angle_theta_ = uf_common::getParam<int>(private_nh, "angle_theta");

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

	ROS_INFO("Odom received setting initial position and starting action server");
	desired_pose_ = current_pose_;

	// Now that we have our current pose start the action server
	moveit_.registerPreemptCallback(boost::bind(&PointShoot::goalPreempt_, this));
	//moveit_.registerGoalCallback(boost::bind(&PointShoot::newGoal_, this));
		// Works in initializer, but causes seg. fault in initializer... odd
	moveit_.start();

}

/*
 * 		Run
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
 * 		Update
 * 	This function updates the controller
 */
void PointShoot::update_()
{
	geometry_msgs::WrenchStamped msg = zero_wrench_;
	geometry_msgs::Wrench &wr = msg.wrench;

	int norm_curr_pose_ = normToPoseTheta_(current_pose_);
	int norm_curr_angle_moving_ = normToAngleMovingTheta_ (current_twist_.angular);
	int norm_curr_angle_ = normToAngleTheta_ (current_pose_.orientation);

	int norm_goal_angle_ = normToAngleTheta_ (desired_pose_.orientation);

	// If we have a goal go to it
	if(has_goal_){ // if (engaging a goal){

		// We are far away from our goal position
		//	Therefore we need to orient the boat towards the goal position
		if ( norm_curr_pose_ > pose_theta_ ){

			if ( norm_curr_angle_ < angle_moving_theta_ ){ // if angle in motion acceptable, move forward

				wr.force = calculateForce_(); // calculate_forward_force ();

			}else{ // if angle in motion not acceptable, orient to path

				wr.torque = calculateTorque_(); // calculate_turn_torque ()
			}

		// We are very close to the goal so set orientation to goal orientation
		}else if ( norm_curr_angle_ > angle_theta_ ){  // orient to goal

			wr.torque = calculateTorque_(); // caculate_turn_force ()

		}else/*TODO:if(twist yaw is less than some yaw_theta)*/{

			moveit_.setSucceeded(); // goal_complete ()
		}

		// TODO: Add timeout in case boat can't station hold
		//			This is important in case we take remote control or disable the motors in any way
		//			Since as soon as the boat regains control of itself it will try to go to the last waypoint
		//			To prevent this we timeout and set a new goal as our current position
	}else{ // no present goal; station holding

		// Boat has drifted to far in the uncontrolled direction so set the desired position as the new goal
		if (true){ // if ( |current_y_pos - desired_pos| > pose_theta ) // boat is no longer pointing at desired position

			boost::shared_ptr<uf_common::MoveToGoal> restation_goal_;
			// set goal parameters

			newGoal_(restation_goal_); // set_goal ( desired_position )

		} else if ( abs(norm_curr_angle_ - norm_goal_angle_) > angle_theta_ ){ // if ( |current_angle - desired_angle| > angle_theta ){

			wr.torque = calculateTorque_(); // calculate_turn_wrench ()

		// Boat has a good orientation so keep it in place with k * sqrt(error) controller
		}else if ( true ){ // if ( |current_x_pos - desired_pose| > pose_theta )

			wr.force = calculateForce_(); // caclulate_force_wrench ()



		}else{

			// reset_time ()

		}

	}

	thrust_pub_.publish(msg);
}

/*
 * 		getCurrentPoseTwist
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
 * 		updateErrors
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

geometry_msgs::Vector3 PointShoot::calculateForce_(){
	geometry_msgs::Vector3 resulting_force;

	return resulting_force;
}

geometry_msgs::Vector3 PointShoot::calculateTorque_(){
	geometry_msgs::Vector3 resulting_torque;

	return resulting_torque;
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
 * 		accept a new goal
 *	Set the desired position and reset the errors
 */
void PointShoot::newGoal_(const uf_common::MoveToGoal::ConstPtr &goal)
{
	ROS_INFO("Got new goal");

	has_goal_ = true;
	// Drop the last goal and set a new one
	desired_pose_ = goal->posetwist.pose;
	desired_twist_ = goal->posetwist.twist;		// We don't use this for anything yet

	clearErrors_(); // todo: why do we clear the errors at this time
}

/*
 * 		Preempt the goal
 * 	The goal has been canceled so stop and set current position
 * 		as desired position
 */
void PointShoot::goalPreempt_()
{
	ROS_INFO("Goal preempted");

	// TODO: Is there anything else we need to do??? J: probably not
	desired_pose_ = current_pose_;
	clearErrors_();

}

/*
 * 		Normalize Pose for Tolerance Comparison
 * 	Quantify pose for comparison to pose tolerance parameter
 */
int PointShoot::normToPoseTheta_ (geometry_msgs::Pose pose){
	return 0;
}

/*
 * 		Normalize Movement Angle for Tolerance Comparison
 * 	Quantify angular velocity from a Twist for comparison to moving angle tolerance parameter
 */
int PointShoot::normToAngleMovingTheta_ (geometry_msgs::Vector3 move_angle)
{
	return 0;
}

/*
 * 		Normalize Orientation Angle for Tolerance Comparison
 * 	Quantify orientation angle from a Pose for comparison to pose tolerance parameter
 */
int PointShoot::normToAngleTheta_ (geometry_msgs::Quaternion angle) // orientation from Pose
{

	return 0;
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

#endif
