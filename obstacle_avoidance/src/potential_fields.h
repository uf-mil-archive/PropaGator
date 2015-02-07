#ifndef pot_fields_h_
#define pot_fields_h_

#include <ros/ros.h>
#include <geometry_msgs/Vector3.h>		// repulsion suggestion vector
#include "lidar_vision/buoy.h"			// obstacle data
#include <ros/rate.h>
#include <ros/time.h>

/*	TODO:
 * 		what gets passed from interpretObst?  dunno yet
 */

class potentialFields{
	public:
		potentialFields();

	private:
		ros::Subscriber buoy_sub_;	// subscriber to perceived buoys in vision
		//int interpretObst(const lidar_vision::buoy::ConstPtr& in);
		void viewObstData();
		geometry_msgs::Vector3 suggestSoln(const lidar_vision::buoy::ConstPtr& in);
};

#endif
