#ifndef ZDRIVE_H
#define ZDRIVE_H


#include <ros/ros.h>
#include <ros/package.h> // to get the file path of the running node
#include <dynamic_reconfigure/server.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include "dynamixel_servo/DynamixelFullConfig.h" //message to config a Dynamixel servo
#include "dynamixel_servo/DynamixelJointConfig.h" //simplified message to just set the position of a servo.
#include "dynamixel_servo/DynamixelStatus.h" //message for the status of a Dynamixel servo
#include "motor_control/thrusterNewtons.h"
#include "z_drive/ZDriveDbg.h"
#include "z_drive/BoatSimZDriveOutsideForce.h"
#include <z_drive/GainsConfig.h> // dynamic reconfig
#include "uf_common/MoveToAction.h"
#include <uf_common/PoseTwistStamped.h>
#include <actionlib/server/simple_action_server.h>
#include <sensor_msgs/JointState.h>
#include <sensor_msgs/Joy.h>
#include <sensor_msgs/Temperature.h>
#include <string>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
// for rviz points
#include <geometry_msgs/Point.h>
#include <visualization_msgs/Marker.h>

#include <boost/progress.hpp>
#include <stdlib.h>

// Per Ros cpp guidelines you should only refer to each element in namespace std that you want. see: http://wiki.ros.org/CppStyleGuide#Namespaces-1
using std::string;

#define ZDRIVE_DBG

class ZDrive
{
private:
	// Information relative to the boat that is used in tuning the controller
	double boat_mass; //Kg
	double boat_inertia; //(m^2)/Kg

	// remember according to ros rep 103: x=forward, y=left,	z=up
	// Currently the root of the z_drive tf tree is centered at the base_link
	static const double z_drive_root_x_offset;
	static const double z_drive_root_y_offset;
	static const double z_drive_root_z_offset;
	// port_servo is back .7239m, left of centerline .3048m, 0m above the water
	double port_servo_x_offset;
	double port_servo_y_offset;
	double port_servo_z_offset;
    
	// starboard_servo is back .7239m, right of center line .3048m, 0m above the water
	double starboard_servo_x_offset;
	double starboard_servo_y_offset;
	double starboard_servo_z_offset;
	// the servo(s) are an a belt drive. also the servos are in joint mode. in joint mode the servo can't smoothly transition from 360->0, it will go 360->359...1->0; hence the belt drive can eliminate this.
	// This functions assume that 0 radians corresponds to 0 rad on the servo, so we will adjust the results we estimate and get so that it is.
	double port_servo_angle_offset;
	double starboard_servo_angle_offset;

	// These are the tunable gain parameters for the "pd" controller. (Note: it is not a "pid" controller)
	double p_gain_x;
	double d_gain_x;
	double p_gain_y;
	double d_gain_y;
	// this is the angle (in radian) of the boat relative to the world coordinate frame
	double p_gain_theta_boat;
	double d_gain_theta_boat;

	// these are gains for the cost function
	double gain_error_force_x;
	double gain_error_force_y;
	double gain_error_moment_z;
	double gain_thrusters_force; // all thrusters will have the same gain amount for the thrusters. This essentially is to penalize the system (in the cost function) for using more energy.
	double gain_deviation_equilibrum_servo_angle; // all servos want to be pointing towards equilibrium/center (to minimize the motion used by the system). Note: since all servos are the same (e.g. same specs) they have the same gain_deviation_equilibrum_servo_angle
	double gain_deviation_changeof_servo_angle; // we want to also minimize the overall amount in which the servos will have to turn (to minimize the motion used by the system). Note: since all servos are the same (e.g. same specs) they have the same gain_deviation_changeof_servo_angle

	// these are is the topic names to listen to for various messages.
	static const string odom_topic;
	static const string dynamixel_namespace;
	static const string dynamixel_status_topic;
	static const string dynamixel_config_topic;
	static const string thruster_status_topic;
	static const string thruster_config_topic;

	// When working to minimize the cost function, a buffer is used to get the min in a range. This is the size of that buffer/range.
	int cost_count_max;

	// All the partial derivative functions will use these temporary variables in their calculations. To make the code look cleaner and to not have to recalculate them with each call to the partial derivatives, they have been made members of the class.
	// remember according to ros rep 103: x=bow=forward, y=port=left
	// the required force is what we would want in an ideal world
	double force_port_required; // =requiredForceY()
	double force_bow_required; // =requiredForceX()
	double moment_z_required; // =requiredMomentZ()

	// this is the step amount that is used when finding the min of our cost function, this amount will change dynamicaly
	double step_size;
	double step_multiplier;
	double step;

	// These values will converge closer and closer to the optimal solution- hence why they are an estimate- with every iteration of calcAngleAndThrustEstimate.
	double estimated_port_servo_angle;
	double estimated_port_thruster_force;
	double estimated_starboard_servo_angle;
	double estimated_starboard_thruster_force;

	// These are locked at the beginning of a calculation of the cost optimization function
	double current_port_servo_angle;
	double current_port_thruster_force;
	double current_starboard_servo_angle;
	double current_starboard_thruster_force;

	// These are the bounds of the servos (angles are in radians and thrust is in newtons). Note: the thrusters can generate thrust in the forward or reverse direction.
	// Note: The Servo Angle limits here will comply with REP103; so 0 radians coincides with the bow, +pi/2 coincides with port, and -pi/2 coincides with starboard.
	double port_servo_angle_clock_wise_limit;
	double port_servo_angle_counter_clock_wise_limit;
	double port_servo_gear_reduction;
	double port_thruster_reverse_limit;
	double port_thruster_foward_limit;
	double starboard_servo_angle_clock_wise_limit;
	double starboard_servo_angle_counter_clock_wise_limit;
	double starboard_servo_gear_reduction;
	double starboard_thruster_reverse_limit;
	double starboard_thruster_foward_limit;



	// These are the ID(s) of the servo(s) in the z_drive- since each servo on a dynamixel bus must have a unique id.
	// Note: The id field in a DynamixelStatusParam msg is a uint8, so the id(s) here are also uint8(s) to avoid type casting.
	uint8_t port_servo_id;
	uint8_t starboard_servo_id;

	// these are simplifications for the current odom information of the boat. These are in the WORLD's frame of reference.
	// remember according to ros rep 103: x=forward, y=left,	z=up
	double x_current;
	double x_velocity_current;
	double y_current;
	double y_velocity_current;
	double z_current;
	double z_velocity_current;
	double yaw_current;
	double yaw_velocity_current;
	double roll_current;
	double roll_velocity_current;
	double pitch_current;
	double pitch_velocity_current;
	// these are simplifications for the desired (what we want the boat to do) odom information of the boat
	// remember according to ros rep 103: x=forward, y=left,	z=up
	double x_desired;
	double x_velocity_desired;
	double y_desired;
	double y_velocity_desired;
	double z_desired;
	double z_velocity_desired;
	double yaw_desired;
	double yaw_velocity_desired;
	double roll_desired;
	double roll_velocity_desired;
	double pitch_desired;
	double pitch_velocity_desired;

	// friction coefficients that are relative to the boat's frame of reference
	double friction_coefficient_forward;
	double friction_coefficient_forward_reduction;// as the boat attains a higher velocity the frictional forces get reduced
	double friction_coefficient_lateral;
	double friction_coefficient_lateral_reduction;// as the boat attains a higher velocity the frictional forces get reduced
	double friction_coefficient_rotational;
	double friction_coefficient_rotational_reduction;// as the boat attains a higher velocity the frictional forces get reduced
	// Frictional forces that are relative to the boats frame of reference
	double friction_force_forward;
	double friction_force_lateral;
	double friction_force_rotational;

	bool desired_position_inited;
	bool kill_thrusters;
	bool kill_action;
	bool new_action;

	// we would like an idea of how long it takes to do specific computations in some functions, and will use this variable for that.
	double computational_time;

	// Dynamically reconfigurable control modes for how the boat should drive
	static const int XBOX_ANALOG_ONLY=0;
	static const int XBOX_ANALOG_TO_REQUIRED=1;
	static const int WAYPOINT_DRIVING=2;
	static const int MANUAL_MODE=3;
	static const int COST_DBG_MODE=4;
	static const int MANUAL_TO_REQUIRED=5;
	static const int MIRROR_MODE_JACKSON=6;
	static const int XBOX_ANALOG_ONLY_JACKSON=7;
	static const int TESTING_MODE=8;
	static const int EXTRA_9=9;

	// This is a variable to allow the control metthod to be changed dynamicaly
	int control_method; // since we have the ability to quickly change the control mode with the xbox control (so we dont run into things), we also want the ability to know how to restore the mode we just escaped out of.
	int prev_control_method;

	// for mapping see http://wiki.ros.org/joy
	// there are two kernel modules that can interface with the xbox controller: "xpad" xor "xboxdrv". The pre-installed module with Ubuntu 13.04 is "xpad"
	// Note: the joynode message "axes" are continuous from [-1 to 1] and "buttons" are discrete (-1,1)
	// xpad module maps the following in the joynode message
	//        [0, 1, 2, 3, 4 , 5 , 6   , 7    , 8       , 9           , 10          ]
	// buttons[A, B, X, Y, LB, RB, BACK, START, XBOX_BTN, L_STICK_DOWN, R_STICK_DOWN] these are 0/1 values
	//			[              0             ,              1          , 2 ,               3             ,              4          , 5 ,         6        ,         7        ]
	// axes: [-L_STICK_RIGHT:+L_STICK_LEFT, -L_STICK_DOWN:L_STICK_UP, LT, -R_STICK_RIGHT:+R_STICK_LEFT, -R_STICK_DOWN:R_STICK_UP, RT, -R_D_PAD:+L_D_PAD, -D_D_PAD:+U_D_PAD] these are [-1:1]
	// xboxdrv module maps the following in the joynode message
	//        [0, 1, 2, 3, 4 , 5 , 6   , 7    , 8       , 9           , 10          ]
	// buttons[A, B, X, Y, LB, RB, BACK, START, XBOX_BTN, L_STICK_DOWN, R_STICK_DOWN] these are 0/1 values
	//			[              0             ,              1          ,               2             ,              3          , 4 ,  5 ,         6        ,         7        ]
	// axes: [-L_STICK_RIGHT:+L_STICK_LEFT, -L_STICK_DOWN:L_STICK_UP, -R_STICK_RIGHT:+R_STICK_LEFT, -R_STICK_DOWN:R_STICK_UP, RT,  LT, -R_D_PAD:+L_D_PAD, -D_D_PAD:+U_D_PAD] these are [-1:1]
	// buttons locations for information
	static const unsigned int XBOX_A=0;
	static const unsigned int XBOX_B=1;
	static const unsigned int XBOX_X=2;
	static const unsigned int XBOX_Y=3;
	static const unsigned int XBOX_L_BTN=4;
	static const unsigned int XBOX_R_BTN=5;
	static const unsigned int XBOX_BACK=6;
	static const unsigned int XBOX_START=7;
	static const unsigned int XBOX_BTN=8;
	static const unsigned int XBOX_L_STICK_DOWN=9;
	static const unsigned int XBOX_R_STICK_DOWN=10;
	// axis[] locations for relative information
	static const unsigned int XBOX_L_STICK_LAT=0; //latitudinal movement is left right
	static const unsigned int XBOX_L_STICK_LON=1; //longitudinal movement is up down
	static const unsigned int XBOX_R_STICK_LAT=2;
	static const unsigned int XBOX_R_STICK_LON=3;
	static const unsigned int XBOX_L_TRIGGER=5;
	static const unsigned int XBOX_R_TRIGGER=4;
	static const unsigned int XBOX_DPAD_LAT=6;
	static const unsigned int XBOX_DPAD_LON=7;


	// these are to just temporarily store the messages that are taken in the callback(s)- for debuging purposes only.
	// Note: We will then strip out the simplified information that we care about
	nav_msgs::Odometry odom_current;
	nav_msgs::Odometry odom_desired;

	ros::NodeHandle n;
	static const double update_rate=50.0; // rate that the controller runs at in Hz
	ros::Subscriber odom_subscriber;
	ros::Subscriber thruster_status_subscriber;
	ros::Subscriber dynamixel_status_subscriber;
	ros::Subscriber joystick_subscriber;
	ros::Publisher dynamixel_config_full_pub;
	ros::Publisher dynamixel_config_position_pub;
	ros::Publisher thruster_config_pub;
	ros::Publisher z_drive_dbg_pub;
	ros::Publisher trajectory_pub;
	ros::Publisher z_drive_sim_pub;
	dynamic_reconfigure::Server<z_drive::GainsConfig> reconfig_server;
	dynamic_reconfigure::Server<z_drive::GainsConfig>::CallbackType reconfig_callback;
	z_drive::GainsConfig current_dynamic_config;
	z_drive::GainsConfig default_dynamic_config;
	bool update_dynamic_param_server;
	z_drive::ZDriveDbg dbg_msg;
	tf::TransformBroadcaster tf_brodcaster;
	tf::Transform z_drive_root_tf;
	tf::Transform port_thruster_tf;
	tf::Transform starboard_thruster_tf;
	tf::Transform port_prop_tf;
	tf::Transform starboard_prop_tf;
	ros::Publisher joint_pub;
	sensor_msgs::JointState joint_state;
	ros::Publisher temperature_pub;
	sensor_msgs::Temperature temperature_data;
	//ros::Publisher marker_pub;
	//visualization_msgs::Marker marker_data;

