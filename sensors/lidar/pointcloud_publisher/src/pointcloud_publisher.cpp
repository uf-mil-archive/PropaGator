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

// counted in half sweeps (up to down = 1)
const int num_sweeps_2d = 1;
const int num_sweeps_3d = 2;
const bool b_2d = false;
const bool b_3d = true;

class My_Filter {
  public:
    My_Filter();
    void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
    void completeCallback(const std_msgs::Bool::ConstPtr& complete);
  private:
    ros::NodeHandle node;
    laser_geometry::LaserProjection projector;
    tf::TransformListener tfListener;

    ros::Publisher point_cloud_publisher_3d;
    ros::Publisher point_cloud_publisher_2d;
    ros::Subscriber scan_sub;
    ros::Subscriber complete_sub;

    pcl::PointCloud<pcl::PointXYZ> pc_comb_3d;
    pcl::PointCloud<pcl::PointXYZ> pc_comb_2d;
    std_msgs::Header header;
    int sweep_count_3d;
    int sweep_count_2d;
};

My_Filter::My_Filter() {
  scan_sub = node.subscribe<sensor_msgs::LaserScan> ("/scan", 100, &My_Filter::scanCallback, this);
  complete_sub = node.subscribe<std_msgs::Bool> ("/scan_complete", 100, &My_Filter::completeCallback, this);
  point_cloud_publisher_3d = node.advertise<sensor_msgs::PointCloud2> ("/cloud_3d", 100, false);
  point_cloud_publisher_2d = node.advertise<sensor_msgs::PointCloud2> ("/cloud_2d", 100, false);
  tfListener.setExtrapolationLimit(ros::Duration(0.1));
  sweep_count_3d = 0;
  sweep_count_2d = 0;
}

void My_Filter::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan) {
  sensor_msgs::PointCloud2 cloud;
  try {
    projector.transformLaserScanToPointCloud("/world", *scan, cloud, tfListener);	// 1st arg was "base_link"
    header = cloud.header;
  } catch(tf::TransformException) {
    ROS_WARN("TransformException!");
    return;
  }
  if (b_2d) {
    pcl::PointCloud<pcl::PointXYZ> pc_2d;
    pcl::fromROSMsg(cloud, pc_2d);
    int points = pc_2d.width;
    for (int i = 0; i < points; i++) {
      pc_2d.points[i].z = 0;
    }
    pc_comb_2d = pc_comb_2d + pc_2d;
  }
  if (b_3d) {
    pcl::PointCloud<pcl::PointXYZ> pc_3d;
    pcl::fromROSMsg(cloud, pc_3d);
    pc_comb_3d = pc_comb_3d + pc_3d;
  }
}

void My_Filter::completeCallback(const std_msgs::Bool::ConstPtr& complete) {
  if (b_3d) {
    if (complete && (sweep_count_3d >= num_sweeps_3d)) {
      sensor_msgs::PointCloud2 cloud_out_3d;
      pcl::toROSMsg(pc_comb_3d, cloud_out_3d);
      cloud_out_3d.header = header;
      point_cloud_publisher_3d.publish(cloud_out_3d);
      ROS_DEBUG("Got Scan_Complete! Publishing Cloud!");
      pcl::PointCloud<pcl::PointXYZ> blank_pc;
      pc_comb_3d = blank_pc;
      sweep_count_3d = 0;
    } else if (!complete) {
      ROS_WARN("Got false Scan_Complete!");
    } else {
      sweep_count_3d = sweep_count_3d + 1;
    }
  }
  if (b_2d) {
    if (complete && (sweep_count_2d >= num_sweeps_2d)) {
      sensor_msgs::PointCloud2 cloud_out_2d;
      pcl::toROSMsg(pc_comb_2d, cloud_out_2d);
      cloud_out_2d.header = header;
      point_cloud_publisher_2d.publish(cloud_out_2d);
      ROS_DEBUG("Publishing 2d cloud!");
      pcl::PointCloud<pcl::PointXYZ> blank_pc;
      pc_comb_2d = blank_pc;
      sweep_count_2d = 0;
    } else if (!complete) {
      ROS_WARN("False Scan_Complete!");
    } else {
      sweep_count_2d = sweep_count_2d + 1;
    }
  }
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "my_filter");

  My_Filter filter;

  ros::spin();

  return 0;
}
