#include <iostream>
#include "ros/ros.h"
#include "dynamixel_class.h"

/*
 * Notes:
 * 	Like ROS node handles, this class uses "RAII" style startup and shutdown- allowing for easy ".launch" file scripting.
 * 	Before creating an instance, it is expected that the ROS Parameter Server be instantiated as follows:
 * 		1. All dynamixel parameters grouped under the same namespace (e.g. they don't directly fall under "/"); by default this is "/dynamixel"
 * 		2. There is a key called "servo_namespace" in the root of this namespace to indicate the RELATIVE namespace for all the servo(s)
 * 		3. All of this nodes server related parameters (ex: serial_port and baud_rate) are set under the same namespace as the running node.
 * 				Note: The "baud_rate" is to be relative to the values of Dynamixel's baud rate EEPROM register- address 0x04 on the servo.
 * 		4. Servo(s) (in the "servo_namespace") are grouped according to the system they are in, THEN based on a single string that directly relates to the servo in the aforementioned system- i.e. description.
 * 		5. Each servo has at-least its "id" set accordingly on the parameter server (as an int). Only id(s) in the range [1-253] will be considered valid- to hedge against errors from uninitialized messages.
 * 				Note: The parameter server TAKES all int(s) in base 10 only, so, be sure to convert hexadecimal number in the Dynamixel's documentation to base 10 before setting them on the param server.
 *
 * 		Example of a ROS Paramater Server based on an instantiated node's namespace "/dynamixel/server":
 * 		/dynamixel/servo_namespace:"servo"
 * 		/dynamixel/server/baud_rate:"1"
 * 		/dynamixel/server/serial_port:"/dev/ttyUSB1"
 * 		/dynamixel/servo/lidar/bow/id:"1"
 *
 * 		Please see the "dynamixel_servo.launch" file for more information on how to script the startup of this node.
 *
 */


using namespace std;

int main(int argc, char **argv)
{
	// Note: if a .launch file is used to launch this node, the node's name ("dynamixel_servo_server") will be remaped/overridden by the name attribute of the node tag.
  ros::init(argc, argv, "dynamixel_servo_server");


  // Create a server based on the parameters that will be pulled from the ROS Parameter Server. Refer to the ".launch" file for more information.
  DynamixelServos servo_server;

  // Run the server (with ros::spin) allowing it to receive callbacks and comunicate with the servo(s) on its active com_port
  servo_server.run();


  return 0;
}