	std::vector<double> inital_theta_port;
	std::vector<double> inital_thrust_port;
	std::vector<double> inital_theta_starboard;
	std::vector<double> inital_thrust_starboard;

public:
	ZDrive();
	void run();

protected:
	void currentOdomCallBack(const nav_msgs::Odometry& odom_msg);
	void desiredOdomCallBack(const  uf_common::PoseTwist desired_pose_twist);
	void dynamixelStatusCallBack(const dynamixel_servo::DynamixelStatus& dynamixel_status_msg);
	void thrusterStatusCallBack(const motor_control::thrusterNewtons& thruster_status_msg);
	void joystickCallBack(const sensor_msgs::Joy::ConstPtr& joystick_msg);
	void dynamicReconfigCallBack(const z_drive::GainsConfig &config, uint32_t level);

	// remember x is forward, y is left right, and z is up
	// Note: in an ideal system the resultant force/moment would equal the required force/moment, but our system isn't ideal; thus we must calculate both.
	// this is the force/moment that can be provided to the system so we can move to our desired position
	double resultantForceX(double port_servo_angle,double port_thruster_force,double starboard_servo_angle,double starboard_thruster_force);
	double resultantForceY(double estimated_port_servo_angle, double estimated_port_thruster_force, double estimated_starboard_servo_angle, double estimated_starboard_thruster_force);
	double resultantMomentZ(double estimated_port_servo_angle, double estimated_port_thruster_force, double estimated_starboard_servo_angle, double estimated_starboard_thruster_force);
	// this is what we want the system to do, but it may not be able to.
	double requiredForceX(double x_current, double x_desired, double x_velocity_current, double x_velocity_desired);
	double requiredForceY(double y_current, double y_desired, double y_velocity_current, double y_velocity_desired);
	double requiredMomentZ(double boat_angle_current, double boat_angle_desired, double boat_angular_velocity_current, double boat_angular_velocity_desired);
	// since the step size varies with each iteration to find the min of the cost function, this will calculate the new step_size amount
	void minimizeCostFunction();
	void debugCostFunction();
	void calcAngleAndThrustEstimate();
	void guessInitalValues();
	double calcCost(double port_servo_angle, double port_thruster_force, double starboard_servo_angle, double starboard_thruster_force);
	void checkEstimatedValuesAgainsBounds();
	// these are the partial derivatives for the cost function
	double dCost_dPortServoAngle(double port_servo_angle,double port_thruster_force,double starboard_servo_angle,double starboard_thruster_force);
	double dCost_dPortThrusterForce(double port_servo_angle,double port_thruster_force,double starboard_servo_angle,double starboard_thruster_force);
	double dCost_dStarboardServoAngle(double port_servo_angle,double port_thruster_force,double starboard_servo_angle,double starboard_thruster_force);
	double dCost_dStarboardThrusterForce(double port_servo_angle,double port_thruster_force,double starboard_servo_angle,double starboard_thruster_force);
	// this will pull all the curent information form the class and post it in a dbg msg
	void publishDbgMsg(double user_defined_1, double user_defined_2, double user_defined_3, double user_defined_4, double user_defined_5);
};
// remember according to ros rep 103: x=forward, y=left,	z=up
// Currently the root of the z_drive tf tree is centered at the base_link
const double ZDrive::z_drive_root_x_offset=0.0;
const double ZDrive::z_drive_root_y_offset=0.0;
const double ZDrive::z_drive_root_z_offset=0.0;

const string ZDrive::odom_topic="odom";
const string ZDrive::dynamixel_namespace="dynamixel";
const string ZDrive::dynamixel_status_topic="dynamixel_status_post";
const string ZDrive::dynamixel_config_topic="dynamixel_config";
const string ZDrive::thruster_status_topic="thruster_status_post";
const string ZDrive::thruster_config_topic="thruster_config";





ZDrive::ZDrive(): force_port_required(0.0), force_bow_required(0.0), moment_z_required(0.0), estimated_port_thruster_force(0.0), estimated_port_servo_angle(0.0), estimated_starboard_thruster_force(0.0), estimated_starboard_servo_angle(0.0),
		boat_mass(36.2874), boat_inertia(7.4623), port_servo_x_offset(-.7239), port_servo_y_offset(.3048), port_servo_z_offset(0.0), starboard_servo_x_offset(-.7239), starboard_servo_y_offset(-.3048), starboard_servo_z_offset(0.0),
		p_gain_x(50), p_gain_y(50), p_gain_theta_boat(1.0), gain_error_force_x(100), gain_error_force_y(1000), gain_error_moment_z(1000), gain_thrusters_force(10), gain_deviation_equilibrum_servo_angle(0), gain_deviation_changeof_servo_angle(.01),
		cost_count_max(20), friction_coefficient_forward(0.0), friction_coefficient_forward_reduction(0.0), friction_coefficient_lateral(0.0), friction_coefficient_lateral_reduction(0.0), friction_coefficient_rotational(0.0), friction_coefficient_rotational_reduction(0.0),
		friction_force_forward(0.0), friction_force_lateral(0.0), friction_force_rotational(0.0), update_dynamic_param_server(false), computational_time(0), new_action(false), port_servo_gear_reduction(2.0), starboard_servo_gear_reduction(2.0), control_method(0), prev_control_method(0)
{
	d_gain_x=sqrt(4*p_gain_x*boat_mass);
	d_gain_y=sqrt(4*p_gain_y*boat_mass);
	d_gain_theta_boat=sqrt(4*p_gain_theta_boat*boat_inertia);


	desired_position_inited=false;
	// initaly the thrusters should be killed
	ZDrive::kill_thrusters=true;

	// While this should not be an issue on most computers, this check is added to try and ensure that doubles (like ros float64 msg types) are ieee754 compliant. See: http://goo.gl/mfC8tJ and http://goo.gl/AN8311
	if(std::numeric_limits<double>::is_iec559==false)
	{
		ROS_WARN("The variable type \"double\" in c++ should be in ieee754 format to comply with ros \"float64\" primitive type. See http://wiki.ros.org/msg");
	}

	// init values
	// Note: The Servo Angle limits here will comply with REP103; so 0 radians coincides with the bow, +pi/2 coincides with port, and -pi/2 coincides with starboard.
	port_servo_angle_clock_wise_limit=(-89*M_PI)/180; // clockwise is towards starboard
	port_servo_angle_counter_clock_wise_limit=(89*M_PI)/180; //counterclockwise is towards port
	//TODO: update based on new motors
	// the specs on the troling motors say about 18lbs of thrust, which should be about 80.0679 newtons
	port_thruster_foward_limit=25;
	// you don't get full thrust in reverse so about 80%
	port_thruster_reverse_limit=-25;

	starboard_servo_angle_clock_wise_limit=(-89*M_PI)/180; // clockwise is towards starboard
	starboard_servo_angle_counter_clock_wise_limit=(89*M_PI)/180; //counterclockwise is towards port
	starboard_thruster_foward_limit=port_thruster_foward_limit;
	starboard_thruster_reverse_limit=port_thruster_reverse_limit;
	port_servo_angle_offset=M_PI;
	starboard_servo_angle_offset=M_PI;

	// init all the values for what we will estimate
	current_port_servo_angle=0.0;
	current_port_thruster_force=0.0;
	current_starboard_servo_angle=0.0;
	current_starboard_thruster_force=0.0;
	//init all the odom messages so a normalization error isn't generated due to the fact that the estimation routine runs before a callback happens initaly.
	odom_current.pose.pose.position.x=0;
	odom_current.pose.pose.position.y=0;
	odom_current.pose.pose.position.z=0;
	odom_current.pose.pose.orientation=tf::createQuaternionMsgFromRollPitchYaw(0,0,0);
	odom_desired.pose.pose.position.x=0;
	odom_desired.pose.pose.position.y=0;
	odom_desired.pose.pose.position.z=0;
	odom_desired.pose.pose.orientation=tf::createQuaternionMsgFromRollPitchYaw(0,0,0);

	//create a parent node in the tree to organize the zdrive components under
	z_drive_root_tf.setOrigin(tf::Vector3(z_drive_root_x_offset, z_drive_root_y_offset, z_drive_root_z_offset));
	z_drive_root_tf.setRotation(tf::Quaternion(0,0,0));

	port_thruster_tf.setOrigin(tf::Vector3(port_servo_x_offset,port_servo_y_offset,port_servo_z_offset));
	// no rotation to start
	port_thruster_tf.setRotation(tf::Quaternion(0,0,0));
	port_prop_tf.setOrigin(tf::Vector3(-.25,0,0));
	port_prop_tf.setRotation(tf::Quaternion(0,M_PI/2,0));

	starboard_thruster_tf.setOrigin(tf::Vector3(starboard_servo_x_offset,starboard_servo_y_offset,starboard_servo_z_offset));
	// no rotation to start
	starboard_thruster_tf.setRotation(tf::Quaternion(0,0,0));
	starboard_prop_tf.setOrigin(tf::Vector3(-.25,0,0));
	starboard_prop_tf.setRotation(tf::Quaternion(0,M_PI/2,0));

	// set the update rate (in hz) for which the pd controller runs

	// init the id variables for what their corresponding servo's are on the dynamixel bus
	port_servo_id=0x03;
	starboard_servo_id=0x02;

	// get the fully qualified namespace for the dynamixel node
	string dynamixel_fqns=ros::names::resolve(dynamixel_namespace,true);

	// Initialize the subscribers and their callback functions (which will just update things to what their current values are).
	odom_subscriber=n.subscribe(odom_topic,1000, &ZDrive::currentOdomCallBack,this);
	dynamixel_status_subscriber=n.subscribe(dynamixel_fqns+"/"+dynamixel_status_topic, 1000, &ZDrive::dynamixelStatusCallBack, this);
	thruster_status_subscriber=n.subscribe("thruster_status",1000,&ZDrive::thrusterStatusCallBack ,this);
	joystick_subscriber=n.subscribe("joy",100,&ZDrive::joystickCallBack,this);

	//Advertise the various publisher(s)
	dynamixel_config_full_pub=n.advertise<dynamixel_servo::DynamixelFullConfig>(dynamixel_fqns+"/"+"dynamixel_config_full",1000);
	dynamixel_config_position_pub=n.advertise<dynamixel_servo::DynamixelJointConfig>(dynamixel_fqns+"/"+"dynamixel_joint_config",1000);
	thruster_config_pub=n.advertise<motor_control::thrusterNewtons>("thruster_config",100);
	z_drive_dbg_pub=n.advertise<z_drive::ZDriveDbg>("z_drive_dbg_msg",1000);
	joint_pub=n.advertise<sensor_msgs::JointState>("joint_states",100);
	temperature_pub=n.advertise<sensor_msgs::Temperature>("temperatures",100);
	trajectory_pub = n.advertise<uf_common::PoseTwistStamped>("trajectory", 1);
	z_drive_sim_pub = n.advertise<z_drive::BoatSimZDriveOutsideForce>("z_drive_sim_force",100);
	//marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);

	//configure our dynamic reconfigurable server
	reconfig_callback=boost::bind(&ZDrive::dynamicReconfigCallBack, this, _1, _2);
	reconfig_server.setCallback(reconfig_callback);

	// the following is a quick hack for darsan to test things
	std::string node_name=ros::this_node::getName();
	if(node_name[0]=='/')
	{
		node_name.erase(0,1);//remove starting '/'
	}
	std::string package_path = ros::package::getPath(node_name);
	if (package_path.empty() == true)
	{
		ROS_WARN("Error getting this nodes (%s) file path, so we can open scripts/inital_values", ros::this_node::getName().c_str());
	}
	else
	{

		ROS_INFO("Loading the file: %s/scripts/initial_values.txt", package_path.c_str());
		package_path.append("/scripts/initial_values.txt");
		std::ifstream fin(package_path.c_str());
		if (!fin)
		{
			ROS_WARN("Error opening the file: %s",package_path.c_str());
		}
		else
		{
			string file_line = "";
			//std::vector<double> data;
			float tmp0 = 0;
			float tmp1 = 0;
			float tmp2 = 0;
			float tmp3 = 0;
			int i = 0;
			while (fin.eof() != true)
			{
				i++;
				getline(fin, file_line);
				if (sscanf(file_line.c_str(), "%f%f%f%f", &tmp0, &tmp1, &tmp2, &tmp3) != 4)
				{
					ROS_WARN("Line %d doesn't have the correct number of args %f, %f, %f, %f", i, tmp0, tmp1, tmp2, tmp3);
				}
				else
				{
					ROS_INFO("Read %f, %f, %f, %f", tmp0, tmp1, tmp2, tmp3);
					//std::vector<double>::iterator it = data.begin();
					ZDrive::inital_theta_port.push_back((double)tmp0);
					ZDrive::inital_thrust_port.push_back((double)tmp1);
					ZDrive::inital_theta_starboard.push_back((double)tmp2);
					ZDrive::inital_thrust_starboard.push_back((double)tmp3);
					ROS_INFO("Read %f, %f, %f, %f", inital_theta_port[i - 1], inital_thrust_port[i - 1],
								inital_theta_starboard[i - 1], inital_thrust_starboard[i - 1]);
				}
				tmp0=0;
				tmp1=0;
				tmp2=0;
				tmp3=0;
			}
			fin.close();
		}
	}
}
void ZDrive::dynamixelStatusCallBack(const dynamixel_servo::DynamixelStatus& dynamixel_status_msg)
{
	// Note: we are basically having to account for how the dynimixel is mounted on the boat, and translate it into the boats world- hence the multiplyer and the offset.
	// we also have to account for any gearing hence the division
	if(dynamixel_status_msg.id==port_servo_id)
	{
		// note: present_position is a float32. Hence the cast is explicitly shown. The dynamixel node is optimized for speed and memory to be near real-time, hence the float32.
		ZDrive::current_port_servo_angle=-1.0*((double)dynamixel_status_msg.present_position)/port_servo_gear_reduction+port_servo_angle_offset/port_servo_gear_reduction;
		temperature_data.header.stamp = ros::Time::now();
		temperature_data.header.frame_id="port_thruster";
		temperature_data.temperature=dynamixel_status_msg.present_temp;
		temperature_pub.publish(temperature_data);
	}
	else if(dynamixel_status_msg.id==starboard_servo_id)
	{
		ZDrive::current_starboard_servo_angle=-1.0*((double)dynamixel_status_msg.present_position)/starboard_servo_gear_reduction+starboard_servo_angle_offset/starboard_servo_gear_reduction;
		temperature_data.header.stamp = ros::Time::now();
		temperature_data.header.frame_id="starboard_thruster";
		temperature_data.temperature=dynamixel_status_msg.present_temp;
		temperature_pub.publish(temperature_data);
	}
	return;
}

