#include <ros/ros.h>
#include <visualization_msgs/MarkerArray.h>
// PCL specific includes

#include <Eigen/Core>
#include <Eigen/LU>

ros::Publisher identified_buoy_pub;

visualization_msgs::MarkerArray camera_buoys;

void lidar_callback(const visualization_msgs::MarkerArrayConstPtr& lidar_buoys) {
  ROS_DEBUG("Got new lidar buoy_marker.");
  // Do work
  
}

void camera_callback(const visualization_msgs::MarkerArrayConstPrt& input) {
  ROS_DEBUG("Got new camera object.");
  // Store
  for (int i = 0; i < input->markers.size(); i++) {
    camera_buoys.pushback(input->markers[i]);
  }
}

int main(int argc, char** argv) {
  // Initialize ROS
  ros::init(argc, argv, "object_identifier");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber lidar_sub = nh.subscribe("/buoy_markers", 1, lidar_callback);
  ros::Subscriber camera_sub = nh.subscribe("/camera_blobs", 1, camera_callback);

  // Create a ROS publisher for the output point cloud
  identified_buoy_pub = nh.advertise<visualization_msgs::MarkerArray>("/identified_buoys", 1);

  // Spin
  ros::spin();
}
