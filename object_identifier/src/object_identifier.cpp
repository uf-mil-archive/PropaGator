#include <ros/ros.h>
#include <visualization_msgs/MarkerArray.h>
// PCL specific includes

#include <Eigen/Core>
#include <Eigen/LU>

ros::Publisher identified_buoy_pub;

const float LC_X_TRANSLATION = 0;
const float LC_Y_TRANSLATION = 0;
const float LC_Z_TRANSLATION = 0;

const float LC_R_ROTATION = 0;
const float LC_P_ROTATION = 0;
const float LC_Y_ROTATION = 0;

void lidar_callback(const visualization_msgs::MarkerArrayConstPtr& input) {
  ROS_DEBUG("Got new lidar buoy_marker.");


}

void camera_callback(const /*whatever camera objects are published as*/ input) {
  ROS_DEBUG("Got new camera object.");

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