void ZDrive::thrusterStatusCallBack(const motor_control::thrusterNewtons& thruster_status_msg)
{
	if(thruster_status_msg.id==port_servo_id)
	{
		ZDrive::current_port_thruster_force=thruster_status_msg.thrust;
	}
	else if(thruster_status_msg.id==starboard_servo_id)
	{
		ZDrive::current_starboard_thruster_force=thruster_status_msg.thrust;
	}
	return;
}

void ZDrive::joystickCallBack(const sensor_msgs::Joy::ConstPtr& joystick_msg)
{
	// Note: we first process the buttons that relate to the dyn_param server (giving priority to driving in analog mode to rescue the boat)
	// for mapping see http://wiki.ros.org/joy
	//        [0, 1, 2, 3, 4 , 5 , 6   , 7    , 8     , 9           , 10          ]
	// buttons[A, B, X, Y, LB, RB, BACK, START, XBOX_X, L_STICK_DOWN, R_STICK_DOWN] these are 0/1 values
	//			[              0             ,              1          , 2 ,               3             ,              4          , 5 ,         6        ,         7        ]
	// axes: [-L_STICK_RIGHT:+L_STICK_LEFT, -L_STICK_DOWN:L_STICK_UP, LT, -R_STICK_RIGHT:+R_STICK_LEFT, -R_STICK_DOWN:R_STICK_UP, RT, -R_D_PAD:+L_D_PAD, -D_D_PAD:+U_D_PAD] these are [-1:1]
	// NOTE: THE CONTROLERS PERSPECTIVE FOR L/R ON THE ANALOG STICKS IS REVERSED/upsidedown WHEN COMPARED TO THE BOAT AND REP 103. clockwise is towards starboard and is negative, counter-clockwise is towards port and is positive
	// Based REP103 and the Boat; 0 radians coincides with the bow, +pi/2 coincides with port, and -pi/2 coincides with starboard.
	if(joystick_msg->buttons[XBOX_A]==1)
	{
		if(ZDrive::control_method!=XBOX_ANALOG_ONLY)
		{
			ROS_INFO("Switching to XBOX_ANALOG_ONLY mode");
			ZDrive::prev_control_method=ZDrive::control_method;
			ZDrive::control_method=ZDrive::XBOX_ANALOG_ONLY; // To stop the boat from running into walls, we need to quickly be able to escape out of a mode, hence xbox_mode
			ZDrive::update_dynamic_param_server=true;
			ZDrive::current_dynamic_config.control_method=ZDrive::XBOX_ANALOG_ONLY;
		}
	}
	else if(joystick_msg->buttons[XBOX_BACK]==1)
	{
		// reinit the dyn param server with default values
		ROS_INFO("Reinitializing the dynamic parameter server with the default values specified in the config (.cfg) file XBOX_ANALOG_ONLY mode");
		ZDrive::prev_control_method=ZDrive::control_method;
		ZDrive::update_dynamic_param_server=true;
		ZDrive::control_method=ZDrive::default_dynamic_config.control_method;
		ZDrive::current_dynamic_config=ZDrive::default_dynamic_config;
	}
	else if(joystick_msg->buttons[XBOX_B]==1)
	{
		if(ZDrive::control_method!=ZDrive::prev_control_method)
		{
			// restore the previous control method
			ROS_INFO("Switching back to control mode: %d", ZDrive::prev_control_method);
			int temp_control_method=ZDrive::control_method;
			ZDrive::control_method=ZDrive::prev_control_method;
			ZDrive::prev_control_method=temp_control_method;
			ZDrive::update_dynamic_param_server=true;
			ZDrive::current_dynamic_config.control_method=ZDrive::control_method;
		}
	}
	else if(joystick_msg->buttons[XBOX_X]==1)
	{
		// if the x button is pushed set it so the desired positions and velocities are exactly what they currently are in the real world
		ROS_INFO("Setting the desired waypoint to where the boat currently is (subsequently killing any previously active way points");
		ZDrive::kill_action=true;
		// stop as quickly as possible at your current position and kill the velocity
		ZDrive::x_desired = ZDrive::x_current;
		ZDrive::y_desired = ZDrive::y_current;
		ZDrive::z_desired = ZDrive::z_current;
		ZDrive::x_velocity_desired=0;
		ZDrive::y_velocity_desired=0;
		ZDrive::z_velocity_desired=0;
		ZDrive::roll_desired=ZDrive::roll_current;
		ZDrive::pitch_desired=ZDrive::pitch_current;
		ZDrive::yaw_desired = ZDrive::yaw_current;
		ZDrive::roll_velocity_desired=0;
		ZDrive::pitch_velocity_desired=0;
		ZDrive::yaw_velocity_desired=0;
	}
	else if(joystick_msg->buttons[XBOX_Y]==1)
	{
		// create a point to display in rviz
		geometry_msgs::Point marker_position;
		marker_position.x=ZDrive::x_current;
		marker_position.y=ZDrive::y_current;
		marker_position.z=ZDrive::z_current;
	}

	if(ZDrive::control_method==ZDrive::XBOX_ANALOG_ONLY)
	{
		if(joystick_msg->axes[XBOX_L_STICK_LAT]>=0.0)// left stick left
		{
			// Note: axes[0] and ZDrive::port_servo_angle_counter_clock_wise_limit are both +, so this must be compensated such that the correct sign is still applied.
			ZDrive::estimated_port_servo_angle=-(joystick_msg->axes[XBOX_L_STICK_LAT]*ZDrive::port_servo_angle_counter_clock_wise_limit);
		}
		else
		{
			// clockwise is towards starboard and is negative
			ZDrive::estimated_port_servo_angle=joystick_msg->axes[XBOX_L_STICK_LAT]*ZDrive::port_servo_angle_clock_wise_limit;
		}
		if(joystick_msg->axes[XBOX_L_STICK_LON]>=0.0)// left stick up
		{
			ZDrive::estimated_port_thruster_force=joystick_msg->axes[XBOX_L_STICK_LON]*ZDrive::port_thruster_foward_limit;
		}
		else
		{
			// Note: axes[1] and ZDrive::port_thruster_reverse_limit are both negative, so this must be compensated such that the correct sign is still applied.
			ZDrive::estimated_port_thruster_force=fabs(joystick_msg->axes[XBOX_L_STICK_LON])*ZDrive::port_thruster_reverse_limit;
		}

		if (joystick_msg->axes[XBOX_R_STICK_LAT] >= 0.0)// right stick left
		{
			// Note: axes[3] and ZDrive::starboard_servo_angle_counter_clock_wise_limit are both +, so this must be compensated such that the correct sign is still applied.
			ZDrive::estimated_starboard_servo_angle =-(joystick_msg->axes[XBOX_R_STICK_LAT] * ZDrive::starboard_servo_angle_counter_clock_wise_limit);
		}
		else
		{
			ZDrive::estimated_starboard_servo_angle =joystick_msg->axes[XBOX_R_STICK_LAT] * ZDrive::starboard_servo_angle_clock_wise_limit;
		}
		if (joystick_msg->axes[XBOX_R_STICK_LON] >= 0.0)// right stick up
		{
			ZDrive::estimated_starboard_thruster_force = joystick_msg->axes[XBOX_R_STICK_LON] * ZDrive::starboard_thruster_foward_limit;
		}
		else
		{
			// Note: axes[4] and ZDrive::starboard_servo_angle_clock_wise_limit are both negative, so this must be compensated such that the correct sign is still applied.
			ZDrive::estimated_starboard_thruster_force = fabs(joystick_msg->axes[XBOX_R_STICK_LON])*ZDrive::starboard_thruster_reverse_limit;
		}
	}
	if(ZDrive::control_method==ZDrive::XBOX_ANALOG_ONLY_JACKSON) // similar to XBOX_ANALOG_ONLY, but left and right are reversed
	{
		if(joystick_msg->axes[XBOX_L_STICK_LAT]>=0.0)// left stick left
		{
			// Note: axes[0] and ZDrive::port_servo_angle_counter_clock_wise_limit are both +, so this must be compensated such that the correct sign is still applied.
			ZDrive::estimated_port_servo_angle=(joystick_msg->axes[XBOX_L_STICK_LAT]*ZDrive::port_servo_angle_counter_clock_wise_limit);
		}
		else
		{
			// clockwise is towards starboard and is negative
			ZDrive::estimated_port_servo_angle=-joystick_msg->axes[XBOX_L_STICK_LAT]*ZDrive::port_servo_angle_clock_wise_limit;
		}
		if(joystick_msg->axes[XBOX_L_STICK_LON]>=0.0)// left stick up
		{
			ZDrive::estimated_port_thruster_force=joystick_msg->axes[XBOX_L_STICK_LON]*ZDrive::port_thruster_foward_limit;
		}
		else
		{
			// Note: axes[1] and ZDrive::port_thruster_reverse_limit are both negative, so this must be compensated such that the correct sign is still applied.
			ZDrive::estimated_port_thruster_force=fabs(joystick_msg->axes[XBOX_L_STICK_LON])*ZDrive::port_thruster_reverse_limit;
		}

		if (joystick_msg->axes[XBOX_R_STICK_LAT] >= 0.0)// right stick left
		{
			// Note: axes[3] and ZDrive::starboard_servo_angle_counter_clock_wise_limit are both +, so this must be compensated such that the correct sign is still applied.
			ZDrive::estimated_starboard_servo_angle =(joystick_msg->axes[XBOX_R_STICK_LAT] * ZDrive::starboard_servo_angle_counter_clock_wise_limit);
		}
		else
		{
			ZDrive::estimated_starboard_servo_angle =-joystick_msg->axes[XBOX_R_STICK_LAT] * ZDrive::starboard_servo_angle_clock_wise_limit;
		}
		if (joystick_msg->axes[XBOX_R_STICK_LON] >= 0.0)// right stick up
		{
			ZDrive::estimated_starboard_thruster_force = joystick_msg->axes[XBOX_R_STICK_LON] * ZDrive::starboard_thruster_foward_limit;
		}
		else
		{
			// Note: axes[4] and ZDrive::starboard_servo_angle_clock_wise_limit are both negative, so this must be compensated such that the correct sign is still applied.
			ZDrive::estimated_starboard_thruster_force = fabs(joystick_msg->axes[XBOX_R_STICK_LON])*ZDrive::starboard_thruster_reverse_limit;
		}
	}
	else if(ZDrive::control_method==ZDrive::XBOX_ANALOG_TO_REQUIRED)
	{
		ZDrive::force_port_required=joystick_msg->axes[XBOX_L_STICK_LAT]*500;//-L_STICK_RIGHT:+L_STICK_LEFT
		ZDrive::force_bow_required=joystick_msg->axes[XBOX_R_STICK_LON]*500;//-R_STICK_DOWN:R_STICK_UP
		double tmp=0.0;
		if(joystick_msg->axes[XBOX_L_TRIGGER]<0.0)//LT pressed
		{
			tmp-=fabs(joystick_msg->axes[XBOX_L_TRIGGER]);
		}
		if(joystick_msg->axes[XBOX_R_TRIGGER]<0.0)//RT pressed
		{
			tmp+=fabs(joystick_msg->axes[XBOX_R_TRIGGER]);
		}
		ZDrive::moment_z_required=tmp*100;//rotate proportional to the amount that the left and right triggers are pressed
	}
	else if(ZDrive::control_method==ZDrive::MIRROR_MODE_JACKSON)
	{
		// the left analog stick up and down is the thrust
		if(joystick_msg->axes[XBOX_L_STICK_LON]>=0.0)// left stick up
		{
			ZDrive::estimated_port_thruster_force=joystick_msg->axes[XBOX_L_STICK_LON]*ZDrive::port_thruster_foward_limit;
			ZDrive::estimated_starboard_thruster_force=joystick_msg->axes[XBOX_L_STICK_LON]*ZDrive::starboard_thruster_foward_limit;
		}
		else
		{
			// Note: axes[1] and ZDrive::port_thruster_reverse_limit are both negative, so this must be compensated such that the correct sign is still applied.
			ZDrive::estimated_port_thruster_force=fabs(joystick_msg->axes[XBOX_L_STICK_LON])*ZDrive::port_thruster_reverse_limit;
			ZDrive::estimated_starboard_thruster_force=fabs(joystick_msg->axes[XBOX_L_STICK_LON])*ZDrive::starboard_thruster_reverse_limit;
		}
		if (joystick_msg->axes[XBOX_R_STICK_LAT] >= 0.0)// right stick left
		{
			// Note: axes[3] and ZDrive::starboard_servo_angle_counter_clock_wise_limit are both +, so this must be compensated such that the correct sign is still applied.
			ZDrive::estimated_starboard_servo_angle =(joystick_msg->axes[XBOX_R_STICK_LAT] * ZDrive::starboard_servo_angle_counter_clock_wise_limit);
			ZDrive::estimated_port_servo_angle=(joystick_msg->axes[XBOX_R_STICK_LAT] * ZDrive::port_servo_angle_counter_clock_wise_limit);
		}
		else
		{
			ZDrive::estimated_starboard_servo_angle =-joystick_msg->axes[XBOX_R_STICK_LAT] * ZDrive::starboard_servo_angle_clock_wise_limit;
			ZDrive::estimated_port_servo_angle =-joystick_msg->axes[XBOX_R_STICK_LAT] * ZDrive::port_servo_angle_clock_wise_limit;
		}
	}

	// Note: we specificly check for these buttons lasts because we want to ensure that we can kill thrusters
	if(joystick_msg->buttons[XBOX_BTN]==1)
	{
		if(kill_thrusters==false)
		{
			ZDrive::kill_thrusters=true;
			ZDrive::current_dynamic_config.kill_thrusters=true;
			ZDrive::update_dynamic_param_server=true;

			// TODO: blink the lights on the xbox controler
			int light_command_execution_return = 1;//system("xboxdrvctl --slot=0 --led=14");
			if(light_command_execution_return<0)
			{
				ROS_WARN("Can't create shell (sh) process for xboxdrvctl");
			}
			else if(light_command_execution_return==0)
			{
				ROS_WARN("ERROR no command processor is available for: xboxdrvctl");
			}
			//
			// while the following is a hack, it is recommended to use because there is no c++ api for dynamic_reconfigure yet. See: http://goo.gl/u63HdR and http://wiki.ros.org/dynamic_reconfigure
			/*
			if((system(NULL)))
			{
				// 1 second timeout
				int return_status=system("rosrun dynamic_reconfigure dynparam set -t 1 /z_drive _control_method:=0");
				ROS_INFO("Command return status: %d", return_status);
			}
			else
			{
				ROS_WARN("No processor available to run the command");
			}
			*/
		}
	}
	else if(joystick_msg->buttons[XBOX_START]==1)
	{
		//re-enable the thrusters only if they were previoulsy killed otherwise don't waste time
		if(kill_thrusters==true)
		{
			ROS_INFO("Re-enabaling the thrusters since they were previously killed");
			ZDrive::kill_thrusters=false;
			ZDrive::current_dynamic_config.kill_thrusters=false;
			ZDrive::update_dynamic_param_server=true;
		}
	}

	return;
}

