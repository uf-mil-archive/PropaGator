cmake_minimum_required(VERSION 2.8.3)
project(server_interaction)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
  message_runtime
)

## Generate messages in the 'msg' folder
add_message_files(

   DIRECTORY msg
   FILES
   docking_bay_sequence.msg
   gate_code.msg
   images_info.msg	
#   Message2.msg
 )

## Generate services in the 'srv' folder
add_service_files(
   FILES
   image_info.srv
   buoy_color.srv
#   Service2.srv
 )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   std_msgs
)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need

catkin_package(
   DEPENDS #TODO
   INCLUDE_DIRS #TODO
   LIBRARIES #TODO
   CATKIN_DEPENDS roscpp rospy std_msgs message_runtime message_generation
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
install(PROGRAMS
   scripts/dockingbayprovider
   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
 )

