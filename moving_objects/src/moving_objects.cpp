/*
 * moving_objects.cpp
 *
 *  Created on: Oct 16, 2014
 *      Author: alan
 */

#include "ros/ros.h"
#include <tf/transform_broadcaster.h>
#include <nav_msgs/OccupancyGrid.h>
#include "sensor_msgs/LaserScan.h"
#include <moving_objects/lidar_ss.h>
#include <moving_object.h>
#include <visualization_msgs/Marker.h>

visualization_msgs::Marker marker;
ros::Publisher vis_pub;
int id;
ObjectList oldObjectList;

void scan1Callback(const sensor_msgs::LaserScan& msg){
	ObjectList newObjectList = ClusterAndSegment(msg);
	ROS_INFO("Num. objects: %i", newObjectList->numberOfObjects);
	Object object = NULL;
	Segment segment = NULL;
	if (newObjectList->numberOfObjects)
		object = newObjectList->firstObject;
	geometry_msgs::Point p;
	geometry_msgs::Point p2;
	marker.action = visualization_msgs::Marker::DELETE;
	for (int j = 0; j <= id; j++){
		marker.id = j;
		vis_pub.publish( marker );
	}
	marker.action = visualization_msgs::Marker::ADD;
	id = 0;
	marker.points.clear();

	while(object){
		segment = object->segmentList->firstSegment;
		while(segment){
			p.x = segment->FirstPoint->X;
			p.y = segment->FirstPoint->Y;
			p.z = 0;
			p2.x = segment->LastPoint->X;
			p2.y = segment->LastPoint->Y;
			p2.z = 0;
			marker.points.push_back(p);
			marker.points.push_back(p2);
			marker.id = id;
			vis_pub.publish( marker );
			id ++;
			segment = segment->nextSegment;
		}

		object = object->nextObject;
	}
}

int main(int argc, char **argv) {

	ros::init(argc, argv, "moving_objects");
	ros::NodeHandle n;
	vis_pub = n.advertise<visualization_msgs::Marker>( "visualization_marker", 0 );
	ros::Subscriber sub = n.subscribe("scan", 1, scan1Callback);

	ros::Rate loop_rate(10);
	oldObjectList = ObjectListInit();

	id = 0;
	marker.header.frame_id = "enu";
	marker.header.stamp = ros::Time();
	marker.id = 0;
	marker.type = visualization_msgs::Marker::LINE_LIST;
	marker.action = visualization_msgs::Marker::ADD;
	marker.pose.position.x = 15;
	marker.pose.position.y = 15;
	marker.pose.position.z = 0;
	marker.pose.orientation.x = 0.0;
	marker.pose.orientation.y = 0.0;
	marker.pose.orientation.z = 0.0;
	marker.pose.orientation.w = 1.0;
	marker.scale.x = 0.1;
	marker.color.a = 1.0;
	marker.color.r = 1.0;

	while (ros::ok()) {
		ros::spinOnce();

		loop_rate.sleep();
	}
}
