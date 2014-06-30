cmake_minimum_required(VERSION 2.8.3)
project(z_drive)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  #cpp related ros stuff
  roscpp
  #general msg stuff
  std_msgs
  message_generation
  genmsg
  #uf-mil created messages
  dynamixel_servo
  motor_control
  uf_common
  #transform messages that everything uses
  tf
  #visualization msgs
  sensor_msgs
  urdf
  # configurability related msgs
  dynamic_reconfigure
  #driving related messages
  actionlib_msgs
  actionlib
  nav_msgs
  #extra stuff that is still being tested
  geometry_msgs
  visualization_msgs
)

#first we will define the low level messages to be made, so the can be rolled up properly in other messages under this node
add_message_files(
  DIRECTORY msg/groups/external_forces
  FILES
  WaterFriction.msg
)

add_message_files(
  DIRECTORY msg/groups/gains
  FILES
  DGains.msg  ErrorGains.msg  PGains.msg
)

add_message_files(
  DIRECTORY msg/groups/operational_info
  FILES
  RunningModes.msg
)

add_message_files(
  DIRECTORY msg/groups/propulsion_system
  FILES
  Offset.msg  RuntimeData.msg  ServoLimits.msg  ThrusterLimits.msg
)

add_message_files(
  DIRECTORY msg/groups/odom_information
  FILES
  PitchOrientation.msg  RollOrientation.msg  XPosition.msg  YawVelocity.msg     YPosition.msg  ZPosition.msg
  PitchVelocity.msg     RollVelocity.msg     XVelocity.msg  YaxOrientation.msg  YVelocity.msg  ZVelocity.msg
)

add_message_files(
  DIRECTORY msg/groups/boat_relative_info
  FILES
  BoatRelatedForceLocal.msg BoatRelatedForceGlobal.msg
)

add_message_files(
  DIRECTORY msg/groups
  FILES
  BoatRelativeForces.msg
  CurrentPropulsionData.msg
  SimplifiedPosition.msg
  SimplifiedVelocity.msg
  EstimatedPropulsionData.msg
  FrictionalResistance.msg
  Gains.msg
  PropulsionSystemLimits.msg
  PropulstionSystemOffsets.msg
  
)


## Generate messages in the 'msg' folder
add_message_files(
	DIRECTORY msg
	FILES
	#message that rolls up all the ZDrive dbg msgs
	ZDriveDbg.msg
	#msg related to simulator based things
	BoatSimZDriveOutsideForce.msg

	
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
   nav_msgs
 )
 
# Generate the added dynamic reconfigurable messages
generate_dynamic_reconfigure_options(
#  cfg/relative_path_to_file1.cfg
	cfg/Gains.cfg
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
#  INCLUDE_DIRS include
#  LIBRARIES z_drive
  CATKIN_DEPENDS
	dynamixel_servo
	motor_control
	uf_common
#  DEPENDS system_lib
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

## Declare a cpp library
# add_library(z_drive
#   src/${PROJECT_NAME}/z_drive.cpp
# )

## Declare a cpp executable
add_executable(z_drive src/z_drive.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
add_dependencies(z_drive z_drive_generate_messages_cpp)
# needed for the dynamic reconfigurable
add_dependencies(z_drive ${PROJECT_NAME}_gencfg)

## Specify libraries to link a library or executable target against
target_link_libraries(z_drive ${catkin_LIBRARIES})

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS z_drive z_drive_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )