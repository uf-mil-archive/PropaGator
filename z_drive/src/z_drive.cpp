#include <ros/ros.h>
#include "z_drive_class.h"
#include <iostream>


int main(int argc, char** argv)
{
	ros::init(argc,argv,"z_drive");
	ZDrive z_drive_system;
	z_drive_system.run();
	return 0;
}
