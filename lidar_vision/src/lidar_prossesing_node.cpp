#include "ros/ros.h"
#include "LidarProssesor.h"

/*
 *  Main program
 */
int main(int argc, char** argv)
{
	/*
	 * 			Initilize ROS
	 */
	//Initilze the node ("lidar_prossisng node_node")
    ros::init(argc, argv, "lidar_prossesing_node");
    LidarProssesor prosesor;
    prosesor.Setup();
    prosesor.Run();

    return 0;
}