void ZDrive::dynamicReconfigCallBack(const z_drive::GainsConfig &config, uint32_t level)
{
	// the config message is passed by reference for speed so we can't directly set one equal to the other, hence this hack to make the code cleaner.
	ZDrive::current_dynamic_config=z_drive::GainsConfig(config);
	if(config.control_method!=ZDrive::control_method)
	{
		ZDrive::prev_control_method=ZDrive::control_method;
	}
	ZDrive::control_method=config.control_method;
	ZDrive::kill_thrusters=config.kill_thrusters;
	ZDrive::boat_mass=config.boat_mass;
	ZDrive::boat_inertia=config.boat_inertia;
	ZDrive::port_servo_x_offset=config.port_servo_x_offset;
	ZDrive::port_servo_y_offset=config.port_servo_y_offset;
	ZDrive::port_servo_z_offset=config.port_servo_z_offset;
	ZDrive::port_servo_gear_reduction=config.port_servo_gear_reduction;
	ZDrive::starboard_servo_x_offset=config.starboard_servo_x_offset;
	ZDrive::starboard_servo_y_offset=config.starboard_servo_y_offset;
	ZDrive::starboard_servo_z_offset=config.starboard_servo_z_offset;
	ZDrive::starboard_servo_gear_reduction=config.starboard_servo_gear_reduction;
	ZDrive::p_gain_x=config.p_gain_x;
	ZDrive::d_gain_x=config.d_gain_x;
	ZDrive::p_gain_y=config.p_gain_y;
	ZDrive::d_gain_y=config.d_gain_y;
	ZDrive::p_gain_theta_boat=config.p_gain_theta_boat;
	ZDrive::d_gain_theta_boat=config.d_gain_theta_boat;
	ZDrive::gain_error_force_x=config.gain_error_force_x;
	ZDrive::gain_error_force_y=config.gain_error_force_y;
	ZDrive::gain_error_moment_z=config.gain_error_moment_z;
	ZDrive::gain_thrusters_force=config.gain_thrusters_force;
	ZDrive::gain_deviation_equilibrum_servo_angle=config.gain_deviation_equilibrum_servo_angle;
	ZDrive::gain_deviation_changeof_servo_angle=config.gain_deviation_changeof_servo_angle;
	ZDrive::step_size=config.step_size;
	ZDrive::step_multiplier=config.step_multiplier;
	ZDrive::cost_count_max=config.cost_count_max;
	ZDrive::port_servo_angle_offset=config.port_servo_angle_offset;
	ZDrive::port_servo_angle_clock_wise_limit=config.port_servo_angle_clock_wise_limit;
	ZDrive::port_servo_angle_counter_clock_wise_limit=config.port_servo_angle_counter_clock_wise_limit;
	ZDrive::port_thruster_foward_limit=config.port_thruster_foward_limit;
	ZDrive::port_thruster_reverse_limit=config.port_thruster_reverse_limit;
	ZDrive::starboard_servo_angle_offset=config.starboard_servo_angle_offset;
	ZDrive::starboard_servo_angle_clock_wise_limit=config.starboard_servo_angle_clock_wise_limit;
	ZDrive::starboard_servo_angle_counter_clock_wise_limit=config.starboard_servo_angle_counter_clock_wise_limit;
	ZDrive::starboard_thruster_foward_limit=config.starboard_thruster_foward_limit;
	ZDrive::starboard_thruster_reverse_limit=config.starboard_thruster_reverse_limit;
	ZDrive::friction_coefficient_forward=config.friction_coefficient_forward;
	ZDrive::friction_coefficient_forward_reduction=config.friction_coefficient_forward_reduction;
	ZDrive::friction_coefficient_lateral=config.friction_coefficient_lateral;
	ZDrive::friction_coefficient_lateral_reduction=config.friction_coefficient_lateral_reduction;
	ZDrive::friction_coefficient_rotational=config.friction_coefficient_rotational;
	ZDrive::friction_coefficient_rotational_reduction=config.friction_coefficient_rotational_reduction;
	if(config.control_method==ZDrive::MANUAL_MODE) // manual mode based on parameters set in the dynamic reconfig
	{
		// note the angles in the dynamic reconfig are in degrees
		// Note the explicit casts to double since the dynamic reconfig values are int_t
		ZDrive::estimated_port_servo_angle=-(double)((M_PI*config.manual_port_servo_angle)/180.0);
		ZDrive::estimated_port_thruster_force=(double)config.manual_port_thruster_force;
		ZDrive::estimated_starboard_servo_angle=-(double)((M_PI*config.manual_starboard_servo_angle)/180.0);
		ZDrive::estimated_starboard_thruster_force=(double)config.manual_starboard_thruster_force;
	}
	else if(config.control_method==MANUAL_TO_REQUIRED)
	{
		ZDrive::force_bow_required=(double)config.manual_force_bow_required;
		ZDrive::force_port_required=(double)config.manual_force_port_required;
		ZDrive::moment_z_required=(double)config.manual_moment_z_required;
	}
	else if(config.control_method==WAYPOINT_DRIVING||config.control_method==COST_DBG_MODE)
	{
		ZDrive::x_desired = ZDrive::x_current;
		ZDrive::y_desired = ZDrive::y_current;
		ZDrive::yaw_desired = ZDrive::yaw_current;
		ZDrive::x_velocity_desired=0;
		ZDrive::y_velocity_desired=0;
		ZDrive::yaw_velocity_desired=0;
	}
}

void ZDrive::currentOdomCallBack(const nav_msgs::Odometry& odom_msg)
{
	// TWIST part of an odom message is relative to the BODY! frame!
	// POSE part of the odom message is relative to the WORLD frame!
	// odom_msg contains all the information about how our robot (base_link) currently relates to the world.
	ZDrive::odom_current=odom_msg;
	ZDrive::x_current=odom_msg.pose.pose.position.x;
	ZDrive::y_current=odom_msg.pose.pose.position.y;
	ZDrive::z_current=odom_msg.pose.pose.position.z;
	// extract the relevant information based on Quaternions
	tf::Quaternion q_temp;
	tf::quaternionMsgToTF(odom_msg.pose.pose.orientation, q_temp);
	tf::Matrix3x3(q_temp).getRPY(ZDrive::roll_current, ZDrive::pitch_current, ZDrive::yaw_current);
	// the velocity(s) needs to be relative to the world frame
	// first we will get the linear velocities relative to the body frame
	tf::Vector3 linear_body_velocity;
	tf::vector3MsgToTF(odom_msg.twist.twist.linear, linear_body_velocity);
	// the friction is relative to the body frame of reference not the world. Also as the velocity increases in a given direction the friction reduces.
	// 		Note: we must take into account the sign of the velocity in the equation c*v-m*sgn(v)*v^2
	ZDrive::friction_force_forward=ZDrive::friction_coefficient_forward*linear_body_velocity.x()-ZDrive::friction_coefficient_forward_reduction*(linear_body_velocity.x()<0.0?-1.0*pow(linear_body_velocity.x(),2):pow(linear_body_velocity.x(),2));
	ZDrive::friction_force_lateral=ZDrive::friction_coefficient_lateral*linear_body_velocity.y()-ZDrive::friction_coefficient_lateral_reduction*(linear_body_velocity.y()<0.0?-1.0*pow(linear_body_velocity.y(),2):pow(linear_body_velocity.y(),2));
	// now make the linear body velocities relative to the world frame
	tf::Vector3 linear_world_velocity = tf::Matrix3x3(q_temp) * linear_body_velocity;
	ZDrive::x_velocity_current=linear_world_velocity.x();
	ZDrive::y_velocity_current=linear_world_velocity.y();
	ZDrive::z_velocity_current=linear_world_velocity.z();
	// now the angular velocities need to be relative to the world frame
	// first we will get the angular velocities relative to the body frame
	tf::Vector3 angular_body_velocity;
	tf::vector3MsgToTF(odom_msg.twist.twist.angular, angular_body_velocity);
	// again the friction is relative to the body frame of reference not the world. Also as the velocity increases in a given direction the friction reduces.
	ZDrive::friction_force_rotational=ZDrive::friction_coefficient_rotational*angular_body_velocity.z()-ZDrive::friction_coefficient_rotational_reduction*(angular_body_velocity.z()<0.0?-1.0*pow(angular_body_velocity.z(),2):pow(angular_body_velocity.z(),2));
	// now make the angular body velocities relative to the world frame
	tf::Vector3 angular_world_velocity =tf::Matrix3x3(q_temp) * angular_body_velocity;
	ZDrive::roll_velocity_current=angular_world_velocity.x();
	ZDrive::pitch_velocity_current=angular_world_velocity.y();
	ZDrive::yaw_velocity_current=angular_world_velocity.z();

	if(desired_position_inited==false)
	{
		ZDrive::x_desired=ZDrive::x_current;
		ZDrive::y_desired=ZDrive::y_current;
		ZDrive::z_desired=ZDrive::z_current;
		ZDrive::x_velocity_desired=0;
		ZDrive::y_velocity_desired=0;
		ZDrive::z_velocity_desired=0;
		ZDrive::roll_velocity_desired=0;
		ZDrive::pitch_velocity_desired=0;
		ZDrive::yaw_velocity_desired=0;
		desired_position_inited=true;
	}

	return;
}

void ZDrive::desiredOdomCallBack(const  uf_common::PoseTwist desired_pose_twist)
{
	desired_position_inited=true;

	// TWIST part of an odom message is relative to the BODY! frame!
	// POSE part of the odom message is relative to the WORLD frame!
	// odom_msg contains all the information about how our robot (base_link) currently relates to the world.
	//ZDrive::odom_desired=odom_msg;
	ZDrive::x_desired=desired_pose_twist.pose.position.x;
	ZDrive::y_desired=desired_pose_twist.pose.position.y;
	ZDrive::z_desired=desired_pose_twist.pose.position.z;
	// extract the relevant information based on Quaternions
	tf::Quaternion q_temp;
	tf::quaternionMsgToTF(desired_pose_twist.pose.orientation, q_temp);
	tf::Matrix3x3(q_temp).getRPY(ZDrive::roll_desired, ZDrive::pitch_desired, ZDrive::yaw_desired);
	// the velocity(s) needs to be relative to the world frame
	// first we will get the linear velocities relative to the world frame
	tf::Vector3 linear_body_velocity;
	tf::vector3MsgToTF(desired_pose_twist.twist.linear, linear_body_velocity);
	tf::Vector3 linear_world_velocity = tf::Matrix3x3(q_temp) * linear_body_velocity;
	ZDrive::x_velocity_desired=linear_world_velocity.x();
	ZDrive::y_velocity_desired=linear_world_velocity.y();
	ZDrive::z_velocity_desired=linear_world_velocity.z();
	// now the angular velocities relative to the world frame
	tf::Vector3 angular_body_velocity;
	tf::vector3MsgToTF(desired_pose_twist.twist.angular, angular_body_velocity);
	tf::Vector3 angular_world_velocity =tf::Matrix3x3(q_temp) * angular_body_velocity;
	ZDrive::roll_velocity_desired=angular_world_velocity.x();
	ZDrive::pitch_velocity_desired=angular_world_velocity.y();
	ZDrive::yaw_velocity_desired=angular_world_velocity.z();
	return;
}

