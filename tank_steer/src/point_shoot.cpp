#ifndef _POINT_SHOOT_
#define _POINT_SHOOT_

#include "point_shoot.h"

// TODO: James K sqrt(error)
// TODO: Kevin PID controller
// TODO: Debug

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
	moveto_("moveto", false),
	update_freq_(0.01),
	distance_tol_(0.5),
	angle_to_path_tol_(10),
	angle_to_goal_tol_(15),
	angle_vel_tol_(0.3),
	linear_vel_tol_(0.1)
{
	//moveto_(nh_, "moveit", boost::bind(&PointShoot::newGoal_, this, _1), false)		// Causes seg. fault
	// TODO: figure out how to put in initilizer
	zero_wrench_.wrench.force.x = zero_wrench_.wrench.force.y = zero_wrench_.wrench.force.z = 0;
	zero_wrench_.wrench.torque.x = zero_wrench_.wrench.torque.y = zero_wrench_.wrench.torque.z = 0;
	zero_twist_.linear.x = zero_twist_.linear.y = zero_twist_.linear.z = 0;
	zero_twist_.angular.x = zero_twist_.angular.y = zero_twist_.angular.z = 0;

	// Makes sure that we have the fully defined domain name
	std::string topic = nh_.resolveName("odom");

	// Setup subscribers and publishers
	odom_sub_ = nh_.subscribe<nav_msgs::Odometry>(topic.c_str(), 10, &PointShoot::getCurrentPoseTwist_, this);
	thrust_pub_ = nh_.advertise<geometry_msgs::WrenchStamped>("wrench", 10);
	//trajectory_pub_ = nh_.advertise<PoseTwistStamped>("trajectory", 1);
	servo_pub_ = nh_.advertise<dynamixel_servo::DynamixelFullConfig>("dynamixel/dynamixel_full_config", 10);

	// ROS params
	nh_.param<double>("linear_gain", linear_gain_, 0.1);
	nh_.param<double>("angle_int_gain", angle_int_gain_, 0.1);
	nh_.param<double>("angle_diff_gain", angle_diff_gain_, 0.1);
	nh_.param<double>("angle_current_gain", angle_current_gain_, 0.1);

	nh_.param<double>("angle_to_path_tol", angle_to_path_tol_, 0.1);
	nh_.param<double>("angle_to_goal_tol", angle_to_goal_tol_, 0.1);
	nh_.param<double>("angle_vel_tol", angle_vel_tol_, 0.1);


	// Make sure we have odometry before moving on
	// TODO: add timeout
	ROS_INFO("Waiting for odom publisher");
	while(odom_sub_.getNumPublishers() <= 0 && ros::ok());

	update_timer_ = nh_.createTimer(update_freq_, boost::bind(&PointShoot::update_, this, _1) );

	// Wait till the first position is found then set that to desired position
	ROS_INFO("Waiting for first odom msg");
	while(current_pose_.position.x == 0)		// The likelihood of x actually being zero is very small
	{
		ros::spinOnce();
	}

	ROS_INFO("Odom received setting initial position and starting action server");
	desired_pose_ = current_pose_;

	// Now that we have our current pose start the action server
	//moveto_.registerPreemptCallback(boost::bind(&PointShoot::goalPreempt_, this));
	//moveto_.registerGoalCallback(boost::bind(&PointShoot::newGoal_, this, _1));
		// Works in initializer, but causes seg. fault in initializer... odd
	moveto_.start();

}

geometry_msgs::Pose old_current_pose;
geometry_msgs::Pose old_desired_pose;

/*
 * 		Update
 * 	This function updates the controller
 */
