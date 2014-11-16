/*
 * lidar_ss.cpp
 *
 *  Created on: Oct 1, 2014
 *      Author: alan
 *      Modified by: Dan
 */

// Alan Hamlet
// September 22, 2014
//
// This node reads in multiple LIDAR's data, transforms it into a common frame,
// and uses it to create and publish an OccupancyGrid.

#include "ros/ros.h"
#include <string>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/OccupancyGrid.h>
#include "sensor_msgs/LaserScan.h"
#include <moving_objects/lidar_ss.h>

// Global variables (evil, I know)
nav_msgs::OccupancyGrid grid;
ros::Publisher grid_pub;

void scan1Callback(const sensor_msgs::LaserScan& msg){
	double start_angle = msg.angle_min;
	double increment = msg.angle_increment;
	float x0 = 15;
	float y0 = 15;
	float x1;
	float y1;
	std::string frame;
	ros::NodeHandle n("~");
	n.param("frame", frame, std::string("lidar"));
	nav_msgs::OccupancyGrid temp_grid;
	grid_init(temp_grid, 50, frame);
	for ( int i = 0; i < msg.ranges.size(); i++){
		x1 = msg.ranges[i]*cos(start_angle + i*increment) + x0;
		y1 = msg.ranges[i]*sin(start_angle + i*increment) + y0;
		raytrace(x0, y0, x1, y1, temp_grid);
		update_hit(x1, y1, temp_grid);
	}
	grid_update(grid, temp_grid);
	grid.header.stamp = ros::Time::now();
	grid.info.map_load_time = ros::Time::now();
	grid_pub.publish(grid);
}

int main(int argc, char **argv) {

	ros::init(argc, argv, "lidar_ss");
	ros::NodeHandle n("~");
	std::string topic;
	std::string frame;
	grid_pub = n.advertise<nav_msgs::OccupancyGrid>("Grid", 10);

	//nh_ns.param("topic", topic, string("scan"));
	n.param("topic", topic, std::string("/lidar1/scan"));
	n.param("frame", frame, std::string("lidar"));
	ros::Subscriber sub = n.subscribe(topic, 1, scan1Callback);

	ros::Rate loop_rate(10);

	grid_init( grid, 50, frame);

	static tf::TransformBroadcaster br;
	tf::Transform transform;
	transform.setOrigin(tf::Vector3(-15, -15, 0.0));
	tf::Quaternion q;
	q.setRPY(0, 0, 0);
	transform.setRotation(q);

	static tf::TransformBroadcaster br2;
	tf::Transform transform2;
	transform2.setOrigin(tf::Vector3(0, 0, 0.0));
	tf::Quaternion q2;
	q2.setRPY(0, 0, 0);
	transform2.setRotation(q2);

	while (ros::ok()) {
	/*br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "lidar","map1"));
		/*br2.sendTransform(tf::StampedTransform(transform2, ros::Time::now(), "world","laser2"));*/

		ros::spinOnce();

		loop_rate.sleep();
	}
	return 0;
}