void ZDrive::publishDbgMsg(double user_defined_1=0, double user_defined_2=0, double user_defined_3=0, double user_defined_4=0, double user_defined_5=0)
{
	dbg_msg.forces.required_global.x=requiredForceX(ZDrive::x_current, ZDrive::x_desired, ZDrive::x_velocity_current, ZDrive::x_velocity_desired);
	dbg_msg.forces.required_global.y=requiredForceY(ZDrive::y_current, ZDrive::y_desired, ZDrive::y_velocity_current, ZDrive::y_velocity_desired);
	dbg_msg.forces.required_global.z=requiredMomentZ(ZDrive::yaw_current, ZDrive::yaw_desired, ZDrive::yaw_velocity_current, ZDrive::yaw_velocity_desired);
	dbg_msg.forces.required_local.bow=ZDrive::force_bow_required;
	dbg_msg.forces.required_local.port=ZDrive::force_port_required;
	dbg_msg.forces.required_local.yaw=ZDrive::moment_z_required;
	dbg_msg.forces.resultant_local.bow=resultantForceX(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);;
	dbg_msg.forces.resultant_local.port=resultantForceY(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);
	dbg_msg.forces.resultant_local.yaw=resultantMomentZ(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);

	dbg_msg.friction.forward.coefficient=ZDrive::friction_coefficient_forward;
	dbg_msg.friction.forward.coefficient_reduction=ZDrive::friction_coefficient_forward_reduction;
	dbg_msg.friction.forward.force=ZDrive::friction_force_forward;
	dbg_msg.friction.lateral.coefficient=ZDrive::friction_coefficient_lateral;
	dbg_msg.friction.lateral.coefficient_reduction=ZDrive::friction_coefficient_lateral_reduction;
	dbg_msg.friction.lateral.force=ZDrive::friction_force_lateral;
	dbg_msg.friction.rotational.coefficient=ZDrive::friction_coefficient_rotational;
	dbg_msg.friction.rotational.coefficient_reduction=ZDrive::friction_coefficient_rotational_reduction;
	dbg_msg.friction.rotational.force=ZDrive::friction_force_rotational;

	dbg_msg.gains.p.x=ZDrive::p_gain_x;
	dbg_msg.gains.p.y=ZDrive::p_gain_y;
	dbg_msg.gains.p.theta_boat=ZDrive::p_gain_theta_boat;
	dbg_msg.gains.d.x=ZDrive::d_gain_x;
	dbg_msg.gains.d.y=ZDrive::d_gain_y;
	dbg_msg.gains.d.theta_boat=ZDrive::d_gain_theta_boat;
	dbg_msg.gains.error.force_x=ZDrive::gain_error_force_x;
	dbg_msg.gains.error.force_y=ZDrive::gain_error_force_y;
	dbg_msg.gains.error.moment_z=ZDrive::gain_error_moment_z;
	dbg_msg.gains.deviation_changeof_servo_angle=ZDrive::gain_deviation_changeof_servo_angle;
	dbg_msg.gains.deviation_equilibrum_servo_angle=ZDrive::gain_deviation_equilibrum_servo_angle;
	dbg_msg.gains.thrusters_force=ZDrive::gain_thrusters_force;

	dbg_msg.limit.port_servo.clock_wise_limit=ZDrive::port_servo_angle_clock_wise_limit;
	dbg_msg.limit.port_servo.counter_clock_wise_limit=ZDrive::port_servo_angle_counter_clock_wise_limit;
	dbg_msg.limit.port_servo.gear_reduction=ZDrive::port_servo_gear_reduction;
	dbg_msg.limit.starboard_servo.clock_wise_limit=ZDrive::starboard_servo_angle_clock_wise_limit;
	dbg_msg.limit.starboard_servo.counter_clock_wise_limit=ZDrive::starboard_servo_angle_counter_clock_wise_limit;
	dbg_msg.limit.starboard_servo.gear_reduction=ZDrive::starboard_servo_gear_reduction;
	dbg_msg.limit.port_thruster.foward=ZDrive::port_thruster_foward_limit;
	dbg_msg.limit.port_thruster.reverse=ZDrive::port_thruster_reverse_limit;
	dbg_msg.limit.starboard_thruster.foward=ZDrive::starboard_thruster_foward_limit;
	dbg_msg.limit.starboard_thruster.reverse=ZDrive::starboard_thruster_reverse_limit;

	dbg_msg.odom.position.x.current = ZDrive::x_current;
	dbg_msg.odom.position.x.desired = ZDrive::x_desired;
	dbg_msg.odom.position.y.current = ZDrive::y_current;
	dbg_msg.odom.position.y.desired = ZDrive::y_desired;
	dbg_msg.odom.position.yaw.current = ZDrive::yaw_current;
	dbg_msg.odom.position.yaw.desired = ZDrive::yaw_desired;
	dbg_msg.odom.velocity.x_velocity.current = ZDrive::x_velocity_current;
	dbg_msg.odom.velocity.x_velocity.desired = ZDrive::x_velocity_desired;
	dbg_msg.odom.velocity.y_velocity.current = ZDrive::y_velocity_current;
	dbg_msg.odom.velocity.y_velocity.desired = ZDrive::y_velocity_desired;
	dbg_msg.odom.velocity.yaw_velocity.current = ZDrive::yaw_velocity_current;
	dbg_msg.odom.velocity.yaw_velocity.desired = ZDrive::yaw_velocity_desired;
	dbg_msg.odom.extra.z.current = ZDrive::z_current;
	dbg_msg.odom.extra.z.desired = z_desired;
	dbg_msg.odom.extra.roll.current = ZDrive::roll_current;
	dbg_msg.odom.extra.roll.desired = ZDrive::roll_desired;
	dbg_msg.odom.extra.pitch.current = ZDrive::pitch_current;
	dbg_msg.odom.extra.pitch.desired = ZDrive::pitch_desired;
	dbg_msg.odom.extra.z_velocity.current = ZDrive::z_velocity_current;
	dbg_msg.odom.extra.z_velocity.desired = ZDrive::z_velocity_desired;
	dbg_msg.odom.extra.roll_velocity.current = ZDrive::roll_velocity_current;
	dbg_msg.odom.extra.roll_velocity.desired = ZDrive::roll_velocity_desired;
	dbg_msg.odom.extra.pitch_velocity.current = ZDrive::pitch_velocity_current;
	dbg_msg.odom.extra.pitch_velocity.desired = ZDrive::pitch_velocity_desired;

	dbg_msg.offsets.port.angle_offset=ZDrive::port_servo_angle_offset;
	dbg_msg.offsets.port.x_offset=ZDrive::port_servo_x_offset;
	dbg_msg.offsets.port.y_offset=ZDrive::port_servo_y_offset;
	dbg_msg.offsets.port.z_offset=ZDrive::port_servo_z_offset;
	dbg_msg.offsets.starboard.angle_offset=ZDrive::starboard_servo_angle_offset;
	dbg_msg.offsets.starboard.x_offset=ZDrive::starboard_servo_x_offset;
	dbg_msg.offsets.starboard.y_offset=ZDrive::starboard_servo_y_offset;
	dbg_msg.offsets.starboard.z_offset=ZDrive::starboard_servo_z_offset;


	dbg_msg.propulsion_current.port.angle=ZDrive::current_port_servo_angle;
	dbg_msg.propulsion_current.port.force=ZDrive::current_port_thruster_force;
	dbg_msg.propulsion_current.starboard.angle=ZDrive::current_starboard_servo_angle;
	dbg_msg.propulsion_current.starboard.force=current_starboard_thruster_force;
	dbg_msg.propulsion_desired.port.angle=ZDrive::estimated_port_servo_angle;
	dbg_msg.propulsion_desired.port.force=ZDrive::estimated_port_thruster_force;
	dbg_msg.propulsion_desired.starboard.angle=ZDrive::estimated_starboard_servo_angle;
	dbg_msg.propulsion_desired.starboard.force=ZDrive::estimated_starboard_thruster_force;

	dbg_msg.runtime_data.active_control_method=ZDrive::control_method;
	dbg_msg.runtime_data.kill_wp=ZDrive::kill_action;
	dbg_msg.runtime_data.kill_thrusters=ZDrive::kill_thrusters;
	dbg_msg.runtime_data.previous_control_method=ZDrive::prev_control_method;
	dbg_msg.runtime_data.new_wp=ZDrive::new_action;

	dbg_msg.cost_value=calcCost(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);

	/*
	dbg_msg.control_method=ZDrive::control_method;
	dbg_msg.cost_value=calcCost(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);
	dbg_msg.current_port_servo_angle=ZDrive::current_port_servo_angle;
	dbg_msg.current_port_thruster_force=ZDrive::current_port_thruster_force;
	dbg_msg.current_starboard_servo_angle=ZDrive::current_starboard_servo_angle;
	dbg_msg.current_starboard_thruster_force=ZDrive::current_starboard_thruster_force;
	dbg_msg.estimated_port_servo_angle=ZDrive::estimated_port_servo_angle;
	dbg_msg.estimated_port_thruster_force=ZDrive::estimated_port_thruster_force;
	dbg_msg.estimated_starboard_servo_angle=ZDrive::estimated_starboard_servo_angle;
	dbg_msg.estimated_starboard_thruster_force=ZDrive::estimated_starboard_thruster_force;
	dbg_msg.required_force_bow=ZDrive::force_bow_required;
	dbg_msg.required_force_port=ZDrive::force_port_required;
	dbg_msg.required_moment_yaw=ZDrive::moment_z_required;
	dbg_msg.resultant_force_bow=resultantForceX(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);
	dbg_msg.resultant_force_port=resultantForceY(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);
	dbg_msg.resultant_moment_yaw=resultantMomentZ(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);
	dbg_msg.step=ZDrive::step;
	dbg_msg.x_current=ZDrive::x_current;
	dbg_msg.x_desired=ZDrive::x_desired;
	dbg_msg.x_velocity_current=ZDrive::x_velocity_current;
	dbg_msg.x_velocity_desired=ZDrive::x_velocity_desired;
	dbg_msg.y_current=ZDrive::y_current;
	dbg_msg.y_desired=ZDrive::y_desired;
	dbg_msg.y_velocity_current=ZDrive::y_velocity_current;
	dbg_msg.y_velocity_desired=ZDrive::y_velocity_desired;
	dbg_msg.z_current=ZDrive::z_current;
	dbg_msg.z_desired=ZDrive::z_desired;
	dbg_msg.z_velocity_current=ZDrive::z_velocity_current;
	dbg_msg.z_velocity_desired=ZDrive::z_velocity_desired;
	dbg_msg.yaw_current=ZDrive::yaw_current;
	dbg_msg.yaw_desired=ZDrive::yaw_desired;
	dbg_msg.yaw_velocity_current=ZDrive::yaw_velocity_current;
	dbg_msg.yaw_velocity_desired=ZDrive::yaw_velocity_desired;
	dbg_msg.roll_current=ZDrive::roll_current;
	dbg_msg.roll_desired=ZDrive::roll_desired;
	dbg_msg.roll_velocity_current=ZDrive::roll_velocity_current;
	dbg_msg.roll_velocity_desired=ZDrive::roll_velocity_desired;
	dbg_msg.pitch_current=ZDrive::pitch_current;
	dbg_msg.pitch_desired=ZDrive::pitch_desired;
	dbg_msg.pitch_velocity_current=ZDrive::pitch_velocity_current;
	dbg_msg.pitch_velocity_desired=ZDrive::pitch_velocity_desired;
	dbg_msg.friction_force_forward=ZDrive::friction_force_forward;
	dbg_msg.friction_force_lateral=ZDrive::friction_force_lateral;
	dbg_msg.friction_force_rotational=ZDrive::friction_force_rotational;
	*/
	dbg_msg.user_defined_1=user_defined_1;
	dbg_msg.user_defined_2=user_defined_2;
	dbg_msg.user_defined_3=user_defined_3;
	dbg_msg.user_defined_4=user_defined_4;
	dbg_msg.user_defined_5=user_defined_5;
	z_drive_dbg_pub.publish(dbg_msg);
}

