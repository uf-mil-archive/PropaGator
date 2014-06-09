#include <iostream>
#include "ros/ros.h"
#include "dynamixel_servo_definitions.h" // This includes all the parameter values and constants one might need to know about a servo
#include "dynamixel_servo/DynamixelConfigParam.h"
#include "dynamixel_servo/DynamixelStatusParam.h"
#include "dynamixel_servo/DynamixelControlTableRequest.h"

#define USING_AX12A

using namespace std;


int main(int argc, char **argv)
{
  ros::init(argc, argv, "dynamixel_servo_client");
  ros::NodeHandle n;
  ros::Publisher dynamixel_config_request = n.advertise<dynamixel_servo::DynamixelConfigParam>("dynamixel_config", 1000);
  ros::Publisher dynamixel_control_table_request = n.advertise<dynamixel_servo::DynamixelControlTableRequest>("dynamixel_control_table_request",1000);
  ros::Rate loop_rate(10);// rate to run the loop in HZ
  uint8_t count = 1;
  dynamixel_servo::DynamixelControlTableRequest dynamixel_control_table_request_msg;
  dynamixel_servo::DynamixelConfigParam config_msg;
  // init the messages with a valid id to start
  dynamixel_control_table_request_msg.id=1;
  config_msg.id=2;

  // Initial setup for the config message
  config_msg.led=Servo::ON;//1 means on
  config_msg.goal_position=Servo::MIN_GOAL_POSITION;
  uint16_t max_goal_position_to_use;
#ifdef USING_AX12A
  max_goal_position_to_use=Servo::MAX_GOAL_POSITION_AX12A;
#else
  max_goal_position_to_use=Servo::MAX_GOAL_POSITION;
#endif

  while (ros::ok())
    {
	  /////////////////////
	  //	Config Messages
	  ////////////////////
	  // Config to test LED
	  if(config_msg.led==Servo::OFF)
	  {
		  config_msg.led=Servo::ON;
	  }
	  else
	  {
		  config_msg.led=Servo::OFF;
	  }
	  // Config to test Goal Position
	  config_msg.goal_position+=12; // to move 10 degrees we need to increase it by 10/step_size=10/.088=113.636
	  if(config_msg.goal_position>max_goal_position_to_use)
	  {
		  config_msg.goal_position=Servo::MIN_GOAL_POSITION;
	  }

	  //////////////////////////////////
	  //	Status Request Messages
	  /////////////////////////////////
	  // General Status Request
	  dynamixel_control_table_request_msg.id=2;


      /**
       * The publish() function is how you send messages. The parameter
       * is the message object. The type of this object must agree with the type
       * given as a template parameter to the advertise<>() call, as was done
       * in the constructor above.
       */

	  dynamixel_config_request.publish(config_msg);
	  dynamixel_control_table_request.publish(dynamixel_control_table_request_msg);



	  ros::spinOnce();
	  loop_rate.sleep();

    }


    return 0;
}
