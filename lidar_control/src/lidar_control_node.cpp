#include <ros/ros.h>
#include "dynamixel_servo/DynamixelFullConfig.h"
#include "dynamixel_servo/DynamixelJointConfig.h"
#include "dynamixel_servo/DynamixelStatus.h"
#include "dynamixel_servo/DynamixelControlTableRequest.h"
#include "dynamixel_servo/DynamixelControlTableParam.h"

ros::Publisher dynamixel_config_full_pub;
ros::Publisher dynamixel_config_position_pub;
//Min 150.1171		2.620039
//Max 222.8027		3.888641
const float MIN_ANGLE = 2.625;//2.357;
const float MAX_ANGLE = 3.14159;//3.891;

//Temporary globals
float abs_max_angle(0), abs_min_angle(0);

void GetLimits(const dynamixel_servo::DynamixelControlTableParam config)
{
	abs_max_angle = config.cw_angle_limit;
	abs_min_angle = config.ccw_angle_limit;
}

/*
 *  Main program
 */
int main(int argc, char** argv)
{
	/*
	 * 			Initilize ROS
	 */
	//Initilze the node ("lidar_control_node")
    ros::init(argc, argv, "lidar_control_node");
    //Initilze the node handle
    ros::NodeHandle n;
    ros::Rate sleep_time(1.5);

    //Initilze publishers
    ros::Publisher dynamixel_config_full_pub = n.advertise<dynamixel_servo::DynamixelFullConfig>("/dynamixel/dynamixel_config_full", 10);
    ros::Publisher dynamixel_config_position_pub = n.advertise<dynamixel_servo::DynamixelJointConfig>("/dynamixel/dynamixel_config_position", 10);
    ros::Publisher dynamixel_control_table_pub = n.advertise<dynamixel_servo::DynamixelControlTableRequest>("/dynamixel/dynamixel_control_table_request", 10);

    //Initilze the subscribers
    ros::Subscriber dynamixel_control_table_sub = n.subscribe("/dynamixel/dynamixel_control_table_post", 1, GetLimits);

    /*
     * 		Initilize the parameters
     */
    //Holds the servo to use to control the lidar
    int servo_id;
    n.param<int>("lidar/servo_id", servo_id, 1);

    //Wait for subscriber on control table request
    while(dynamixel_control_table_pub.getNumSubscribers() == 0);
    //Send out request for control table
    dynamixel_servo::DynamixelControlTableRequest request;
    request.id = servo_id;
    //Wait for reply
    while(abs_max_angle == 0);
    //Fill out abs max and min angles


    //Holds the maximum angle (rad) to set the lidar to
    float max_angle;
    //n.param<float>("lidar/max_angle", max_angle, 3.14);
    //Holds the minimum angle (rad) to set the lidar to
    float min_angle;
    //n.param<float>("lidar/min_angle", min_angle, 2.625);
    //Holds the angular speed (rad/s)
    //n.param<float>


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
    	msg.id = servo_id;

    	if(dir == 1)
    		msg.goal_position = max_angle;
    	else
    		msg.goal_position = min_angle;

    	dynamixel_config_position_pub.publish(msg);

		sleep_time.sleep();
		ros::spinOnce();
    }

    return 0;
}
