#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseArray.h>

ros::Publisher buoy_marker_pub;

void pose_callback(const geometry_msgs::PoseArrayConstPtr& pose) {

  ROS_ERROR("Got new PoseArray.");

  for (int i = 0; i < pose->poses.size(); i++) {
    visualization_msgs::Marker marker;
    marker.header.frame_id = "/world";
    marker.header.stamp = ros::Time::now();

     // marker.ns = "my_namespace";
    marker.id = i;

    marker.type = visualization_msgs::Marker::SPHERE;
    marker.action = visualization_msgs::Marker::ADD;
    marker.pose.position.x = pose->poses[i].position.x;
    marker.pose.position.y = pose->poses[i].position.y;
    marker.pose.position.z = pose->poses[i].position.z;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
    marker.scale.x = 0.3;
    marker.scale.y = 0.3;
    marker.scale.z = 0.32;
    marker.color.a = 0.5;//1.0
    marker.color.r = 1.0;
    marker.color.g = 0.0;
    marker.color.b = 0.0;
    buoy_marker_pub.publish(marker);
  }
}

int main(int argc, char** argv) {
  // Initialize ROS
  ros::init (argc, argv, "marker_publisher");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe ("/buoy_pose", 1, pose_callback);

  // Create a ROS publisher for the output point cloud
  buoy_marker_pub = nh.advertise<visualization_msgs::Marker>("/buoy_marker", 0);

  // Spin
  ros::spin ();
}
