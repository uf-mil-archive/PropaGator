#ifndef pot_fields_
#define pot_fields_

#include "potential_fields.h"
//#include "std_msgs/String.h"
//#include "geometry_msgs/Vector3.h"		// repulsion suggestion vector

/*int interpretObst(const lidar_vision::buoy::ConstPtr& in){
	ROS_INFO("Obstacle at position (%f, %f) and has a radius of %f", in->position.x, in->position.y, in->radius);

	return 0;
}*/

potentialFields::potentialFields(){
	ros::NodeHandle private_nh("~");

	// set up subscription
	ros::NodeHandle public_nh;

	buoy_sub_ = public_nh.subscribe<lidar_vision::buoy>("buoy", 100, &potentialFields::suggestSoln, this);
			  //public_nh.subscribe<sensor_msgs::PointCloud2>(topic.c_str(), 100, &FeatureExtractor::extract, this);
	//buoy_pub_ = public_nh.advertise<lidar_vision::buoy>("buoy", 100);
	//buoy_marker_pub_ = public_nh.advertise<visualization_msgs::Marker>("buoy_marker", 100);
}

void potentialFields::viewObstData(){	// what gets passed from interpretObst?  dunno yet


}

geometry_msgs::Vector3 potentialFields::suggestSoln(const lidar_vision::buoy::ConstPtr& in){	// what gets passed from interpretObst?  dunno yet
	geometry_msgs::Vector3 soln;



	return soln;
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
	//ros::string topic = nh.resolveName("buoy_pc");

	//ros::Subscriber in = nh.subscribe(topic.c_str(), 100, interpretObst);
}

#endif
