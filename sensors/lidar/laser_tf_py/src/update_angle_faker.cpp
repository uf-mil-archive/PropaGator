#include <ros/ros.h>
#include "laser_tf_py/ScanAngle.h"
#include <sstream>
#include <iostream>

class My_Filter {
  public:
    My_Filter();
    std_msgs::Header header;
    float min_angle;
    float max_angle;

    ros::NodeHandle node;
    ros::Publisher scan_angle_pub;
};

My_Filter::My_Filter() {
  scan_angle_pub = node.advertise<laser_tf_py::ScanAngle> ("/lidar_angle", 100, false);
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "ScanAngle_Update_Faker");

  My_Filter filter;

  while (ros::ok()) {
    std::cout << "Set new min angle: " << std::endl;
    std::cin >> filter.min_angle;

    std::cout << "Set new max angle: " << std::endl;
    std::cin >> filter.max_angle;
    laser_tf_py::ScanAngle scan_angle;
    scan_angle.header.stamp = ros::Time::now();
    scan_angle.header.frame_id = "/laser";
    scan_angle.min_angle = filter.min_angle;
    scan_angle.max_angle = filter.max_angle;
    filter.scan_angle_pub.publish(scan_angle);
  }

  ros::spin();

  return 0;
}
