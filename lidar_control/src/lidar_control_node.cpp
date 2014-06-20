#include <ros/ros.h>
#include "dynamixel_servo/DynamixelConfigParam.h"
#include "dynamixel_servo/DynamixelConfigPosition.h"
#include "dynamixel_servo/DynamixelStatusParam.h"

ros::Publisher dynamixel_config_full_pub;
ros::Publisher dynamixel_config_position_pub;

const float MIN_ANGLE = 2.357;
const float MAX_ANGLE = 3.891;

int dir = 1;

int main(int argc, char** argv)
{
	//Initilize ROS
    ros::init(argc, argv, "lidar_control_node");
    ros::NodeHandle n;
    ros::Rate sleep_time(1);

    //Initilze publishers
    dynamixel_config_full_pub = n.advertise<dynamixel_servo::DynamixelConfigParam>("/dynamixel/dynamixel_config_full", 1000);
    dynamixel_config_position_pub = n.advertise<dynamixel_servo::DynamixelConfigPosition>("/dynamixel/dynamixel_config_position", 1000);

    while(ros::ok())
    {
    	dir *= -1;

    	dynamixel_servo::DynamixelConfigPosition msg;
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
