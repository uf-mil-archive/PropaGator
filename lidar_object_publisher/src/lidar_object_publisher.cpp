#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
// PCL specific includes
#include <pcl/ros/conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>

ros::Publisher pub;

void cloud_callback(const sensor_msgs::PointCloud2ConstPtr& cloud) {

  sensor_msgs::PointCloud2 cloud_filtered;

  // ... do data processing
  // Create the filtering object
  pcl::VoxelGrid<sensor_msgs::PointCloud2> vox;
  vox.setInputCloud (cloud);
  vox.setLeafSize (0.01, 0.01, 0.01);
  vox.filter (cloud_filtered);

  // Publish the data
  pub.publish (cloud_filtered);
}

int main(int argc, char** argv) {
  // Initialize ROS
  ros::init (argc, argv, "my_pcl_tutorial");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe ("/cloud", 1, cloud_callback);

  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<sensor_msgs::PointCloud2> ("/filtered_cloud", 1);

  // Spin
  ros::spin ();
}
