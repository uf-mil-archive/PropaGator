#include <ros/ros.h>
#include "dynamixel_servo/DynamixelFullConfig.h"
#include "dynamixel_servo/DynamixelJointConfig.h"
#include "dynamixel_servo/DynamixelStatus.h"

ros::Publisher dynamixel_config_full_pub;
ros::Publisher dynamixel_config_position_pub;
//Min 150.1171		2.620039
//Max 222.8027		3.888641
const float MIN_ANGLE = 2.625;//2.357;
const float MAX_ANGLE = 3.14159;//3.891;

int dir = 1;

int main(int argc, char** argv)
{
	//Initilize ROS
    ros::init(argc, argv, "lidar_control_node");
    ros::NodeHandle n;
    ros::Rate sleep_time(1.5);

    //Initilze publishers
    dynamixel_config_full_pub = n.advertise<dynamixel_servo::DynamixelFullConfig>("/dynamixel/dynamixel_config_full", 1000);
    dynamixel_config_position_pub = n.advertise<dynamixel_servo::DynamixelJointConfig>("/dynamixel/dynamixel_config_position", 1000);

    while(ros::ok())
    {
    	dir *= -1;

    	dynamixel_servo::DynamixelJointConfig msg;
    	msg.id = 1;
    	if(dir == 1)
    		msg.goal_position = MAX_ANGLE;
    	else
    		msg.goal_position = MIN_ANGLE;

    	dynamixel_config_position_pub.publish(msg);

		sleep_time.sleep();
		ros::spinOnce();
    }

    return 0;
}
