#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <laser_geometry/laser_geometry.h>

#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"

#include <sstream>

class My_Filter {
  public:
    My_Filter();
    void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
  private:
    ros::NodeHandle node_;
    laser_geometry::LaserProjection projector_;
    tf::TransformListener tfListener_;

    ros::Publisher point_cloud_publisher_;
    ros::Subscriber scan_sub_;
};

My_Filter::My_Filter() {
  scan_sub_ = node_.subscribe<sensor_msgs::LaserScan> ("/scan", 100, &My_Filter::scanCallback, this);
  point_cloud_publisher_ = node_.advertise<sensor_msgs::PointCloud2> ("/cloud", 100, false);
  tfListener_.setExtrapolationLimit(ros::Duration(0.1));

}

void My_Filter::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan) {
  sensor_msgs::PointCloud2 cloud;
  try {
    projector_.transformLaserScanToPointCloud("/world", *scan, cloud, tfListener_);	// 1st arg was "base_link"
  } catch(tf::TransformException) {
    ROS_WARN("TransformException!");
    return;
  }
  point_cloud_publisher_.publish(cloud);
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "my_filter");

  My_Filter filter;

  ros::spin();

  return 0;
}









/*
bool new_scan = false;
sensor_msgs::LaserScan::ConstPtr scan;

void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan_in)
{
  scan = scan_in;
  new_scan = true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pointcloud_publisher");
  ros::NodeHandle n;
  ros::Subscriber pcl_sub = n.subscribe("scan", 1000, scanCallback);
  ros::Publisher pcl_pub = n.advertise<std_msgs::String>("pointcloud", 1000);
  tf::TransformListener tf_listener;
  ros::Rate rate(10.0);
  while (n.ok()){
    if (new_scan == true) {
      sensor_msgs::PointCloud cloud;
      tf::StampedTransform transform;
      try {
        tf_listener.lookupTransform("/world", "/laser", ros::Time(0), transform);
      } catch (tf::TransformException ex) {
        ROS_ERROR("%s",ex.what());
      }
      laser_geometry::LaserProjection projector_;
      projector_.transformLaserScanToPointCloud("base_link", *scan_in, cloud, tf_listener);
      pcl_pub.publish(cloud);
      new_scan = false;
    }
    rate.sleep();
  }
  return 0;
}
*/
