#ifndef pot_fields_h_
#define pot_fields_h_

#include <ros/ros.h>
#include <geometry_msgs/Vector3.h>		// repulsion suggestion vector
#include <sensor_msgs/PointCloud2.h>
#include <ros/rate.h>
#include <ros/time.h>

/*	TODO:
 * 		what gets passed from interpretObst?  dunno yet
 */

class potentialFields{
	public:
		potentialFields();

	private:
		ros::Subscriber obstacle;
		void viewObstData();
		geometry_msgs::Vector3 suggestSoln(const lidar_vision::buoy::ConstPtr& in);
};

#endif
