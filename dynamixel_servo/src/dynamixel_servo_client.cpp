#include <iostream>
#include "ros/ros.h"
#include "dynamixel_servo_definitions.h" // This includes all the parameter values and constants one might need to know about a servo
#include "dynamixel_servo/DynamixelFullConfig.h"
#include "dynamixel_servo/DynamixelJointConfig.h"
#include "dynamixel_servo/DynamixelStatus.h"
#include "dynamixel_servo/DynamixelControlTableRequest.h"

#define USING_AX12A

using namespace std;


int main(int argc, char **argv)
{
  ros::init(argc, argv, "dynamixel_servo_client");
  ros::NodeHandle n;
  ros::Publisher dynamixel_config_request = n.advertise<dynamixel_servo::DynamixelFullConfig>("dynamixel_config_full", 1000);
  ros::Publisher dynamixel_control_table_request = n.advertise<dynamixel_servo::DynamixelControlTableRequest>("dynamixel_control_table_request",1000);
  ros::Rate loop_rate(10);// rate to run the loop in HZ
  uint8_t servo_count = 4;
  dynamixel_servo::DynamixelControlTableRequest dynamixel_control_table_request_msg;
  dynamixel_servo::DynamixelFullConfig config_msg;
  // init the messages with a valid id to start
  dynamixel_control_table_request_msg.id=2;
  config_msg.id=1;

  float one_deg_in_rads=3.14159265/180;

  // Initial setup for the config message
  config_msg.led=Servo::OFF;//1 means on
  config_msg.goal_position=0;
  config_msg.torque_limit=512;
  config_msg.moving_speed=512;
  config_msg.goal_acceleration=38.0;

  while (ros::ok())
    {
	  /////////////////////
	  //	Config Messages
	  ////////////////////

	  if(config_msg.led==Servo::OFF)
	  {
		  config_msg.led=Servo::ON;
	  }
	  else
	  {
		  config_msg.led=Servo::OFF;
	  }

	  // Config to test Goal Position
	  config_msg.goal_position+=one_deg_in_rads; // to move 10 degrees we need to increase it by 10/step_size=10/.088=113.636
	  if(config_msg.goal_position>2*3.14159265)
	  {
		  config_msg.goal_position=0;
	  }


      /**
       * The publish() function is how you send messages. The parameter
       * is the message object. The type of this object must agree with the type
       * given as a template parameter to the advertise<>() call, as was done
       * in the constructor above.
       */

	  dynamixel_config_request.publish(config_msg);
	  dynamixel_control_table_request.publish(dynamixel_control_table_request_msg);

	  /*
	  if(config_msg.id==servo_count||dynamixel_control_table_request_msg.id==servo_count)
	  {
		  config_msg.id=1;
		  dynamixel_control_table_request_msg.id=1;
	  }
	  else
	  {
		  config_msg.id++;
		  dynamixel_control_table_request_msg.id++;
	  }
	  */


	  ros::spinOnce();
	  loop_rate.sleep();

    }


    return 0;
}