void ZDrive::run()
{

	// odom_current.pose.pose.position for points x,y,z in the world frame
	// odom_current.pose.pose.orientation for Quaternion which need to be turned into rpy for rotation about an axis in the world frame
	// odom_current.twist.twist.linear for vx,vy,vz in the base frame
	// odom_current.twist.twist.angular for angular velocity in the base frame

	// we have to wait for the nodes to connect before we post the inital config message
	//Timeout of 5 seconds
	ros::Time start = ros::Time::now();
	ros::Duration timeout(5);
	while(dynamixel_config_full_pub.getNumSubscribers()==0 && (ros::Time::now() - start) < timeout)
	{
		//ROS_INFO("Waiting for ZDrive node and Dynamixel/Motor node(s) to connect");
	}
	if(dynamixel_config_full_pub.getNumSubscribers() == 0)
	{
		ROS_ERROR("Timed out, no subscribers present for dynamixel_config_full_pub; is the dynamixel_server running?");
	}
	if(dynamixel_config_position_pub.getNumSubscribers() == 0)
	{
		ROS_ERROR("Timed out, no subscribers present for dynamixel_config_position_pub; is the dynamixel_server running?");
	}
	if(thruster_config_pub.getNumSubscribers() == 0)
	{
		ROS_ERROR("Timed out, no subscribers present for thruster_config_pub; is the motor_control running?");
	}

	uf_common::PoseTwistStamped trajectory_msg;
	dynamixel_servo::DynamixelJointConfig dynamixel_position_msg;
	motor_control::thrusterNewtons thruster_config_msg;
	z_drive::BoatSimZDriveOutsideForce sim_msg;

	// fill in a full init message for the servos
	dynamixel_servo::DynamixelFullConfig dynamixel_init_config_msg;
	dynamixel_init_config_msg.control_mode=dynamixel_servo::DynamixelFullConfig::JOINT;
	dynamixel_init_config_msg.goal_position=(float)(M_PI);
	dynamixel_init_config_msg.moving_speed=12.25; // 12.25rad/s = 117rpm
	dynamixel_init_config_msg.torque_limit=0x03FF;//The range 0 to 1023 (0x3FF)
	dynamixel_init_config_msg.goal_acceleration=38.04;//rad/s^2

	// config the port servo initaly first
	dynamixel_init_config_msg.id=ZDrive::port_servo_id;
	//dynamixel_config_full_pub.publish(dynamixel_init_config_msg);
	// config the starboard servo initaly first
	dynamixel_init_config_msg.id=ZDrive::starboard_servo_id;
	//dynamixel_config_full_pub.publish(dynamixel_init_config_msg);

	// fill in a init message to the thrusters that kills them initaly
	thruster_config_msg.thrust=0;
	// config the port thruster initaly first
	thruster_config_msg.id=ZDrive::port_servo_id;
	thruster_config_pub.publish(thruster_config_msg);
	// config the starboard thruster initaly first
	thruster_config_msg.id=ZDrive::starboard_servo_id;
	thruster_config_pub.publish(thruster_config_msg);

	actionlib::SimpleActionServer<uf_common::MoveToAction> actionserver(n, "moveto", false);
	actionserver.start();

	// init the dynamic configs so we can update the dyn_param server (the reconfig server isn't fully active yet until this point in the code)
	reconfig_server.getConfigDefault(ZDrive::current_dynamic_config);
	reconfig_server.getConfigDefault(ZDrive::default_dynamic_config);

	// this is to get an idea of how long it takes to run the control algorithms
	double computational_time=0;

	ros::Rate loop_rate(ZDrive::update_rate); // Note: currently the update rate isn't dynamic like it should be, the node has to be shutdown and restarted for a new update rate to take affect
	while(ros::ok())
	{
		// reinit the timer to see how long this entire loop takes
		computational_time= ros::Time::now().toSec();

		// First process callbacks to get the most recent odom and position information
		ros::spinOnce();

		// see if there is a new action that we should execute
		if(actionserver.isNewGoalAvailable())
		{
			boost::shared_ptr<const uf_common::MoveToGoal> goal = actionserver.acceptNewGoal();
			desiredOdomCallBack(goal->posetwist);
			ZDrive::new_action=true;
		}
		else
		{
			ZDrive::new_action=false;
		}

		// Update Rviz joint_state(s) based on any new status data that was received in the callback
		// Note:: the names of these states are defined in the urdf files under the propagator_description node
		joint_state.header.stamp = ros::Time::now();
		joint_state.name.resize(4);
		joint_state.position.resize(4);
		joint_state.name[0] ="port_servo";
		joint_state.position[0]=ZDrive::current_port_servo_angle;
		joint_state.name[1] ="port_housing_to_prop";
		joint_state.position[1]=(ZDrive::current_port_thruster_force>=0)?(ZDrive::current_port_thruster_force/ZDrive::port_thruster_foward_limit):(-ZDrive::current_port_thruster_force/ZDrive::port_thruster_reverse_limit); // scale the thruster prismatic joint to 1 so it's easier to see
		joint_state.name[2] ="starboard_servo";
		joint_state.position[2]=ZDrive::current_starboard_servo_angle;
		joint_state.name[3]="starboard_housing_to_prop";
		joint_state.position[3]=ZDrive::current_starboard_thruster_force>=0?(ZDrive::current_starboard_thruster_force/ZDrive::starboard_thruster_foward_limit):(-ZDrive::current_starboard_thruster_force/ZDrive::starboard_thruster_reverse_limit); // scale the thruster prismatic joint to 1 so it's easier to see
		// publish the joint state
		joint_pub.publish(joint_state);

		// Based on the control mode run the required algorithms.
		switch(ZDrive::control_method)
		{
			case ZDrive::XBOX_ANALOG_ONLY: // xbox_analog_only
				z_drive_sim_pub.publish(z_drive::BoatSimZDriveOutsideForce());
				break;
			case ZDrive::XBOX_ANALOG_TO_REQUIRED: // xbox_analog_to_required
				minimizeCostFunction();
				z_drive_sim_pub.publish(z_drive::BoatSimZDriveOutsideForce());
				break;
			case ZDrive::WAYPOINT_DRIVING: //waypoint_driving
				minimizeCostFunction();
				z_drive_sim_pub.publish(z_drive::BoatSimZDriveOutsideForce());
				break;
			case ZDrive::MANUAL_MODE:
				// the work required for this mode is done in the dynamic "reconfig_callback"
				z_drive_sim_pub.publish(z_drive::BoatSimZDriveOutsideForce());
				break;
			case ZDrive::COST_DBG_MODE://COST_DBG_MODE
				// the work required for this mode is done in the dynamic "reconfig_callback"
				debugCostFunction();
				minimizeCostFunction();
				z_drive_sim_pub.publish(z_drive::BoatSimZDriveOutsideForce());
				break;
			case ZDrive::MANUAL_TO_REQUIRED:
				minimizeCostFunction();
				z_drive_sim_pub.publish(z_drive::BoatSimZDriveOutsideForce());
				break;
			case ZDrive::MIRROR_MODE_JACKSON:
				z_drive_sim_pub.publish(z_drive::BoatSimZDriveOutsideForce());
				break;
			case ZDrive::XBOX_ANALOG_ONLY_JACKSON:
				z_drive_sim_pub.publish(z_drive::BoatSimZDriveOutsideForce());
				break;
			case ZDrive::TESTING_MODE: // testing_mode
				ZDrive::force_bow_required=cos(yaw_current)*requiredForceX(ZDrive::x_current, ZDrive::x_desired, ZDrive::x_velocity_current, ZDrive::x_velocity_desired) + sin(yaw_current)*requiredForceY(ZDrive::y_current, ZDrive::y_desired, ZDrive::y_velocity_current, ZDrive::y_velocity_desired);
				ZDrive::force_port_required=-sin(yaw_current)*requiredForceX(ZDrive::x_current, ZDrive::x_desired, ZDrive::x_velocity_current, ZDrive::x_velocity_desired) + cos(yaw_current)*requiredForceY(ZDrive::y_current, ZDrive::y_desired, ZDrive::y_velocity_current, ZDrive::y_velocity_desired);
				ZDrive::moment_z_required=requiredMomentZ(ZDrive::yaw_current, ZDrive::yaw_desired, ZDrive::yaw_velocity_current, ZDrive::yaw_velocity_desired);
				sim_msg.outside_force_x=ZDrive::force_bow_required;
				sim_msg.outside_force_y=ZDrive::force_port_required;
				sim_msg.outside_force_z=0; // no force in the z direction
				sim_msg.outside_torque_r=0; // no torque in the roll direction
				sim_msg.outside_torque_p=0; // no otorque in the pitch direction
				sim_msg.outside_torque_y=ZDrive::moment_z_required;
				// so we can see the calculation start when we say so, only publish the message when the thrusters arn't killed.
				if(ZDrive::kill_thrusters==true)
				{
					z_drive_sim_pub.publish(z_drive::BoatSimZDriveOutsideForce());
				}
				else
				{
					z_drive_sim_pub.publish(sim_msg);
				}
				break;
			case ZDrive::EXTRA_9:
				debugCostFunction();
				minimizeCostFunction();
				z_drive_sim_pub.publish(z_drive::BoatSimZDriveOutsideForce());
				break;
			default: // not defined
				ROS_WARN("Unknown ZDrive::control_method: %d", ZDrive::control_method);
				break;
		}

		// publish a dynamixel_config_position message for the port servo based on the controller's (possibly) new estimates.
		// Note: the dynamixel_server is written such that it quickly disregards messages where there is no change required in the servo's position.
		// Note: this doen't hurt to do when the thrusters are killed, because the boat's position wont move.
		// Note: the multiplier and the offset account for how the servo is mounted, and the gearing represents any gearing between the servo on motor
		dynamixel_position_msg.id=ZDrive::port_servo_id;
		dynamixel_position_msg.goal_position=(float)(-1.0*ZDrive::estimated_port_servo_angle*port_servo_gear_reduction+ZDrive::port_servo_angle_offset);
		dynamixel_config_position_pub.publish(dynamixel_position_msg);
		dynamixel_position_msg.id=ZDrive::starboard_servo_id;
		dynamixel_position_msg.goal_position=(float)(-1.0*ZDrive::estimated_starboard_servo_angle*starboard_servo_gear_reduction+ZDrive::starboard_servo_angle_offset);
		dynamixel_config_position_pub.publish(dynamixel_position_msg);

		if(ZDrive::kill_thrusters==true)
		{
			// kill thrusters so the boat stops moving
			thruster_config_msg.id=ZDrive::port_servo_id;
			thruster_config_msg.thrust=0;
			thruster_config_pub.publish(thruster_config_msg);
			// do the same for starboard
			thruster_config_msg.id=ZDrive::starboard_servo_id;
			thruster_config_msg.thrust=0;
			thruster_config_pub.publish(thruster_config_msg);
			ROS_INFO("Z_DRIVE THRUSTERS KILLED");
		}
		else // thrusters arn't killed so, based on the algorithms that were just run, publish out to the thrusters
		{
			thruster_config_msg.id=ZDrive::port_servo_id;
			// clip the  thrust value if need be
			if(ZDrive::estimated_port_thruster_force>ZDrive::port_thruster_foward_limit)
			{
				ZDrive::estimated_port_thruster_force=ZDrive::port_thruster_foward_limit;
			}
			else if(ZDrive::estimated_port_thruster_force<ZDrive::port_thruster_reverse_limit)
			{
				ZDrive::estimated_port_thruster_force=ZDrive::port_thruster_reverse_limit;
			}
			else
			{
				thruster_config_msg.thrust=ZDrive::estimated_port_thruster_force;
			}
			thruster_config_pub.publish(thruster_config_msg);
			thruster_config_msg.id=ZDrive::starboard_servo_id;
			// clip the  thrust value if need be
			if (ZDrive::estimated_starboard_thruster_force > ZDrive::starboard_thruster_foward_limit)
			{
				ZDrive::estimated_starboard_thruster_force = ZDrive::starboard_thruster_foward_limit;
			}
			else if (ZDrive::estimated_starboard_thruster_force < ZDrive::starboard_thruster_reverse_limit)
			{
				ZDrive::estimated_starboard_thruster_force = ZDrive::starboard_thruster_reverse_limit;
			}
			else
			{
				thruster_config_msg.thrust = ZDrive::estimated_starboard_thruster_force;
			}
			thruster_config_pub.publish(thruster_config_msg);
		}

		// since the majority of the code in the main loop has finished stop timing
		computational_time=ros::Time::now().toSec()-computational_time;
		// Publish a dbg msg with everything that was just sent/evaluated/computed
		ZDrive::publishDbgMsg();

		/*
		if(ZDrive::new_action==true)
		{
			marker_data.header.frame_id="/base_link";
			marker_data.header.stamp=ros::Time::now();
			// Set the namespace and id for this marker.  This serves to create a unique ID
			// Any marker sent with the same namespace and id will overwrite the old one
			marker_data.ns="desired_position";
			// set what we want to do with the marker
			marker_data.action=visualization_msgs::Marker::ADD;
			marker_data.type=visualization_msgs::Marker::ARROW;

			marker_data.lifetime=ros::Duration();
			marker_data.pose.position.x=ZDrive::x_desired;
			marker_data.pose.position.y=ZDrive::y_desired;
			marker_data.pose.position.z=ZDrive::z_desired;
			//marker_data.pose.orientation.
		}
		*/


		// the main z_drive algorithm has now finished running, so do the required housekeeping tasks and update visual display information
		trajectory_msg.header.stamp=ros::Time::now();
		trajectory_msg.header.frame_id="/base_link";
		trajectory_msg.posetwist.pose.position.x=ZDrive::x_desired;
		trajectory_msg.posetwist.pose.position.y=ZDrive::y_desired;

		tf::Matrix3x3 m;
		m.setRPY(ZDrive::roll_desired, ZDrive::pitch_desired, ZDrive::yaw_desired);
		tf::Quaternion q_temp;
		m.getRotation(q_temp);
		tf::quaternionTFToMsg(q_temp, trajectory_msg.posetwist.pose.orientation);

		trajectory_pub.publish(trajectory_msg);

		// Inform the action server
		// if the euclidian norm is less than our tolerance, the action from the action server has succeed
		// TODO: add in the yaw component
		if(actionserver.isActive() && sqrt(pow(ZDrive::x_current-ZDrive::x_desired,2)+pow(ZDrive::y_current-ZDrive::y_desired,2))<.5)
		{
			actionserver.setSucceeded();
		}
		else if((actionserver.isActive() &&actionserver.isPreemptRequested())||(actionserver.isActive() && ZDrive::kill_action==true))
		{
			// if the user cancels the current action, perform the necessary cleanup.
			// stop as quickly as possible at your current position and kill the velocity
			ZDrive::x_desired = ZDrive::x_current;
			ZDrive::y_desired = ZDrive::y_current;
			ZDrive::yaw_desired = ZDrive::yaw_current;
			//
			ZDrive::x_velocity_desired=0;
			ZDrive::y_velocity_desired=0;
			ZDrive::yaw_velocity_desired=0;
			actionserver.setPreempted();
			ZDrive::kill_action=false;
		}

		// update the dynparam server and display of the dynparam server- if need be.
		if(ZDrive::update_dynamic_param_server==true)
		{
			reconfig_server.updateConfig(ZDrive::current_dynamic_config);
			ZDrive::update_dynamic_param_server=false;
		}



		loop_rate.sleep();
	}
}

