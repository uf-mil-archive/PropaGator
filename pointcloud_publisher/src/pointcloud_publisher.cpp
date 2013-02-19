#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <laser_geometry/laser_geometry.h>

#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include "pcl/point_cloud.h"
#include "std_msgs/Bool.h"
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>

#include <sstream>

const bool use_double_sweep = false;
bool second_sweep = false;

class My_Filter {
  public:
    My_Filter();
    void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
    void completeCallback(const std_msgs::Bool::ConstPtr& complete);
  private:
    ros::NodeHandle node_;
    laser_geometry::LaserProjection projector_;
    tf::TransformListener tfListener_;

    ros::Publisher point_cloud_publisher_;
    ros::Subscriber scan_sub_;
    ros::Subscriber complete_sub_;

    pcl::PointCloud<pcl::PointXYZ> pc_comb;
    std_msgs::Header header;
    bool first_scan;	// do I actually use this??...
};

My_Filter::My_Filter() {
  scan_sub_ = node_.subscribe<sensor_msgs::LaserScan> ("/scan", 100, &My_Filter::scanCallback, this);
  complete_sub_ = node_.subscribe<std_msgs::Bool> ("/scan_complete", 100, &My_Filter::completeCallback, this);
  point_cloud_publisher_ = node_.advertise<sensor_msgs::PointCloud2> ("/cloud", 100, false);
  tfListener_.setExtrapolationLimit(ros::Duration(0.1));

}

void My_Filter::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan) {
  sensor_msgs::PointCloud2 cloud;
  try {
    projector_.transformLaserScanToPointCloud("/world", *scan, cloud, tfListener_);	// 1st arg was "base_link"
    header = cloud.header;
  } catch(tf::TransformException) {
    ROS_WARN("TransformException!");
    return;
  }
  pcl::PointCloud<pcl::PointXYZ> pc;
  pcl::fromROSMsg(cloud, pc);
  pc_comb = pc_comb + pc;
}

void My_Filter::completeCallback(const std_msgs::Bool::ConstPtr& complete) {
  ROS_ERROR("IN COMPLETECALLBACK");
  if (complete && !use_double_sweep) {
    sensor_msgs::PointCloud2 cloud_out;
    pcl::toROSMsg(pc_comb, cloud_out);
    cloud_out.header = header;
    point_cloud_publisher_.publish(cloud_out);
    ROS_ERROR("Got Scan_Complete! Publishing Cloud!");
    pcl::PointCloud<pcl::PointXYZ> blank_pc;
    pc_comb = blank_pc;
    first_scan = true;
  } else if (complete && use_double_sweep) {
    if (second_sweep) {
      second_sweep = false;
      sensor_msgs::PointCloud2 cloud_out;
      pcl::toROSMsg(pc_comb, cloud_out);
      cloud_out.header = header;
      point_cloud_publisher_.publish(cloud_out);
      ROS_ERROR("Got Scan_Complete! Publishing Cloud!");
      pcl::PointCloud<pcl::PointXYZ> blank_pc;
      pc_comb = blank_pc;
      first_scan = true;
    } else {
      second_sweep = true;
    }
  } else {
    ROS_WARN("Got false Scan_Complete!");
  }
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "my_filter");

  My_Filter filter;

  ros::spin();

  return 0;
}
