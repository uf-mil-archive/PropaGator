cmake_minimum_required(VERSION 2.8.3)
project(azi_drive)
find_package(catkin REQUIRED COMPONENTS
  message_generation
  rospy
  message_runtime
  std_msgs
)

add_service_files(
   FILES
   trajectory_mode.srv
   AziFloat.srv
)

catkin_python_setup()

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  LIBRARIES azi_drive azi_cvxbind
  CATKIN_DEPENDS message_runtime std_msgs rospy python_module
)

add_subdirectory (src/azi_cvx)
include_directories(
  ${catkin_INCLUDE_DIRS}
)
if (CATKIN_ENABLE_TESTING) 
    catkin_add_nosetests(tests/test_azi_drive.py)
endif()
