#ifndef LIDAR_ANGLE_MANAGER_H_
#define LIDAR_ANGLE_MANAGER_H_

#include <ros/ros.h>
#include "dynamixel_servo/DynamixelFullConfig.h"
#include "dynamixel_servo/DynamixelJointConfig.h"
#include "dynamixel_servo/DynamixelStatus.h"
#include "dynamixel_servo/DynamixelControlTableRequest.h"
#include "dynamixel_servo/DynamixelControlTableParam.h"

class LidarAngleManager
{
	/*
	 *  Private vars
	 */
private:
	//Publishers
	ros::Publisher dynamixel_config_full_pub_;
	ros::Publisher dynamixel_config_position_pub_;
	ros::Publisher dynamixel_control_table_pub_;

	//Subscribers
	ros::Subscriber dynamixel_control_table_sub_;
	
	//min and max angle
	float max_angle_, min_angle_;
	
	//Servo id
	int servo_id_;
	
	//Current angle
	float current_angle_;
	
	//Transform
	
	/*
	 *  Private functions
	 */
private:
	//Conversion between dynamixel setting and radians
	float DynamixelToRads(int dynamixelSetting);

	//Get the abs max and min angles
	void GetLimits(const dynamixel_servo::DynamixelControlTableParam config);
	
	/*
	 * 	Public functions
	 */
public:
	//Constructor
	LidarAngleManager();
	
	//Destructor
	//~LidarAngleManager();
	
	//Run the node
	void run();
};

/*
 *  Constructor
 */
LidarAngleManager::LidarAngleManager():
		current_angle_(3.14159), max_angle_(3.14159), min_angle_(2.625)
{
	
}

/*
 * 	Runs the node
 */
void LidarAngleManager::run()
{
	/*
	 * 			Initilize ROS
	 */
	//Initilze the node ("lidar_control_node")
	//Initilze the node handle
	ros::NodeHandle n;
	ros::Rate sleep_time(1.5);
	    
	//Initilze publishers
	ros::Publisher dynamixel_config_full_pub_ = n.advertise<dynamixel_servo::DynamixelFullConfig>("/dynamixel/dynamixel_config_full", 10);
	ros::Publisher dynamixel_config_position_pub_ = n.advertise<dynamixel_servo::DynamixelJointConfig>("/dynamixel/dynamixel_config_position", 10);
	ros::Publisher dynamixel_control_table_pub_ = n.advertise<dynamixel_servo::DynamixelControlTableRequest>("/dynamixel/dynamixel_control_table_request", 10);

	//Initilze the subscribers
	ros::Subscriber dynamixel_control_table_sub_ = n.subscribe("/dynamixel/dynamixel_control_table_post", 1, &LidarAngleManager::GetLimits, this);
	
	/*
	 * 		Initilize the parameters
	 */
	//Holds the servo to use to control the lidar
	//<param name="$(arg servo_namespace)/lidar/bow/id" value="1" type="int"/>
	n.param<int>("servo/lidar/bow/id", servo_id_, 1);
	
	//Holds the maximum angle (rad) to set the lidar to
	//n.param<float>("lidar/max_angle", max_angle_, 3.14159);
	//Holds the minimum angle (rad) to set the lidar to
	//n.param<float>("lidar/min_angle", min_angle_, 2.625);
	//Holds the angular speed (rad/s)
	//n.param<float>

	
	//Wait for subscriber on control table request
	//Timeout of 5 seconds
	ros::Time start = ros::Time::now();
	ros::Duration timeout(5);
	while(dynamixel_control_table_pub_.getNumSubscribers() == 0 && (ros::Time::now() - start) < timeout);
	if(dynamixel_control_table_pub_.getNumSubscribers() !=0)
	{
		//Send out request for control table
		dynamixel_servo::DynamixelControlTableRequest request;
		request.id = servo_id_;
		dynamixel_control_table_pub_.publish(request);
		//When the data comes in it'll reset the min and max
	}
	else
	{
		ROS_WARN("Failed to talk to dynamixel server. No subscribers to /dynamixel/dynamixel_control_table_request");
	}
	
	//Configure lidar servo settings
	//TODO
	
	

	/*
	 *  Other
	 */
	int dir = 1;

	/*
	 * 			Main loop
	 */
	while(ros::ok())
	{
		dir *= -1;

		dynamixel_servo::DynamixelJointConfig msg;
		msg.id = servo_id_;

		if(dir == 1)
			msg.goal_position = max_angle_;
		else
			msg.goal_position = min_angle_;

		dynamixel_config_position_pub_.publish(msg);

		sleep_time.sleep();
		ros::spinOnce();
	}
}

/*
 *  Conversion dynamixel setting * 360 / 4095 = degrees, degrees * pi / 180 = radians
 */
inline float LidarAngleManager::DynamixelToRads(int dynamixelSetting)
{
	return (dynamixelSetting * 360 * 3.14159) / (4095 * 180);
}

/*
 * Get the limits from the servo server
 */
void LidarAngleManager::GetLimits(const dynamixel_servo::DynamixelControlTableParam config)
{
	//Check to make sure we got the correct servo
	if(config.id == servo_id_)
	{
		max_angle_ = DynamixelToRads(config.cw_angle_limit);
		min_angle_ = DynamixelToRads(config.ccw_angle_limit);
	}
	else	//Otherwise request servo id again
	{
		//Send out request for control table
		dynamixel_servo::DynamixelControlTableRequest request;
		request.id = servo_id_;
		dynamixel_control_table_pub_.publish(request);
	}
}

#endif