// Note: in an ideal system the resultant force/moment would equal the required force/moment, but our system isn't ideal; thus we must calculate both.
//---------------------- this is the force/moment that can be provided to the system for a given configuration of the z_drive ---------------
double ZDrive::resultantForceX(double port_servo_angle,double port_thruster_force,double starboard_servo_angle,double starboard_thruster_force)
{
	return (port_thruster_force*cos(port_servo_angle) + starboard_thruster_force*cos(starboard_servo_angle));
}
double ZDrive::resultantForceY(double port_servo_angle, double port_thruster_force, double starboard_servo_angle, double starboard_thruster_force)
{
	return (port_thruster_force*sin(port_servo_angle) + starboard_thruster_force*sin(starboard_servo_angle));
}
double ZDrive::resultantMomentZ(double port_servo_angle, double port_thruster_force, double starboard_servo_angle, double starboard_thruster_force)
{
	return (port_servo_y_offset*port_thruster_force*cos(port_servo_angle) + starboard_servo_y_offset*starboard_thruster_force*cos(starboard_servo_angle) - port_servo_x_offset*port_thruster_force*sin(port_servo_angle) - starboard_servo_x_offset*starboard_thruster_force*sin(starboard_servo_angle));
}

//---------------------- this is what we want the system to do, but it may not be able to. ----------------
double ZDrive::requiredForceX(double x_current, double x_desired, double x_velocity_current, double x_velocity_desired)
{
	return p_gain_x*(x_desired-x_current)+d_gain_x*(x_velocity_desired-x_velocity_current);
}
double ZDrive::requiredForceY(double y_current, double y_desired, double y_velocity_current, double y_velocity_desired)
{
	return p_gain_y*(y_desired-y_current)+d_gain_y*(y_velocity_desired-y_velocity_current);
}
double ZDrive::requiredMomentZ(double boat_angle_current, double boat_angle_desired, double boat_angular_velocity_current, double boat_angular_velocity_desired)
{
	// this is the angle (in radians) of the boat relative to the world coordinate frame
	double temp_angle=boat_angle_desired-boat_angle_current;

	while(temp_angle<-M_PI)
	{
		temp_angle+=2*M_PI;
	}
	while(temp_angle>M_PI)
	{
		temp_angle-=2*M_PI;
	}


	return p_gain_theta_boat*(temp_angle)+d_gain_theta_boat*(boat_angular_velocity_desired-boat_angular_velocity_current);
}

//---------------------- these are the partial derivatives related to a given configuration of the z_drive system (with respect to the costValue/"cost function")  --------------------------
double ZDrive::dCost_dPortServoAngle(double port_servo_angle,double port_thruster_force,double starboard_servo_angle,double starboard_thruster_force)
{
	// remember according to ros rep 103: x=bow=forward, y=port=left in the boats frame of reference.
	return gain_deviation_equilibrum_servo_angle*port_servo_angle - gain_deviation_changeof_servo_angle*(current_port_servo_angle - port_servo_angle) + gain_error_moment_z*(port_servo_x_offset*port_thruster_force*cos(port_servo_angle) + port_servo_y_offset*port_thruster_force*sin(port_servo_angle))*(moment_z_required - port_servo_y_offset*port_thruster_force*cos(port_servo_angle) - starboard_servo_y_offset*starboard_thruster_force*cos(starboard_servo_angle) + port_servo_x_offset*port_thruster_force*sin(port_servo_angle) + starboard_servo_x_offset*starboard_thruster_force*sin(starboard_servo_angle)) - gain_error_force_x*port_thruster_force*sin(port_servo_angle)*(port_thruster_force*cos(port_servo_angle) - force_bow_required + starboard_thruster_force*cos(starboard_servo_angle)) + gain_error_force_y*port_thruster_force*cos(port_servo_angle)*(port_thruster_force*sin(port_servo_angle) - force_port_required + starboard_thruster_force*sin(starboard_servo_angle));
}
double ZDrive::dCost_dPortThrusterForce(double port_servo_angle,double port_thruster_force,double starboard_servo_angle,double starboard_thruster_force)
{
	// remember according to ros rep 103: x=bow=forward, y=port=left
	return gain_thrusters_force*port_thruster_force + gain_error_force_x*cos(port_servo_angle)*(port_thruster_force*cos(port_servo_angle) - force_bow_required + starboard_thruster_force*cos(starboard_servo_angle)) + gain_error_force_y*sin(port_servo_angle)*(port_thruster_force*sin(port_servo_angle) - force_port_required + starboard_thruster_force*sin(starboard_servo_angle)) - gain_error_moment_z*(port_servo_y_offset*cos(port_servo_angle) - port_servo_x_offset*sin(port_servo_angle))*(moment_z_required - port_servo_y_offset*port_thruster_force*cos(port_servo_angle) - starboard_servo_y_offset*starboard_thruster_force*cos(starboard_servo_angle) + port_servo_x_offset*port_thruster_force*sin(port_servo_angle) + starboard_servo_x_offset*starboard_thruster_force*sin(starboard_servo_angle));
}
double ZDrive::dCost_dStarboardServoAngle(double port_servo_angle,double port_thruster_force,double starboard_servo_angle,double starboard_thruster_force)
{
	// remember according to ros rep 103: x=bow=forward, y=port=left
	return gain_deviation_equilibrum_servo_angle*starboard_servo_angle - gain_deviation_changeof_servo_angle*(current_starboard_servo_angle - starboard_servo_angle) + gain_error_moment_z*(starboard_servo_x_offset*starboard_thruster_force*cos(starboard_servo_angle) + starboard_servo_y_offset*starboard_thruster_force*sin(starboard_servo_angle))*(moment_z_required - port_servo_y_offset*port_thruster_force*cos(port_servo_angle) - starboard_servo_y_offset*starboard_thruster_force*cos(starboard_servo_angle) + port_servo_x_offset*port_thruster_force*sin(port_servo_angle) + starboard_servo_x_offset*starboard_thruster_force*sin(starboard_servo_angle)) - gain_error_force_x*starboard_thruster_force*sin(starboard_servo_angle)*(port_thruster_force*cos(port_servo_angle) - force_bow_required + starboard_thruster_force*cos(starboard_servo_angle)) + gain_error_force_y*starboard_thruster_force*cos(starboard_servo_angle)*(port_thruster_force*sin(port_servo_angle) - force_port_required + starboard_thruster_force*sin(starboard_servo_angle));
}
double ZDrive::dCost_dStarboardThrusterForce(double port_servo_angle,double port_thruster_force,double starboard_servo_angle,double starboard_thruster_force)
{
	// remember according to ros rep 103: x=bow=forward, y=port=left
	return gain_thrusters_force*starboard_thruster_force + gain_error_force_x*cos(starboard_servo_angle)*(port_thruster_force*cos(port_servo_angle) - force_bow_required + starboard_thruster_force*cos(starboard_servo_angle)) + gain_error_force_y*sin(starboard_servo_angle)*(port_thruster_force*sin(port_servo_angle) - force_port_required + starboard_thruster_force*sin(starboard_servo_angle)) - gain_error_moment_z*(starboard_servo_y_offset*cos(starboard_servo_angle) - starboard_servo_x_offset*sin(starboard_servo_angle))*(moment_z_required - port_servo_y_offset*port_thruster_force*cos(port_servo_angle) - starboard_servo_y_offset*starboard_thruster_force*cos(starboard_servo_angle) + port_servo_x_offset*port_thruster_force*sin(port_servo_angle) + starboard_servo_x_offset*starboard_thruster_force*sin(starboard_servo_angle));
}

//---------------------- calcCost function calculates the given "cost" that we have assigned to the system for a given configuration of the z_drive. --------------
double ZDrive::calcCost(double port_servo_angle, double port_thruster_force, double starboard_servo_angle, double starboard_thruster_force)
{
	// Note: Doubles are stored in ieee754 format (thus having a sign, exponent, and fraction section), so binary division and multiplication can't be applied here- for optimization.
	return
			gain_error_force_x/2               		* pow((resultantForceX(port_servo_angle, port_thruster_force, starboard_servo_angle, starboard_thruster_force)-force_bow_required),2) +
			gain_error_force_y/2                    * pow((resultantForceY(port_servo_angle, port_thruster_force, starboard_servo_angle, starboard_thruster_force)-force_port_required),2) +
			gain_error_moment_z/2                   * pow((resultantMomentZ(port_servo_angle, port_thruster_force, starboard_servo_angle, starboard_thruster_force)-moment_z_required),2)  +
			gain_thrusters_force/2                  * pow(port_thruster_force,2) +
			gain_thrusters_force/2                  * pow(starboard_thruster_force,2) +
			gain_deviation_equilibrum_servo_angle/2 * pow(port_servo_angle,2) +
			gain_deviation_equilibrum_servo_angle/2 * pow(starboard_servo_angle,2) +
			gain_deviation_changeof_servo_angle/2   * pow((port_servo_angle - current_port_servo_angle),2) +
			gain_deviation_changeof_servo_angle/2   * pow((starboard_servo_angle - current_starboard_servo_angle),2);
			/*
			(gain_deviation_changeof_servo_angle*(pow(current_port_servo_angle - port_servo_angle,2) + pow(current_starboard_servo_angle - starboard_servo_angle,2)))/2+
			(gain_error_force_x*pow((force_bow_required - resultantForceX(ZDrive::x_current,ZDrive::x_desired,ZDrive::x_velocity_current,ZDrive::x_velocity_desired)),2))/2 +
			(gain_thrusters_force*(pow(port_thruster_force,2) + pow(starboard_thruster_force,2)))/2 +
			(gain_deviation_equilibrum_servo_angle*(pow(port_servo_angle,2) + pow(starboard_servo_angle,2)))/2 +
			(gain_error_force_y*pow((force_port_required - resultantForceY(ZDrive::y_current,ZDrive::y_desired,ZDrive::y_velocity_current,ZDrive::y_velocity_desired)),2))/2 +
			(gain_error_moment_z*pow((moment_z_required - resultantMomentZ(ZDrive::yaw_current,ZDrive::yaw_desired,ZDrive::yaw_velocity_current,ZDrive::yaw_velocity_desired)),2))/2;
			*/
}

void ZDrive::guessInitalValues()
{
	// this is for future use to allow someone to create a good initial guess on what the first servo angle should be
	ZDrive::estimated_port_servo_angle=ZDrive::current_port_servo_angle;
	ZDrive::estimated_port_thruster_force=ZDrive::current_port_thruster_force;
	ZDrive::estimated_starboard_servo_angle=ZDrive::current_starboard_servo_angle;
	ZDrive::estimated_starboard_thruster_force=ZDrive::current_starboard_thruster_force;

#ifdef ZDRIVE_DBG
	// when we are debugging we don't want these optimized out, hence they will be declared as volatile
	volatile double dbg_tmp_cost, dbg_est_cost;
	volatile double temp_theta_port, temp_theta_starboard, temp_force_port, temp_force_starboard;
#else
	// the compiler will optimize out the expressions below that are using these variables
	double dbg_tmp_cost, dbg_est_cost;
	double temp_theta_port, temp_theta_starboard, temp_force_port, temp_force_starboard;
#endif
	std::vector<double>::iterator i;
	std::vector<double>::iterator j;
	std::vector<double>::iterator k;
	std::vector<double>::iterator l;

	for (i = ZDrive::inital_theta_port.begin(); i!=ZDrive::inital_theta_port.end(); i++)
	{
		for (j = ZDrive::inital_thrust_port.begin(); j!=ZDrive::inital_thrust_port.end(); j++)
		{
			for (k = ZDrive::inital_theta_starboard.begin(); k!=ZDrive::inital_theta_starboard.end(); k++)
			{
				for (l = ZDrive::inital_thrust_starboard.begin(); l!=ZDrive::inital_thrust_starboard.end(); l++)
				{
					temp_theta_port=*i;
					temp_force_port=*j;
					temp_theta_starboard=*k;
					temp_force_starboard=*l;
#ifdef ZDRIVE_DBG
					// the cost function calculates the resultant force, so this is for easy debuging
					volatile double temp_resultant_force_x=resultantForceX(temp_theta_port, temp_force_port, temp_theta_starboard, temp_force_starboard);
					volatile double temp_resultant_force_y=resultantForceY(temp_theta_port, temp_force_port, temp_theta_starboard, temp_force_starboard);
					volatile double temp_resultant_moment_z=resultantMomentZ(temp_theta_port, temp_force_port, temp_theta_starboard, temp_force_starboard);
#endif

					dbg_tmp_cost=calcCost(temp_theta_port, temp_force_port, temp_theta_starboard, temp_force_starboard);
					dbg_est_cost=calcCost(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);
					if (dbg_tmp_cost< dbg_est_cost)
					{
						ZDrive::estimated_port_servo_angle = temp_theta_port;
						ZDrive::estimated_port_thruster_force = temp_force_port;
						ZDrive::estimated_starboard_servo_angle = temp_theta_starboard;
						ZDrive::estimated_starboard_thruster_force = temp_force_starboard;
						//publishDbgMsg(temp_theta_port,temp_force_port,temp_theta_starboard ,temp_force_starboard);
					}


				}
			}
		}
	}
}

