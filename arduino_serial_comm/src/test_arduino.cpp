#include <ros/ros.h>
#include <cmath>
#include "motor_control/thrusterPWM.h"

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
    //This is the frequency of the node (default 1 Hz)
    ros::Rate sleep_time(100);

    //Initilze publishers
    ros::Publisher test_pub = n.advertise<motor_control::thrusterPWM>("thruster_pwm_config",10);

    /*
     * 			Main loop
     */
    while(ros::ok())
    {
    	motor_control::thrusterPWM msg;
    	msg.id = 2;
    	msg.pulse_width = rand() % 1000 + 1000;
    	test_pub.publish(msg);
    	
    	msg.id = 3;
    	msg.pulse_width = rand() % 1000 + 1000;
    	test_pub.publish(msg);

		sleep_time.sleep();
		ros::spinOnce();
    }

    return 0;
}
