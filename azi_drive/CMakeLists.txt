cmake_minimum_required(VERSION 2.8.3)
project(azi_drive)
find_package(catkin REQUIRED COMPONENTS
  rospy
)
catkin_python_setup()
catkin_package()
include_directories(
  ${catkin_INCLUDE_DIRS}
)
if (CATKIN_ENABLE_TESTING) 
    catkin_add_nosetests(tests/test_azi_drive.py)
endif()
