#include <ros/ros.h>

/************************
 * 			Main		*
 ************************/
int main(int argc, char** argv)
{
	//Initialize ROS
	ros::init(argc, argv, "feature_extractor_node");
	ros::NodeHandle nh;
	ros::Rate update_rate(100);

	//Set up the extractor


	while(ros::ok())
	{
		update_rate.sleep();
		ros::spinOnce();
	}
	return 0;
}
