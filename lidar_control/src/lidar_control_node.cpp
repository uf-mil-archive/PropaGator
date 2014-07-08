#include <ros/ros.h>
#include "LidarAngleManager.h"

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
    LidarAngleManager manager;
    manager.Setup();
    manager.Run();

    return 0;
}
