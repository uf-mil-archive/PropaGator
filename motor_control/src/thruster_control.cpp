#include <ros/ros.h>
#include <thruster_control/ThrusterClass.h>

int main(int argc, char** argv)
{
	//Initilize ROS
    ros::init(argc, argv, "thruster_control_node");
    ros::NodeHandle n;
    ros::Rate sleep_time(1);

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
