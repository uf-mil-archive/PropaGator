#ifndef pot_fields_
#define pot_fields_

#include "potential_fields.h"
#include "sensor_msgs/PointCloud2.h"

potentialFields::potentialFields(){

	ros::NodeHandle private_nh("~");

	// set up subscription to rawPC
	ros::NodeHandle public_nh;
	obstacle_sub_ = public_nh.subscribe<sensor_msgs::PointCloud2>("raw_pc", 100, &potentialFields::suggestSoln, this);
}

void potentialFields::viewObstData(){	// what gets passed from interpretObst?  dunno yet

}

void potentialFields::suggestSoln(const sensor_msgs::PointCloud2::ConstPtr& in){	// what gets passed from interpretObst?  dunno yet
	ROS_INFO("WORKS!");
}

int main(int argc, char **argv){

	// initialize ROS
	ros::init(argc, argv, "potential_fields");
	ros::NodeHandle nh;
	ros::Rate update_rate(100);

	potentialFields pf;

	while(ros::ok()){
		update_rate.sleep();
		ros::spinOnce();
	}

	return 0;
}

#endif