void ZDrive::checkEstimatedValuesAgainsBounds()
{
	// if any bound exceeds the accepted limits, reset it to the corresponding limit
	if(ZDrive::estimated_port_servo_angle<ZDrive::port_servo_angle_clock_wise_limit)
	{
		ZDrive::estimated_port_servo_angle=ZDrive::port_servo_angle_clock_wise_limit;
	}
	else if(ZDrive::estimated_port_servo_angle>ZDrive::port_servo_angle_counter_clock_wise_limit)
	{
		ZDrive::estimated_port_servo_angle=ZDrive::port_servo_angle_counter_clock_wise_limit;
	}
	if(ZDrive::estimated_port_thruster_force<ZDrive::port_thruster_reverse_limit)
	{
		ZDrive::estimated_port_thruster_force=ZDrive::port_thruster_reverse_limit;
	}
	else if(ZDrive::estimated_port_thruster_force>ZDrive::port_thruster_foward_limit)
	{
		ZDrive::estimated_port_thruster_force=ZDrive::port_thruster_foward_limit;
	}

	if (ZDrive::estimated_starboard_servo_angle < ZDrive::starboard_servo_angle_clock_wise_limit)
	{
		ZDrive::estimated_starboard_servo_angle = ZDrive::starboard_servo_angle_clock_wise_limit;
	}
	else if (ZDrive::estimated_starboard_servo_angle > ZDrive::starboard_servo_angle_counter_clock_wise_limit)
	{
		ZDrive::estimated_starboard_servo_angle = ZDrive::starboard_servo_angle_counter_clock_wise_limit;
	}
	if (ZDrive::estimated_starboard_thruster_force < ZDrive::starboard_thruster_reverse_limit)
	{
		ZDrive::estimated_starboard_thruster_force = ZDrive::starboard_thruster_reverse_limit;
	}
	else if (ZDrive::estimated_starboard_thruster_force > ZDrive::starboard_thruster_foward_limit)
	{
		ZDrive::estimated_starboard_thruster_force = ZDrive::starboard_thruster_foward_limit;
	}
}

// After this function runs the final solution that is estimated is closer and closer to the optimal solution
// Note: The initial call to this function assumes that a guess has been made for the respective trust and angle measurements; this function was designed to converge towards the optimal solution with each iteration.
void ZDrive::calcAngleAndThrustEstimate()
{

	double partial_derivative_port_angle_result=dCost_dPortServoAngle(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);
	double partial_derivative_port_thrust_result=dCost_dPortThrusterForce(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);
	double partial_derivative_starboard_angle_result=dCost_dStarboardServoAngle(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);
	double partial_derivative_starboard_thrust_result=dCost_dStarboardThrusterForce(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);

	// init the cost variables. so innately best_cost_value=the current config of the system, and new_cost_value will be another hypothesized config that might be more optimal.
	// the cost cost_values/cost_function can be very oscillatory, so the goal is to get the best results even if it is oscillating, hence the following variables
	double best_cost_value=calcCost(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);
	double new_cost_value=DBL_MAX; // initalize it to the maximum allowed value since initaly it isnt better than anything
	double best_step=0;
	double best_port_servo_angle=ZDrive::estimated_port_servo_angle;
	double best_port_thruster_force=ZDrive::estimated_port_thruster_force;
	double best_starboard_servo_angle=ZDrive::estimated_starboard_servo_angle;
	double best_starboard_thruster_force=ZDrive::estimated_starboard_thruster_force;
	int cost_count=0;// this is essentialy how many times have we seen results that are worse than previously calculated values


	// this is in a for loop to ensure there isn't an endless loop while finding the best value for the step size- that we will return.
	// so the goal is to run the for loop until we find the first cost value that isn't decreasing in a given buffer range
	for (double i=0; i<=.01; i+=ZDrive::step_size )
	{
		ZDrive::step=i;
		ZDrive::estimated_port_servo_angle=ZDrive::estimated_port_servo_angle-(ZDrive::step*partial_derivative_port_angle_result);
		ZDrive::estimated_port_thruster_force=ZDrive::estimated_port_thruster_force-(ZDrive::step*partial_derivative_port_thrust_result);
		ZDrive::estimated_starboard_servo_angle=ZDrive::estimated_starboard_servo_angle-(ZDrive::step*partial_derivative_starboard_angle_result);
		ZDrive::estimated_starboard_thruster_force=ZDrive::estimated_starboard_thruster_force-(ZDrive::step*partial_derivative_starboard_thrust_result);
		new_cost_value=calcCost(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);

		if(new_cost_value<=best_cost_value)
		{
			best_cost_value=new_cost_value;
			best_step=ZDrive::step;
			best_port_servo_angle=ZDrive::estimated_port_servo_angle;
			best_port_thruster_force=ZDrive::estimated_port_thruster_force;
			best_starboard_servo_angle=ZDrive::estimated_starboard_servo_angle;
			best_starboard_thruster_force=ZDrive::estimated_starboard_thruster_force;
			cost_count=0;
		}
		else
		{
			cost_count++;
		}

		if(cost_count>=ZDrive::cost_count_max)
		{
			cost_count=0; //reinitalize it for the next loop to just use it
			break; //exit the for loop
		}
	}

	// Again we will bound the loop so it isn't an infinate loop
	// Now work backwards from where we got the best_cost_value by a VERY SMALL amount (ZDrive::step_multiplier times smaller) to try to find the min value thats less.
	for (double i=best_step; i>=0; i-=(ZDrive::step_size/ZDrive::step_multiplier) )
	{
		ZDrive::step=i;
		ZDrive::estimated_port_servo_angle=ZDrive::estimated_port_servo_angle-(ZDrive::step*partial_derivative_port_angle_result);
		ZDrive::estimated_port_thruster_force=ZDrive::estimated_port_thruster_force-(ZDrive::step*partial_derivative_port_thrust_result);
		ZDrive::estimated_starboard_servo_angle=ZDrive::estimated_starboard_servo_angle-(ZDrive::step*partial_derivative_starboard_angle_result);
		ZDrive::estimated_starboard_thruster_force=ZDrive::estimated_starboard_thruster_force-(ZDrive::step*partial_derivative_starboard_thrust_result);
		new_cost_value=calcCost(ZDrive::estimated_port_servo_angle, ZDrive::estimated_port_thruster_force, ZDrive::estimated_starboard_servo_angle, ZDrive::estimated_starboard_thruster_force);
		if(new_cost_value<=best_cost_value)
		{
			best_cost_value=new_cost_value;
			best_step=ZDrive::step;
			best_port_servo_angle=ZDrive::estimated_port_servo_angle;
			best_port_thruster_force=ZDrive::estimated_port_thruster_force;
			best_starboard_servo_angle=ZDrive::estimated_starboard_servo_angle;
			best_starboard_thruster_force=ZDrive::estimated_starboard_thruster_force;
			cost_count=0;
		}
		else
		{
			cost_count++;
		}

		if(cost_count>=ZDrive::cost_count_max)
		{
			cost_count=0; //reinitalize it for the next loop to just use it
			break; //exit the for loop
		}
	}

	// Now again set it back to what that best solution was that we found
	ZDrive::step=best_step;
	ZDrive::estimated_port_servo_angle=best_port_servo_angle;
	ZDrive::estimated_port_thruster_force=best_port_thruster_force;
	ZDrive::estimated_starboard_servo_angle=best_starboard_servo_angle;
	ZDrive::estimated_starboard_thruster_force=best_starboard_thruster_force;

	// lastly verify that the estimates we calculated are within bounds. Note: checkEstimatedValuesAgainsBounds will fix to their corresponding limit if they are not.
	checkEstimatedValuesAgainsBounds();
}

void ZDrive::debugCostFunction()
{
	// this function essentially sets things manually so it is easier to debug in eclipse and we don't have to worry about callback(s) happening
	/*
	ZDrive::x_current=0;
	ZDrive::y_current=0;
	ZDrive::z_current=0;
	ZDrive::x_velocity_current=0;
	ZDrive::y_velocity_current=0;
	ZDrive::z_velocity_current=0;
	ZDrive::roll_current=0;
	ZDrive::pitch_current=0;
	ZDrive::yaw_current=0;
	ZDrive::roll_velocity_current=0;
	ZDrive::pitch_velocity_current=0;
	ZDrive::yaw_velocity_current=0;
	*/
	/*
	ZDrive::x_desired=0;
	ZDrive::y_desired=10;
	ZDrive::z_desired=0;
	ZDrive::x_velocity_desired=0;
	ZDrive::y_velocity_desired=0;
	ZDrive::z_velocity_desired=0;
	ZDrive::roll_desired=0;
	ZDrive::pitch_desired=0;
	ZDrive::yaw_desired=M_PI;
	ZDrive::roll_velocity_desired=0;
	ZDrive::pitch_velocity_desired=0;
	ZDrive::yaw_velocity_desired=0;
	*/
	ZDrive::friction_coefficient_forward=0;
	ZDrive::friction_coefficient_forward_reduction=0;
	ZDrive::friction_coefficient_lateral=0;
	ZDrive::friction_coefficient_lateral_reduction=0;
	ZDrive::friction_coefficient_rotational=0;
	ZDrive::friction_coefficient_rotational_reduction=0;
	/*
	ZDrive::current_port_servo_angle=0;
	ZDrive::current_port_thruster_force=0;
	ZDrive::estimated_port_servo_angle=0;
	ZDrive::estimated_port_thruster_force=0;
	ZDrive::current_starboard_servo_angle=0;
	ZDrive::current_starboard_thruster_force=0;
	ZDrive::estimated_starboard_servo_angle=0;
	ZDrive::estimated_starboard_thruster_force=0;
	*/
	ROS_DEBUG("z_drive node has debugging active. NOTE: this means the current and desired positions are overwritten.");
}


void ZDrive::minimizeCostFunction()
{

	// we must first get the required forces before we can create an initial guess for the solution to minimize the cost function. (remember we may not be able to archive this in the end, hence the resultant force)
	if(ZDrive::control_method!=XBOX_ANALOG_TO_REQUIRED && ZDrive::control_method!=MANUAL_TO_REQUIRED)
	{
#ifdef ZDRIVE_DBG
		volatile double temp_required_force_x=requiredForceX(ZDrive::x_current, ZDrive::x_desired, ZDrive::x_velocity_current, ZDrive::x_velocity_desired);
		volatile double temp_required_force_y=requiredForceY(ZDrive::y_current, ZDrive::y_desired, ZDrive::y_velocity_current, ZDrive::y_velocity_desired);
		volatile double temp_moment_z_required=requiredMomentZ(ZDrive::yaw_current, ZDrive::yaw_desired, ZDrive::yaw_velocity_current, ZDrive::yaw_velocity_desired);
#endif
		ZDrive::force_bow_required=cos(yaw_current)*requiredForceX(ZDrive::x_current, ZDrive::x_desired, ZDrive::x_velocity_current, ZDrive::x_velocity_desired) + sin(yaw_current)*requiredForceY(ZDrive::y_current, ZDrive::y_desired, ZDrive::y_velocity_current, ZDrive::y_velocity_desired);
		ZDrive::force_port_required=-  sin(yaw_current)*requiredForceX(ZDrive::x_current, ZDrive::x_desired, ZDrive::x_velocity_current, ZDrive::x_velocity_desired) + cos(yaw_current)*requiredForceY(ZDrive::y_current, ZDrive::y_desired, ZDrive::y_velocity_current, ZDrive::y_velocity_desired);
		ZDrive::moment_z_required=requiredMomentZ(ZDrive::yaw_current, ZDrive::yaw_desired, ZDrive::yaw_velocity_current, ZDrive::yaw_velocity_desired);
		// these forces need to be turned into the boat coordinate system
	}

	// Intelligently guess and initialize values for the respective thrusts and angles. This must be done before we can calculate estimates.
	guessInitalValues();

	/*
	for(int i=0;i<40;i++)
	{
		// each time we calculate an estimate again we get closer and closer to the optimal solution for the desired position
		calcAngleAndThrustEstimate();
		//publishDbgMsg((double)i);
	}
	*/

}

#endif