void PointShoot::update_(const ros::TimerEvent& nononononononon)
{
	// Check for new goal
	if(moveto_.isNewGoalAvailable()){
		ROS_INFO("New Goal");
		boost::shared_ptr<const uf_common::MoveToGoal> goal = moveto_.acceptNewGoal();
		desired_pose_ = goal->posetwist.pose;
		desired_twist_ = goal->posetwist.twist;	// Not used yet...
		ROS_INFO("New goal is: %f, %f, %f", desired_pose_.position.x, desired_pose_.position.y, desired_pose_.position.z);

		// Clear errors
		clearErrors_();
	}

	// Check if goal preempted
	if(moveto_.isPreemptRequested())
	{
		ROS_INFO("Goal preempted");
		// Set our current position to desired position
		desired_pose_ = current_pose_;
		desired_twist_ = zero_twist_;

		// Clear errors
		clearErrors_();

		return;
	}


	// TODO: add velocity considerations

	geometry_msgs::WrenchStamped msg = zero_wrench_;
	geometry_msgs::Wrench &wrench = msg.wrench;

	if(desired_pose_.position.x != old_desired_pose.position.x ||
			desired_pose_.position.y != old_desired_pose.position.y ||
			desired_pose_.position.z != old_desired_pose.position.z){

		old_desired_pose.position.x = desired_pose_.position.x;
		old_desired_pose.position.y = desired_pose_.position.y;
		old_desired_pose.position.z = desired_pose_.position.z;

		ROS_INFO("I am here: %f, %f, %f", current_pose_.position.x, current_pose_.position.y, current_pose_.position.z);
		ROS_INFO("I want to be here: %f, %f, %f", desired_pose_.position.x, desired_pose_.position.y, desired_pose_.position.z);
	}

	// Distance from goal is within tolerance
	if( current_linear_error_ < distance_tol_ ){

		if ( current_angular_error_ < angle_to_goal_tol_ ){
			//ROS_INFO("Station hold");
			// stationHold()

		}else{
			ROS_INFO("Standing on top of proper position; correcting Orientation");
			//ROS_INFO("Orient to point");
			wrench.torque = calculateTorque_();

		}

	// Distance from goal is far; boat is on the path
	}else{

		if ( current_angular_error_ > angle_to_path_tol_ ){
			ROS_INFO("Far away from target; Orient to path");
			wrench.torque = calculateTorque_();

		}else{
			ROS_INFO("Far away from target but aiming at it; Move toward Goal");
			wrench.force = calculateForce_();

		}
	}

	// Finally publish the wrench and servo
	/*self.servo_pub.publish(DynamixelFullConfig(
	                id=thruster['id'],
	                goal_position=2*thruster['angle'] + math.pi,
	                moving_speed=12, # near maximum, not actually achievable ...
	                torque_limit=1023,
	                goal_acceleration=38,
	                control_mode=DynamixelFullConfig.CONTINUOUS_ANGLE,
	                goal_velocity=2*thruster['dangle'],
	            ))*/
	dynamixel_servo::DynamixelFullConfig servo_pos;
	servo_pos.id = 2;
	servo_pos.goal_position = PI;
	servo_pos.moving_speed = 12;
	servo_pos.torque_limit = 1023;
	servo_pos.goal_acceleration = 38;
	servo_pos.control_mode = dynamixel_servo::DynamixelFullConfig::CONTINUOUS_ANGLE;
	servo_pos.goal_velocity = 10;

	servo_pub_.publish(servo_pos);
	servo_pos.id = 3;
	servo_pub_.publish(servo_pos);

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

	resulting_force.x = 50;
	resulting_force.y = 0;
	resulting_force.z = 0;

	return resulting_force;
}

geometry_msgs::Vector3 PointShoot::calculateTorque_(){
	geometry_msgs::Vector3 resulting_torque;

	resulting_torque.z = 9;
	resulting_torque.x = 0;
	resulting_torque.y = 0;

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


/************************
 * 			Main		*
 ************************/
int main(int argc, char** argv)
{
	//Initialize ROS
	ros::init(argc, argv, "point_shoot");

	// Initialize controller (everything)
	PointShoot ps;

	ros::spin();

	return 0;
}

#endif
