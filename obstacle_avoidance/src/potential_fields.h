#ifndef pot_fields_h_
#define pot_fields_h_

#include <ros/ros.h>
#include <geometry_msgs/Vector3.h>
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
		ros::Subscriber obstacle_sub_;
		void viewObstData();
		void suggestSoln(const sensor_msgs::PointCloud2::ConstPtr& in);
};

#endif
