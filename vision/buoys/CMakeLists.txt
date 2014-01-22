# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(buoys)
# Load catkin and all dependencies required for this package
# TODO: remove all from COMPONENTS that are not catkin packages.
find_package(catkin REQUIRED COMPONENTS roscpp sensor_msgs actionlib message_runtime std_msgs image_transport rospy message_generation cv_bridge actionlib_msgs)
# CATKIN_MIGRATION: removed during catkin migration
# cmake_minimum_required(VERSION 2.4.6)

# CATKIN_MIGRATION: removed during catkin migration
# include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)


# CATKIN_MIGRATION: removed during catkin migration
# rosbuild_find_ros_package(actionlib_msgs)

# CATKIN_MIGRATION: removed during catkin migration
# include(${actionlib_msgs_PACKAGE_PATH}/cmake/actionbuild.cmake)
add_action_files(
  FILES
  FindBuoys.action
)


# CATKIN_MIGRATION: removed during catkin migration
# rosbuild_init()

#set the default path for built executables to the "bin" directory

# CATKIN_MIGRATION: removed during catkin migration
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory

# CATKIN_MIGRATION: removed during catkin migration
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages

# CATKIN_MIGRATION: removed during catkin migration
# rosbuild_genmsg()
## Generate added messages and services with any dependencies listed here
generate_messages(
    DEPENDENCIES actionlib_msgs
)

# catkin_package parameters: http://ros.org/doc/groovy/api/catkin/html/dev_guide/generated_cmake_api.html#catkin-package
# TODO: fill in what other packages will need to use this package
catkin_package(
    DEPENDS  # TODO
    CATKIN_DEPENDS roscpp sensor_msgs actionlib message_runtime std_msgs image_transport rospy message_generation cv_bridge actionlib_msgs
    INCLUDE_DIRS # TODO include
    LIBRARIES # TODO
)

include_directories( ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})

#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})

install(PROGRAMS scripts/pointcloud2xyz.py scripts/buoy_detection.py DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
