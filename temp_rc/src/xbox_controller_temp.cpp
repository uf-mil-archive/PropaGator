#include "ros/ros.h"
#include <sensor_msgs/Joy.h>
#include "ros/time.h"
#include "ros/duration.h"
#include "motor_control/thrusterConfig"


/*
* Forward declarations
*/

//Buttons on the Xbox Controller
enum XboxButtons
{
	A,
	B,
	X,
	Y,
	LB,
	RB,
	BACK,
	START,
	POWER,
	LEFT_STICK,
	RIGHT_STICK,

	NUM_BTNS
};

/*
* LR = left right
* UD = up down
* Left = left stick
* Right = right stick
* RT = right trigger
* LT = left trigger
*/
enum XboxAxis
{
	LR_LEFT,
	UD_LEFT,
	LT,
	LR_RIGHT,
	UD_RIGHT,
	RT
};


/*
 * Variables
 */
//Keep track of button presses (to find button releases)
bool btn_pressed[NUM_BTNS] = { false }; 
bool btn_released[NUM_BTNS] = { false };

//Ros publishers and subscribers
ros::Publisher thruster_pub;

void XboxCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
	//Check for button releases
	for (int iii = 0; iii < NUM_BTNS; ++iii)
	{
		if (btn_pressed[iii] & !joy->buttons[iii])
			btn_released[iii] = true;
		else
			btn_released[iii] = false;

		btn_pressed[iii] = joy->buttons[iii];
	}
    
    thrusterConfig thrust;
    thrust.id = 2;
    thrust.thrust = joy->axes[UD_LEFT] * 100.0;
    thruster_pub.publish(thrust);

    thrust.id = 3;
    thrust.thrust = joy->axes[UD_RIGHT] * 100.0;
    thruster_pub.publish(thrust);
    
}



int main(int argc, char** argv)
{
    /*
     * Initilization
     */

    //Initilize the remote control node
    ros::init(argc, argv, "remote_control_node");

    //Node handler this is how you work with ROS
    ros::NodeHandle n;

    //Set up subscriber, listens to joy topic, buffer only 10 messages, us XboxCallback
    ros::Subscriber joy_sub = n.subscribe<sensor_msgs::Joy>("joy", 1, XboxCallback);


    //Set up publisher on motor_rc, buffer up to 1000 msgs
    thruster_pub = n.advertise<motor_control::WheelMotor>("wheel_motor_rc", 1);

    //Initilize time
    last_time = ros::Time::now();
    current_time = last_time;


    /*
     * Main loop
     */
    while (ros::ok())
    {
        ros::spinOnce();
    }

   return 0;
}
