#ifndef DYNAMIXEL_CLASS_H_
#define DYNAMIXEL_CLASS_H_

/*
 * Notes:
 *
 *
 *
 *
 */

#include <string>
#include <algorithm> // for find and search methods on stl containers
// std::vector is used instead of a std:deque for quicker creation/deletion/access times since most data won't exceed the page size of the OS.
// 		Also std::vector iterators are "random-access" giving log(n)+2 search complexity.
// 		For more information on the performance of std::vector and std:deque see: http://goo.gl/Q0GFca and http://goo.gl/T7dzdv and http://goo.gl/p85LuI
#include <vector>
#include "ros/ros.h"
#include "dynamixel_serial_port.h" //header file to talk to the dynamixel(s) through the serial port
#include "dynamixel_servo_definitions.h"
#include "dynamixel_servo/DynamixelFullConfig.h" //message for ALL available configurable location on a mx-64t servo that our project will need
#include "dynamixel_servo/DynamixelJointConfig.h" //simplified message to just set the position of a servo.
#include "dynamixel_servo/DynamixelStatus.h" //message status return parameters
#include "dynamixel_servo/DynamixelControlTableRequest.h" //message control table request
#include "dynamixel_servo/DynamixelControlTablePost.h" //message control table return paramaters
#include "dynamixel_servo/DynamixelWheelConfig.h"
#include "dynamixel_servo/DynamixelContinuousAngleConfig.h"


// Per Ros cpp guidelines you should only refer to each element in namespace std that you want. see: http://wiki.ros.org/CppStyleGuide#Namespaces-1
using std::string;
using std::vector;

class DynamixelServos
{
private:

	vector<Servo> servos;
	DynamixelSerialPort com_port;
	ros::NodeHandle n;
	ros::Publisher status_pub;
	ros::Subscriber full_config_subscriber;
	ros::Subscriber joint_config_subscriber;
	ros::Subscriber control_table_request_subscriber;
	ros::Subscriber wheel_config_subscriber;
	ros::Subscriber continuous_angle_config_subscriber;
	ros::Publisher control_table_request_publisher;
	double poll_rate;
	static const string DEFAULT_PARAM_SERVER_DYNAMIXEL_NAMESPACE;
	static const string DEFAULT_PARAM_SERVER_SERVO_NAMESPACE;
	static const float PI=3.14159265359; // since no other math functions are needed from math.h M_PI isn't used, but instead defined here. (to keep the namespace cleaner)

public:
	DynamixelServos();
	~DynamixelServos();
	void run();

protected:
	bool pingServo(uint8_t id);
	void controlTableRequestCallback(const dynamixel_servo::DynamixelControlTableRequest::ConstPtr& msg);
	void configCallbackFull(const dynamixel_servo::DynamixelFullConfig::ConstPtr& msg);
	void configCallbackJoint(const dynamixel_servo::DynamixelJointConfig::ConstPtr& msg);
	void configCallbackWheel(const dynamixel_servo::DynamixelWheelConfig::ConstPtr& msg);
	void configCallbackContinuousAngle(const dynamixel_servo::DynamixelContinuousAngleConfig::ConstPtr& msg);
	void setTorqueLimit(const vector<Servo>::iterator servo_to_config, uint16_t torque_limit);
	void setGoalAcceleration(const vector<Servo>::iterator servo_to_config, float goal_acceleration_rad_per_second_squared);
	void setLed(const vector<Servo>::iterator servo_to_config, uint8_t led_state);
	void setGoalPosition(const vector<Servo>::iterator servo_to_config, float goal_position);
	void setMovingSpeed(const vector<Servo>::iterator servo_to_config, float moving_speed, bool force_update);
	bool setControlMode(const vector<Servo>::iterator servo_to_config, dynamixel_servo::DynamixelFullConfig::_control_mode_type  control_mode);
	void stopAllServos();

	vector<uint8_t> servoRead(uint8_t id, uint8_t location, uint8_t num_bytes_to_read);
	float approximateSpeedControl(Servo & servo_to_approximate);

	void watchDogRoutine();

};
// A string is not an integral type and hence it must be defined outside the class.
const string DynamixelServos::DEFAULT_PARAM_SERVER_DYNAMIXEL_NAMESPACE="dynamixel";
const string DynamixelServos::DEFAULT_PARAM_SERVER_SERVO_NAMESPACE="servo";

DynamixelServos::DynamixelServos()
{
	// To avoid creating a new node handle we will reference the Private ROS Parameter Server Values using a bare interface.
	//			Note: all private parameters are prefaced with a "~"
	// Check for the serial_port private parameter
	string port;
	if(ros::param::has("~serial_port"))
	{
		ros::param::get("~serial_port",port);
		ROS_INFO("Private Parameter serial_port: %s",port.c_str());
	}
	else
	{
		// It wasn't set as a private parameter, so set it to the default value.
		port=DynamixelSerialPort::DEFAULT_SERIAL_PORT;
		ROS_INFO("Using default serial_port ( %s ) since private parameter serial_port was not set", port.c_str());
	}

	// Check for the baud_rate private parameter
	int temp_baud;
	uint8_t verified_baud;
	if(ros::param::has("~baud_rate"))
	{
		// Note that the XmlRpc class can't directly cast a XmlRpcValue that's an int to a uint8_t
		ros::param::get("~baud_rate",temp_baud);
		// Check the bounds on the baud_rate to make sure it's valid
		if(temp_baud<0)
		{
			ROS_FATAL("Private Parameter baud_rate should not be negative: %d",temp_baud);
			verified_baud = DynamixelSerialPort::DEFAULT_BAUD_RATE;
		}
		else if(temp_baud==0)
		{
			// If the value is 0 it is non standard for the basic linux serial port implementations and hence not allowed
			ROS_WARN("While the baud rate 0x%X is valid, it isn't in the default range [1-249]. So the baud rate will be set to default value 0x%X.",temp_baud,DynamixelSerialPort::DEFAULT_BAUD_RATE);
			verified_baud = DynamixelSerialPort::DEFAULT_BAUD_RATE;
		}
		else if(temp_baud>=0x01 && temp_baud<=0xF9)
		{
			// Valid range of baud rates so do nothing
			ROS_INFO("Private Parameter baud_rate: 0x%X",temp_baud);
			// Note the implicit cast
			verified_baud=(uint8_t)temp_baud;
		}
		else if(temp_baud>=0xFA && temp_baud<=0xFE)
		{
			// If the value is between [250-254] it is non standard and hence not allowed
			ROS_WARN("While the baud rate 0x%X is valid, it isn't in the default range [1-249]. So the baud rate will be set to default value 0x%X.",temp_baud,DynamixelSerialPort::DEFAULT_BAUD_RATE);
			verified_baud = DynamixelSerialPort::DEFAULT_BAUD_RATE;
		}
		else
		{
			// all other values outside the predefined ranges are invalid since it must fit inside a uint8_t
			ROS_FATAL("Private Parameter baud_rate falls outside the range of an uint8_t: %d",temp_baud);
			verified_baud = DynamixelSerialPort::DEFAULT_BAUD_RATE;
		}
	}
	else
	{
		// It wasn't set as a private parameter, so set it to the default value.
		verified_baud=DynamixelSerialPort::DEFAULT_BAUD_RATE;
		ROS_INFO("Using default baud_rate ( 0x%X ) since private parameter baud_rate was not set", verified_baud);
	}

	if(ros::param::has("~poll_rate"))
	{
		ros::param::get("~poll_rate",poll_rate);
		ROS_INFO("Private Parameter poll_rate: %fHz",poll_rate);
	}
	else
	{
		// It wasn't set as a private parameter, so set it to a default value.
		poll_rate=1;
		ROS_INFO("Using a default poll_rate of %fHz to poll information from the servo(s)- since private parameter poll_rate was not set", poll_rate);
	}

	// Initialize the com port based on the passed parameters
	com_port.setPortName(port);
	com_port.setBaudRate(verified_baud);
	// verify that the parameters work to get a handle on the com port
	// Note: the port name and baud are assumed to be set in "com_port" before a call to openPort() is made
	if(com_port.openPort()!=true)
	{
		ROS_FATAL("ERROR with node %s acquiring a handle on port %s",ros::this_node::getName().c_str(),com_port.getPortName().c_str());
	}

	// the dynamixel_namespace will at least resolve to the root ('/') since the node is in this function
	string dynamixel_namespace=n.getNamespace();
	if(dynamixel_namespace=="/")
	{
		ROS_WARN("All dynamixel components must sit under a namespace other than the root ('/'), trying the default dynamixel_namespace of: %s", DynamixelServos::DEFAULT_PARAM_SERVER_DYNAMIXEL_NAMESPACE.c_str());
		dynamixel_namespace.append(DynamixelServos::DEFAULT_PARAM_SERVER_DYNAMIXEL_NAMESPACE);
	}
	ROS_INFO("The dynamixel_namespace is: %s",dynamixel_namespace.c_str());
	string servo_namespace;
	// get the servo_namespace parameter value if it exists, if not use the default value
	n.param<std::string>("servo_namespace",servo_namespace,DynamixelServos::DEFAULT_PARAM_SERVER_SERVO_NAMESPACE);
	ROS_INFO("The servo_namespace is: %s",servo_namespace.c_str());

	// Retrieve the list of possible servo(s) located under "dynamixel_namespace/servo_namespace", note these are not private parameters.
	// searchParam searches upwards (from the private namespace to the default namespace) looking for a matching parameter instance, but note that it can only return a string. Hence we will use hasParam to check then getParam.
	XmlRpc::XmlRpcValue system_info;
	string servo_fully_qualified_name=dynamixel_namespace;
	servo_fully_qualified_name.append("/");
	servo_fully_qualified_name.append(servo_namespace);
	if(n.hasParam(servo_fully_qualified_name))
	{
		// Remember: Servo(s) will be grouped according to the system they are in, THEN based on a single string that directly relates to the servo in the aforementioned system.
		n.getParam(servo_fully_qualified_name,system_info);
		// For more information on pulling list parameters using XmlRpcValue(s) see: http://goo.gl/mxpPIb and http://goo.gl/3WgDce
		// To ensure that the correct types are pulled and paths were properly instantiated, the ROS documentation recommends to check the XmlRpcValue(s) with ROS_ASSERT.
		// ROS_ASSERT requires that the provided expression evaluates to true. If it is false, program execution will abort, with an informative statement about which assertion failed, in what file.
		// Since every servo must have at-least its ID on the Parameter Server, we know that getting All the param(s) under the servo_namespace key will be a struct (because there are still multiple subkeys).
		ROS_ASSERT(system_info.getType() == XmlRpc::XmlRpcValue::TypeStruct);
		ROS_INFO("Found %d systems under %s that use dynamixel servo(s)",system_info.size(),servo_fully_qualified_name.c_str());
		// create an iterator to easily go through the servo parameters that were on the server
		std::map<std::string, XmlRpc::XmlRpcValue>::iterator system_iterator;
		for(system_iterator=system_info.begin();system_iterator!=system_info.end();system_iterator++)
		{
			// the system_iterator's "first" value here is the name of the system; the "second" value is everything else related to that system.
			XmlRpc::XmlRpcValue systems_servo_info=system_iterator->second;
			// again check that the the structure on this new variable is valid
			ROS_ASSERT(systems_servo_info.getType() == XmlRpc::XmlRpcValue::TypeStruct);
			// So if we again "map" the XmlRpcValue, we effectively have each system broken down into its servo(s).
			std::map<std::string, XmlRpc::XmlRpcValue>::iterator systems_servo_iterator;
			for(systems_servo_iterator=systems_servo_info.begin();systems_servo_iterator!=systems_servo_info.end();systems_servo_iterator++)
			{
				// there may be multiple parameters specified for a servo, so again we will map them to pull the info- for ease of use.
				// the systems_servo_iterator "first" value here is the description of the servo in the given system; the "second" value are all the servo's parameters that we are interested in.
				XmlRpc::XmlRpcValue servo_params=systems_servo_iterator->second;
				ROS_ASSERT(servo_params.getType() == XmlRpc::XmlRpcValue::TypeStruct);
				std::map<std::string, XmlRpc::XmlRpcValue>::iterator servo_params_iterator;
				if(servo_params.hasMember("id")&&servo_params["id"].getType()==XmlRpc::XmlRpcValue::TypeInt)
				{
					// since the parameter server only stores base10 ints, convert it to the proper type for an "id"; it should be noted that an XmlRpcValue can't be directly converted to an uint8_t.
					int temp_id_int=servo_params["id"];
					// We limit the range of allowed id(s) to protect the system.
					// By default in a ros message, if a parameter (like the id parameter) is not set, it defaults to a value.
					// 	The default value that the ID parameter would take if it isn't set would be 0, so we don't want to send commands to the wrong servo on accident.
					//	The id of 254 is used to broadcast the command to all servos on the bus, and thus isn't allowed.
					if(temp_id_int>=0x01 && temp_id_int<=0xFD)
					{
						// Only after the "id" has been validated to be in the proper range of an uint8_t- also the range for an id- can we cast it to a uint8_t; this is because it just truncates the lower bytes.
						uint8_t temp_id=(uint8_t)temp_id_int;
						// Consider adding the servo to the known servo(s) only if we can at least initially ping it.
						if(pingServo(temp_id)==true)
						{
							// Each servo "id" must be unique, if the id is unique add it to the list of known servo(s).
							// Note: "find" returns an iterator to "last" if val was not found- "last" is related to iterators and is technically past the last data element.
							if(find(servos.begin(),servos.end(),temp_id)==servos.end())
							{
								servos.push_back(Servo(temp_id,system_iterator->first,systems_servo_iterator->first,verified_baud));
								for(servo_params_iterator=servo_params.begin();servo_params_iterator!=servo_params.end();servo_params_iterator++)
								{
									// This section is so future servo initialization parameters can be added for servo(s) on the param server

								}
							}
							else
							{
								ROS_WARN("Each \"id\" must be unique for node %s to be able to talk to the servo. The node will not communicate Conflicting \"id\": 0x%X",ros::this_node::getName().c_str(),temp_id);
								remove(servos.begin(),servos.end(),temp_id);
							}
						}
					}
					else
					{
						ROS_WARN("The ID of 0x%X is not in the allowed range [1-253] for Servo Description: %s\t\t System: %s",temp_id_int,systems_servo_iterator->first.c_str(),system_iterator->first.c_str());
					}
				}
				else
				{
					ROS_WARN("At least the \"id\" parameter must be specified as an \"int\" for Servo Description: %s\t\t System: %s",systems_servo_iterator->first.c_str(),system_iterator->first.c_str());
				}
			}
		}
	}
	else
	{
		// the search didn't succeed, no systems were defined- to use servos-  in the servo_namespace
		ROS_WARN("No \"/%s\" system(s) was defined on the ROS Parameter Server relative to the \"%s\" namespace",servo_namespace.c_str(),dynamixel_namespace.c_str());
		ROS_WARN("All Dynamixel Servo(s) must be know and set on the on the ROS Parameter Server prior to the instantiation of the %s node.", ros::this_node::getName().c_str());
	}

	if(servos.size()>0)
	{

		ROS_INFO("The node %s will communicate with the following verified servo(s):", ros::this_node::getName().c_str());
		for(vector<Servo>::iterator i=servos.begin();i!=servos.end();i++)
		{
			ROS_INFO("\t\t\tSystem: %s\t\tDescription: %s\t\tID: 0x%X", i->getSystemMembership().c_str(),i->getDescription().c_str(),i->getID());
		}
		// Sort the servo(s) based on their "id" (since the vector doesn't change after this initialization). This lets us use "binary_search" on the vector to verify the "id"(s) in messages faster.
		sort(servos.begin(),servos.end());
	}

	// Ensure that the servos are stopped when this node is turned on
	stopAllServos();

	// Now that everything is setup on the server we will allow subscriptions and advertise the posts
	status_pub= n.advertise<dynamixel_servo::DynamixelStatus>("dynamixel_status_post", 1000);
	control_table_request_subscriber= n.subscribe("dynamixel_control_table_request", 1000, &DynamixelServos::controlTableRequestCallback, this);
	control_table_request_publisher=n.advertise<dynamixel_servo::DynamixelControlTablePost>("dynamixel_control_table_post",1000);
	full_config_subscriber= n.subscribe("dynamixel_full_config", 1000, &DynamixelServos::configCallbackFull,this);
	joint_config_subscriber=n.subscribe("dynamixel_joint_config", 1000, &DynamixelServos::configCallbackJoint,this);
	wheel_config_subscriber=n.subscribe("dynamixel_wheel_config", 1000, &DynamixelServos::configCallbackWheel,this);
	continuous_angle_config_subscriber=n.subscribe("dynamixel_continuous_config", 1000, &DynamixelServos::configCallbackContinuousAngle,this);
}

void DynamixelServos::run()
{

	// there must be an active handle on the com_port to communicate with the servo(s) in subsequent callbacks
	if(com_port.isOpen()==true)
	{
		// initially populate the servo struct with info for all the servo(s), and post it.
		boost::shared_ptr<dynamixel_servo::DynamixelControlTableRequest> init_msg=boost::make_shared<dynamixel_servo::DynamixelControlTableRequest>();
		for(vector<Servo>::iterator i=servos.begin();i!=servos.end();i++)
		{
			init_msg->id=i->getID();
			controlTableRequestCallback(init_msg);
			// changing the control mode from joint to wheel or joint to multi-turn mode clears the previous safty limits, so try to keep track of this
			if(i->inWheelMode())
			{
				ROS_INFO("Servo 0x%X is set inWheelMode from a previous config, setting the previous safty limits to 0x%04X (clockwise) and 0x%04X (counterclockwise)- so it can switch to joint mode when requested.", i->getID(), Servo::MIN_GOAL_POSITION, Servo::MAX_GOAL_POSITION);
				i->previous_cw_angle_limit=Servo::MIN_GOAL_POSITION;
				i->previous_ccw_angle_limit=Servo::MAX_GOAL_POSITION;
			}
			else if(i->inMultiTurnMode())
			{
				ROS_INFO("Servo 0x%X is set inMultiTurnMode from a previous config, setting the previous safty limits to 0x%04X (clockwise) and 0x%04X (counterclockwise)- so it can switch to joint mode when requested.", i->getID(), Servo::MIN_GOAL_POSITION, Servo::MAX_GOAL_POSITION);
				i->previous_cw_angle_limit=Servo::MIN_GOAL_POSITION;
				i->previous_ccw_angle_limit=Servo::MAX_GOAL_POSITION;

			}
			else
			{
				ROS_INFO("Servo 0x%X has the previous safty limits to 0x%04X (clockwise) and 0x%04X (counterclockwise).", i->getID(), i->cw_angle_limit.two_byte_value, i->ccw_angle_limit.two_byte_value);
				i->previous_cw_angle_limit=i->cw_angle_limit;
				i->previous_ccw_angle_limit=i->ccw_angle_limit;
			}
			// also init the position information for the continuous rotation in software
			i->previous_continuous_position_in_radians=i->current_continuous_position_in_radians=PI;
		}

		ROS_INFO("%s is running with a handle on port: %s",ros::this_node::getName().c_str(),com_port.getPortName().c_str());

		ros::Rate loop_rate(poll_rate);
		vector<uint8_t> read_return;
		dynamixel_servo::DynamixelStatus status_message;

		// This will be used as a watchdog timer to make sure that there are no errors on the port or with the servo(s), if there are it will try to recover.
		double watch_dog_count=0;

		float position_degrees=0.0;
		float position_radians=0.0;
		
		// on boot lock the lidar in place straight ahead
		if(binary_search(servos.begin(),servos.end(), 0x01))
		{
		    vector<Servo>::iterator servo_to_config=find(servos.begin(),servos.end(),0x01);
		    setGoalPosition(servo_to_config,3.2);
		}
		
		// on boot energize the hydrophone servo to lock in the up position. This is needed because the inital torque limit that is set is very small to protect it agains miss-use.
		boost::shared_ptr<dynamixel_servo::DynamixelFullConfig> init_hydrophone_msg=boost::make_shared<dynamixel_servo::DynamixelFullConfig>();
		if(binary_search(servos.begin(),servos.end(), 0x04))
		{
		    init_hydrophone_msg->id=0x04;
		    init_hydrophone_msg->led=0;
		    init_hydrophone_msg->goal_position=4.3; // 2.4 rad/s~22 rpm NOTE: we explicitly retract to pi to try and avoid being at the 0/2*PI boundary on a powerup
		    init_hydrophone_msg->moving_speed=1.4; // 1.4 rad/s~22 rpm
		    init_hydrophone_msg->torque_limit=143; // 143/1023 is about 14% torque (so we don't break the rope if someone didn't feed them correctly to start)
		    init_hydrophone_msg->goal_acceleration=20;
		    init_hydrophone_msg->control_mode=dynamixel_servo::DynamixelFullConfig::CONTINUOUS_ANGLE;
		    init_hydrophone_msg->goal_velocity=1.4;
		    configCallbackFull(init_hydrophone_msg);
		}
		
		// We will poll the servo(s) based on the loop rate; so because we need to do that work we will use the "while(ros::ok)" paradigm.
		while(ros::ok())
		{
			for(vector<Servo>::iterator i=servos.begin();i!=servos.end();i++)
			{
				// Do a block read from the servo's (contiguous) ram- which is faster than multiple read request for each register.
				// 		The block of memory that is associated with a status post is from the [GOAL_POSITION_REG to the IS_MOVING_REG]. Note: don't forget that it's inclusive, hence the plus 1.
				read_return=servoRead(i->getID(), Servo::GOAL_POSITION_REG, (Servo::IS_MOVING_REG-Servo::GOAL_POSITION_REG+1));
				// verify that it read the correct number of bytes- note: the size of the vector returned on an error will be 0.
				if(read_return.size()==(Servo::IS_MOVING_REG-Servo::GOAL_POSITION_REG+1))
				{
					// Since a bus read is slower than the cpu cycles required to update a servo struct, don't waste the new information gathered.
					status_message.id=i->getID();
					i->goal_position=dynamixel_uint16_t(read_return[1],read_return[0]);
					//position_degrees=i->goal_position*(360.0/Servo::MAX_GOAL_POSITION);
					//position_radians=position_degrees*(PI/180);
					status_message.goal_position=i->getGoalPositionInRadians();
					i->moving_speed=dynamixel_uint16_t(read_return[3],read_return[2]);
					status_message.moving_speed=i->getMovingSpeedRadPerSec();
					i->torque_limit=dynamixel_uint16_t(read_return[5],read_return[4]);
					status_message.torque_limit=i->torque_limit;
					//i->present_position=dynamixel_uint16_t(read_return[7],read_return[6]);
					//position_degrees=i->present_position*(360.0/Servo::MAX_GOAL_POSITION);
					//position_radians=position_degrees*(PI/180);
					i->setPresentPosition(dynamixel_uint16_t(read_return[7],read_return[6]));
					status_message.present_position=i->getPresentPositionInRadians();

					i->present_speed=dynamixel_uint16_t(read_return[9],read_return[8]);
					status_message.present_speed=i->getPresentSpeedRadPerSec();
					i->present_load=dynamixel_uint16_t(read_return[11],read_return[10]);
					status_message.present_load=i->present_load;
					i->present_voltage=read_return[12];
					status_message.present_voltage=(i->present_voltage/10.0);//This value in a reg is 10 times larger than the actual voltage
					i->present_temp=read_return[13];
					status_message.present_temp=i->present_temp;
					i->instruction_registered=read_return[14];
					status_message.registered=i->instruction_registered;
					// Note: Byte 45 (0x2D) isn't in the ram, this corresponds to byte 15 in the read_return here,
					i->is_moving=read_return[16];
					status_message.is_moving=i->is_moving;
					status_pub.publish(status_message);
					if(i->continuous_angle_mode)
					{
						setMovingSpeed(i,approximateSpeedControl(*i),true);// if in continuous rotation mode make sure the servo is still good
					}
				}
				else
				{
					ROS_WARN("Can't post status message for id: 0x%X due to bad read request", i->getID());
				}
			}

			// process a round of callback(s)
			ros::spinOnce();

			// check the watch dog timer to see if we need to try and recover from any errors.
			// 	It runs approximately every 1 second or 1 period, which ever is larger (due to the fact that poll_rate is in Hz).
			if(++watch_dog_count>=poll_rate)
			{
				// run the watch dog routine
				watchDogRoutine();
				// reset the watchdog timer
				watch_dog_count=0;
			}

			// if there is still time left in the rest of the cycle sleep
			loop_rate.sleep();

		}
	}
	else
	{
		// This is considered a fatal error because the node will not even run if we can't talk on the com port
		ROS_FATAL("ERROR node %s doesn't have an active handle on port %s and can't be run.",ros::this_node::getName().c_str(),com_port.getPortName().c_str());
		ROS_FATAL("%s will now EXIT", ros::this_node::getName().c_str());
	}
}

DynamixelServos::~DynamixelServos()
{
	// Ensure that the servos are stopped before this node is turned off
	stopAllServos();

	// Ensure that the com_port is closed when the variable goes out of scope, remember this can happen when the ros node is killed.
	if(com_port.isOpen()==true)
	{
		// Note: fprintf is used here instead of ROS_INFO because ROS_INFO does not run once rosnode kill is called (ctrl+c).
		fprintf(stdout,"%s is closing its handle on port %s.\n",ros::this_node::getName().c_str(),com_port.getPortName().c_str());
		com_port.closePort();
	}
}

void DynamixelServos::watchDogRoutine()
{
	// this routine should be as fast as possible and try to recover devices that are in and error state
	if(com_port.comErrorPresent())
	{
		ROS_WARN("%s is reseting its handle on port %s since it has the error: %s.",ros::this_node::getName().c_str(),com_port.getPortName().c_str(),com_port.printComStatus().c_str());
		com_port.closePort();
		if(com_port.openPort())
		{
			ROS_INFO("%s has reestablished a handle on port: %s",ros::this_node::getName().c_str(),com_port.getPortName().c_str());
		}
	}
	else if(com_port.isOpen())
	{
		// Since the comport is up and working, see if any servo(s) have been put into an error mode (based on Alarm Shutdown EPROM)
		vector<uint8_t> read_return;
		for(vector<Servo>::iterator i=servos.begin();i!=servos.end();i++)
		{
			// See if any servo(s) have errors. If so, try to get them operational again.
			read_return=servoRead(i->getID(),Servo::TORQUE_LIMIT_REG, sizeof(Servo::TORQUE_LIMIT_REG));
			if(read_return.size()==sizeof(Servo::TORQUE_LIMIT_REG))
			{
				dynamixel_uint16_t read_torque_limit(read_return[1],read_return[0]);
				dynamixel_uint16_t last_known_torque_limit=i->torque_limit;
				if(last_known_torque_limit!=read_torque_limit)
				{
					// update the servo struct to reflect the changes we just read- don't waste the time we just used on the bus.
					i->torque_limit=read_torque_limit;
				}
				// If the function of Alarm Shutdown is triggered, the motor loses its torque- setting the TORQUE_LIMIT_REG's value to 0.
				// So if the servo wasn't initially configured to be off, due to the initial default values on boot, an error has occurred.
				if(read_torque_limit==Servo::OFF && i->getMaxTorqueDefaultValue()!=Servo::OFF)
				{
					// If the value of TORQUE_LIMIT_REG is changed to a value other than 0, the motor will be able to be used again.
					// If possible re-enable it to the last known value, otherwise set it to the EPROM configured default.
					if(last_known_torque_limit!=Servo::OFF)
					{
						setTorqueLimit(i,last_known_torque_limit);
						ROS_WARN("Servo id: 0x%X seems to have been shutdown; re-enabling it with last know \"Torque Limit\" of 0x%X", i->getID(), last_known_torque_limit.two_byte_value);
					}
					else
					{
						setTorqueLimit(i, i->getMaxTorqueDefaultValue());
						ROS_WARN("Servo id: 0x%X seems to have been shutdown; re-enabling it with default \"Max Torque\" EPROM value of 0x%X", i->getID(), i->getMaxTorqueDefaultValue());
					}
				}
			}
			else
			{
				ROS_WARN("Can't check for errors on servo id: 0x%X due to bad read request", i->getID());
			}
		}
	}
	else if(com_port.isOpen()==false)
	{
		ROS_WARN("%s indicates port %s is closed. Trying to re-open the port.",ros::this_node::getName().c_str(),com_port.getPortName().c_str());
		if(com_port.openPort())
		{
			ROS_INFO("%s has reestablished a handle on port: %s",ros::this_node::getName().c_str(),com_port.getPortName().c_str());
		}
	}
}

void DynamixelServos::controlTableRequestCallback(const dynamixel_servo::DynamixelControlTableRequest::ConstPtr& msg)
{
	if(msg->id==0x00)
	{
		ROS_WARN("Servo ID 0x00 is explicitly not allowed to prevent people from requesting info from the wrong servo due to uninitialized ID parameters in the messages.");
		ROS_WARN("The %s is dropping the Status Request message for servo ID 0x%X.",ros::this_node::getName().c_str(),msg->id);
		return;
	}
	// Verify that the "id" is a servo that we verified exists so we don't tie up the bus with needless messages- because cpu calculations are faster than bus transfer speeds.
	// "binary_search" is an "STL" algorithm that does a binary search in O(log n) time on the servo vector- note that it's sorted during the initialization; it returns "true" if the "id" was found.
	if(binary_search(servos.begin(),servos.end(),msg->id))
	{

		// Do a block read from the servo's (contiguous) EPROM then RAM- which is faster than multiple read request for each register.
		// 		The block of memory that is associated with EPROM is from the [MODEL_EPROM to the TORQUE_ENABLE_REG-1].
		//			The block of memory that is associated with RAM is from the [TORQUE_ENABLE_REG to the GOAL_ACCELERATION_REG].
		//			Note: Don't forget that the addresses are 0 indexed and the range is inclusive, hence the plus 1.
		vector<uint8_t> eprom_return=servoRead(msg->id, Servo::MODEL_EPROM, ((Servo::TORQUE_ENABLE_REG-1)-Servo::MODEL_EPROM+1));
		vector<uint8_t> ram_return=servoRead(msg->id, Servo::TORQUE_ENABLE_REG,(Servo::GOAL_ACCELERATION_REG-Servo::TORQUE_ENABLE_REG+1));
		// verify that it read the correct number of bytes- note: the size of the vector returned on an error will be 0.
		if(eprom_return.size()!=((Servo::TORQUE_ENABLE_REG-1)-Servo::MODEL_EPROM+1))
		{
			ROS_WARN("Can't read EPROM Control Table for id: 0x%X due to bad read request size of %lu", msg->id, eprom_return.size());
		}
		else if(ram_return.size()!=(Servo::GOAL_ACCELERATION_REG-Servo::TORQUE_ENABLE_REG+1))
		{
			ROS_WARN("Can't read Ram Control Table for id: 0x%X due to bad read request size of %lu", msg->id, eprom_return.size());
		}
		else if(eprom_return[Servo::ID_EPROM]!=msg->id)
		{
			ROS_WARN("When pulling the EPROM control table for servo id: 0x%x, the value returned for \"id\" (which was: 0x%x) doesn't match.", msg->id, eprom_return[Servo::ID_EPROM]);
		}
		else if(eprom_return.size()+ram_return.size()<=Servo::GOAL_ACCELERATION_REG)
		{
			// The EEPROM+RAM makes up the control table, so their combined "size" should be larger than the last address that we can reference into- since it's 0 indexed.
			ROS_WARN("The Size of EEPROM and RAM (0x%lX) is smaller than the last address that contains information, address: 0x%X",eprom_return.size()+ram_return.size(), Servo::GOAL_ACCELERATION_REG);
		}
		else
		{
			// Efficiently merge the two data structures into one so we can easily access the relevant values in accordance with the entire control table.
			//			Note: the combined size of the vectors that are being merged is "reserved" so it doesn't have to waste time growing.
			vector<uint8_t> control_table;
			control_table.reserve(eprom_return.size()+ram_return.size());
			control_table.insert(control_table.end(), eprom_return.begin(), eprom_return.end());
			control_table.insert(control_table.end(), ram_return.begin(), ram_return.end());
			// We will actually fill in the servo structs with their relevant data so we can reference it- for speed- in future functions.
			// Since we know the element was found based on the binary_search (O log n), get a handle on the servo whose data we will fill in.
			vector<Servo>::iterator servo_to_update=find(servos.begin(),servos.end(),msg->id);

			//---------------- EEPROM ---------------------------------------
			servo_to_update->model_num=dynamixel_uint16_t(control_table[Servo::MODEL_EPROM+1],control_table[Servo::MODEL_EPROM]);
			servo_to_update->firmware_ver=control_table[Servo::FIRMWARE_EPROM];
			servo_to_update->id=control_table[Servo::ID_EPROM];
			servo_to_update->baud=control_table[Servo::BAUD_EPROM];
			servo_to_update->return_delay_time=control_table[Servo::RETURN_DELAY_EPROM];
			servo_to_update->cw_angle_limit=dynamixel_uint16_t(control_table[Servo::CW_ANGLE_LIMIT_EPROM+1],control_table[Servo::CW_ANGLE_LIMIT_EPROM]);
			servo_to_update->ccw_angle_limit=dynamixel_uint16_t(control_table[Servo::CCW_ANGLE_LIMIT_EPROM+1],control_table[Servo::CCW_ANGLE_LIMIT_EPROM]);
			servo_to_update->high_temp_limit=control_table[Servo::HIGH_TEMP_LIMIT_EPROM];
			servo_to_update->low_voltage_limit=control_table[Servo::LOW_VOLTAGE_LIMIT_EPROM];
			servo_to_update->high_voltage_limit=control_table[Servo::HIGH_VOLTAGE_LIMIT_EPROM];
			servo_to_update->max_torque=dynamixel_uint16_t(control_table[Servo::MAX_TORQUE_EPROM+1],control_table[Servo::MAX_TORQUE_EPROM]);
			servo_to_update->status_return_level=control_table[Servo::STATUS_RETURN_LEVEL_EPROM];
			servo_to_update->alarm_led=control_table[Servo::ALARM_LED_EPROM];
			servo_to_update->alarm_shutdown=control_table[Servo::ALARM_SHUTDOWN_EPROM];
			servo_to_update->multi_turn_offset=dynamixel_uint16_t(control_table[Servo::MULTI_TURN_OFFSET_EPROM+1],control_table[Servo::MULTI_TURN_OFFSET_EPROM]);
			servo_to_update->resolution_divider=control_table[Servo::RESOLUTION_DIVIDER_EPROM];
			//--------------- RAM -----------------------------------------
			servo_to_update->torque_enable=control_table[Servo::TORQUE_ENABLE_REG];
			servo_to_update->led=control_table[Servo::LED_REG];
			servo_to_update->pid_d_gain=control_table[Servo::PID_D_REG];
			servo_to_update->pid_i_gain=control_table[Servo::PID_I_REG];
			servo_to_update->pid_p_gain=control_table[Servo::PID_P_REG];
			servo_to_update->goal_position=dynamixel_uint16_t(control_table[Servo::GOAL_POSITION_REG+1],control_table[Servo::GOAL_POSITION_REG]);
			servo_to_update->moving_speed=dynamixel_uint16_t(control_table[Servo::MOVING_SPEED_REG+1],control_table[Servo::MOVING_SPEED_REG]);
			servo_to_update->torque_limit=dynamixel_uint16_t(control_table[Servo::TORQUE_LIMIT_REG+1],control_table[Servo::TORQUE_LIMIT_REG]);
			servo_to_update->present_position=dynamixel_uint16_t(control_table[Servo::PRESENT_POSITION_REG+1],control_table[Servo::PRESENT_POSITION_REG]);
			//servo_to_update->setPresentPosition(dynamixel_uint16_t(control_table[Servo::PRESENT_POSITION_REG+1],control_table[Servo::PRESENT_POSITION_REG]));
			servo_to_update->present_speed=dynamixel_uint16_t(control_table[Servo::PRESENT_SPEED_REG+1],control_table[Servo::PRESENT_SPEED_REG]);
			servo_to_update->present_load=dynamixel_uint16_t(control_table[Servo::PRESENT_LOAD_REG+1],control_table[Servo::PRESENT_LOAD_REG]);
			servo_to_update->present_voltage=control_table[Servo::PRESENT_VOLTAGE_REG];
			servo_to_update->present_temp=control_table[Servo::PRESENT_TEMP_REG];
			servo_to_update->instruction_registered=control_table[Servo::INSTRUCTION_REGISTERED_REG];
			servo_to_update->is_moving=control_table[Servo::IS_MOVING_REG];
			servo_to_update->eeprom_lock=control_table[Servo::EEPROM_LOCK_REG];
			servo_to_update->punch=dynamixel_uint16_t(control_table[Servo::PUNCH_REG+1],control_table[Servo::PUNCH_REG]);
			servo_to_update->consuming_current=dynamixel_uint16_t(control_table[Servo::CONSUMING_CURRENT_REG+1],control_table[Servo::CONSUMING_CURRENT_REG]);
			servo_to_update->torque_control_mode=control_table[Servo::TORQUE_CONTROL_MODE_REG];
			servo_to_update->goal_torque=dynamixel_uint16_t(control_table[Servo::GOAL_TORQUE_REG+1],control_table[Servo::GOAL_TORQUE_REG]);
			servo_to_update->goal_acceleration=control_table[Servo::GOAL_ACCELERATION_REG];

			// now generate the message and publish it
			dynamixel_servo::DynamixelControlTablePost control_table_message;
			control_table_message.model_num=servo_to_update->model_num;
			control_table_message.firmware_ver=servo_to_update->firmware_ver;
			control_table_message.id=servo_to_update->id;
			control_table_message.baud=servo_to_update->baud;
			control_table_message.return_delay_time=servo_to_update->return_delay_time;
			control_table_message.cw_angle_limit=servo_to_update->cw_angle_limit;
			control_table_message.ccw_angle_limit=servo_to_update->ccw_angle_limit;
			control_table_message.high_temp_limit=servo_to_update->high_temp_limit;
			control_table_message.low_voltage_limit=servo_to_update->low_voltage_limit;
			control_table_message.high_voltage_limit=servo_to_update->high_voltage_limit;
			control_table_message.max_torque=servo_to_update->max_torque;
			control_table_message.status_return_level=servo_to_update->status_return_level;
			control_table_message.alarm_led=servo_to_update->alarm_led;
			control_table_message.alarm_shutdown=servo_to_update->alarm_shutdown;
			control_table_message.multi_turn_offset=servo_to_update->multi_turn_offset;
			control_table_message.resolution_divider=servo_to_update->resolution_divider;
			//--------------------------------------
			control_table_message.torque_enable=servo_to_update->torque_enable;
			control_table_message.led=servo_to_update->led;
			control_table_message.pid_d_gain=servo_to_update->pid_d_gain;
			control_table_message.pid_i_gain=servo_to_update->pid_i_gain;
			control_table_message.pid_p_gain=servo_to_update->pid_p_gain;
			control_table_message.goal_position=servo_to_update->goal_position;
			control_table_message.moving_speed=servo_to_update->moving_speed;
			control_table_message.torque_limit=servo_to_update->torque_limit;
			control_table_message.present_position=servo_to_update->present_position;
			control_table_message.present_speed=servo_to_update->present_speed;
			control_table_message.present_load=servo_to_update->present_load;
			control_table_message.present_voltage=servo_to_update->present_voltage;
			control_table_message.present_temp=servo_to_update->present_temp;
			control_table_message.instruction_registered=servo_to_update->instruction_registered;
			control_table_message.is_moving=servo_to_update->is_moving;
			control_table_message.eeprom_lock=servo_to_update->eeprom_lock;
			control_table_message.punch=servo_to_update->punch;
			control_table_message.consuming_current=servo_to_update->consuming_current;
			control_table_message.torque_control_mode=servo_to_update->torque_control_mode;
			control_table_message.goal_torque=servo_to_update->goal_torque;
			control_table_message.goal_acceleration=servo_to_update->goal_acceleration;



			control_table_request_publisher.publish(control_table_message);
		}
	}
	else
	{
		ROS_WARN("The dynamixel_servo_server wasn't originally configured to know about ID: 0x%X, so the Status Request message will be dropped.",msg->id);
		return;
	}
	return;
}

bool DynamixelServos::pingServo(uint8_t id)
{
	// Note: Regardless of whether the Broadcasting ID is used or the Status Return Level (Address 0x10) is 0, a Status Packet is always returned by the PING instruction.
	string temp = com_port.sendAndReceive(id, DynamixelSerialPort::INST_PING);

	if(com_port.comErrorPresent())
	{
		ROS_WARN("Error: %s didn't sendAndReceive to servo id: 0x%X correctly. error_returned: %s",__func__, id, temp.c_str());
		return false;
	}
	else if (temp != DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL)
	{
		// it could be just a bad status return packet from a servo that mis-behaved previously
		ROS_WARN("Error: %s didn't sendAndReceive to servo id: 0x%X correctly. error_returned: %s",__func__, id, temp.c_str());
	}
	//ROS_INFO("%s sent id: %u the packet: %s",__func__, id, com_port.printSendPacket().c_str());
	//ROS_INFO("%s received from id: %u the packet: %s",__func__,id, com_port.printReceivePacket().c_str());
	return true;
}
void DynamixelServos::configCallbackContinuousAngle(const dynamixel_servo::DynamixelContinuousAngleConfig::ConstPtr& msg)
{
	if (msg->id == 0x00)
	{
		ROS_WARN("Servo ID 0x00 is explicitly not allowed to prevent people from sending configs to the wrong servo due to uninitialized ID parameters in the messages.");
		ROS_WARN("The %s is dropping the \"continuous angle\" config message for servo ID 0x%X.", ros::this_node::getName().c_str(), msg->id);
		return;
	}
	if (binary_search(servos.begin(), servos.end(), msg->id))
	{
		vector<Servo>::iterator servo_to_config = find(servos.begin(), servos.end(), msg->id);
		servo_to_config->continuous_angle_goal = msg->goal_position;
		servo_to_config->continuous_velocity_goal = msg->goal_velocity;
		if(servo_to_config->continuous_angle_mode)
		{
			setMovingSpeed(servo_to_config, approximateSpeedControl(*servo_to_config),false); // XXX
		}
		else
		{
			ROS_WARN("Servo ID 0x%X is not currently in \"continuous angle mode\"; changing the mode in which the servo operates can clear the clockwise and counterclockwise safety limits.", msg->id);
			ROS_WARN("If you would like to change the mode it operates in, use a full config message or configure it outside of ros.");
		}
	}
	else
	{
		ROS_WARN("The dynamixel_servo_server wasn't originally configured to know about ID: 0x%X, so the \"continuous angle mode\" config message will be dropped.",msg->id);
		return;
	}
	return;

}

void DynamixelServos::configCallbackWheel(const dynamixel_servo::DynamixelWheelConfig::ConstPtr& msg)
{
	if (msg->id == 0x00)
	{
		ROS_WARN("Servo ID 0x00 is explicitly not allowed to prevent people from sending configs to the wrong servo due to uninitialized ID parameters in the messages.");
		ROS_WARN("The %s is dropping the \"wheel mode\" config message for servo ID 0x%X.", ros::this_node::getName().c_str(), msg->id);
		return;
	}
	if (binary_search(servos.begin(), servos.end(), msg->id))
	{
		vector<Servo>::iterator servo_to_config = find(servos.begin(), servos.end(), msg->id);
		if(servo_to_config->inWheelMode())
		{
			setMovingSpeed(servo_to_config, msg->moving_speed, false);
		}
		else
		{
			ROS_WARN("Servo ID 0x%X is not currently in \"wheel mode\"; changing the mode in which the servo operates can clear the clock wise and counter clockwise safty limits.", msg->id);
			ROS_WARN("If you would like to change the mode it operates in, use a full config message or configure it outside of ros.");
		}
	}
	else
	{
		ROS_WARN("The dynamixel_servo_server wasn't originally configured to know about ID: 0x%X, so the \"wheel mode\" config message will be dropped.",msg->id);
		return;
	}
	return;
}

void DynamixelServos::configCallbackJoint(const dynamixel_servo::DynamixelJointConfig::ConstPtr& msg)
{
	if(msg->id==0x00)
	{
		ROS_WARN("Servo ID 0x00 is explicitly not allowed to prevent people from sending configs to the wrong servo due to uninitialized ID parameters in the messages.");
		ROS_WARN("The %s is dropping the \"joint mode\" config message for servo ID 0x%X.",ros::this_node::getName().c_str(), msg->id);
		return;
	}
	if(binary_search(servos.begin(),servos.end(),msg->id))
	{
		vector<Servo>::iterator servo_to_config=find(servos.begin(),servos.end(),msg->id);
		setGoalPosition(servo_to_config,msg->goal_position);
	}
	else
	{
		ROS_WARN("The dynamixel_servo_server wasn't originally configured to know about ID: 0x%X, so the \"joint mode\" config message will be dropped.",msg->id);
		return;
	}
	return;
}

void DynamixelServos::configCallbackFull(const dynamixel_servo::DynamixelFullConfig::ConstPtr& msg)
{
	if(msg->id==0x00)
	{
		ROS_WARN("Servo ID 0x00 is explicitly not allowed to prevent people from sending configs to the wrong servo due to uninitialized ID parameters in the messages.");
		ROS_WARN("The %s is dropping the full config message for servo ID 0x%X.",ros::this_node::getName().c_str(), msg->id);
		return;
	}
	if(binary_search(servos.begin(),servos.end(),msg->id))
	{
		// find the relevant servo
		vector<Servo>::iterator servo_to_config=find(servos.begin(),servos.end(),msg->id);
		servo_to_config->continuous_angle_goal = msg->goal_position;
		servo_to_config->continuous_velocity_goal = msg->goal_velocity;
		setLed(servo_to_config,msg->led);
		// now that we have it see if we need to change the mode
		if(msg->control_mode==dynamixel_servo::DynamixelFullConfig::JOINT&&servo_to_config->inWheelMode())
		{
			// to go from wheel to joint we must first stop the servo set the mode then re-enable the speed
			setMovingSpeed(servo_to_config,0.0, true);
			setControlMode(servo_to_config, msg->control_mode);
			setMovingSpeed(servo_to_config,msg->moving_speed, false);


		}
		else if(servo_to_config->continuous_angle_mode==false && setControlMode(servo_to_config, msg->control_mode)==true)
		{
			// only if the mode has explicitly changed the mode should we force the moving_speed to be writen to the servo
			setMovingSpeed(servo_to_config,msg->moving_speed, true);
		}
		else
		{
			// in continuous_angle_mode dont set the mofing speed because the feedback loop (when it gets the status(s)) will set it for us- so the moving speed is changing at the rate the statuses are updated i.e. always changing.
			// if you set it here it will keep overwriting eachother and cause BIG issues
		}
		setGoalPosition(servo_to_config,msg->goal_position);
		setTorqueLimit(servo_to_config,msg->torque_limit);
		setGoalAcceleration(servo_to_config,msg->goal_acceleration);
	}
	else
	{
		ROS_WARN("The dynamixel_servo_server wasn't originally configured to know about ID: 0x%X, so the full config message will be dropped.",msg->id);
		return;
	}
	return;
}

bool DynamixelServos::setControlMode(const vector<Servo>::iterator servo_to_config, dynamixel_servo::DynamixelFullConfig::_control_mode_type  control_mode)
{	//the reason this function returns a bool is because we need to know if we actualy changed a mode to explicitly write a value to the servo. (ex. switching from joint to wheel mode needs a moving_speed explicitly set again before it will rotate)

	// don't tie up the bus needlessly with a config that is already done
	if(control_mode==dynamixel_servo::DynamixelFullConfig::JOINT&&servo_to_config->inJointMode())
	{
		ROS_DEBUG("Servo ID 0x%X is already inJointMode, %s packet is being drop to not tie up the bus",servo_to_config->getID(),__func__);
		return false;
	}
	else if(control_mode==dynamixel_servo::DynamixelFullConfig::WHEEL&&servo_to_config->inWheelMode())
	{
		ROS_DEBUG("Servo ID 0x%X is already inWheelMode, %s packet is being drop to not tie up the bus",servo_to_config->getID(),__func__);
		return false;
	}
	else if(control_mode==dynamixel_servo::DynamixelFullConfig::MULTI_TURN&&servo_to_config->inMultiTurnMode())
	{
		ROS_DEBUG("Servo ID 0x%X is already inMultiTurnMode, %s packet is being drop to not tie up the bus",servo_to_config->getID(),__func__);
		return false;
	}
	else if(control_mode==dynamixel_servo::DynamixelFullConfig::CONTINUOUS_ANGLE&&servo_to_config->continuous_angle_mode)
	{
		ROS_DEBUG("Servo ID 0x%X is already in continuious_angle_mode, %s packet is being drop to not tie up the bus",servo_to_config->getID(),__func__);
		return false;
	}
	// It doesn't look like the config has been put on the servo yet, so write it.
	dynamixel_uint16_t cw_angle_limit_temp;
	dynamixel_uint16_t ccw_angle_limit_temp;
	if(servo_to_config->inJointMode()&&(control_mode==dynamixel_servo::DynamixelFullConfig::WHEEL||control_mode==dynamixel_servo::DynamixelFullConfig::MULTI_TURN||dynamixel_servo::DynamixelFullConfig::CONTINUOUS_ANGLE))// the rest of this if statement is redundant but is here to show clarity
	{
		ROS_WARN("Servo ID 0x%X is being taken out of \"joint mode\" and will have the safety limits of 0x%04X (Clockwise) and 0x%04X (CounterClockwise) cleared!",servo_to_config->id, servo_to_config->cw_angle_limit.two_byte_value,servo_to_config->ccw_angle_limit.two_byte_value);
		servo_to_config->previous_cw_angle_limit = servo_to_config->cw_angle_limit;
		servo_to_config->previous_ccw_angle_limit = servo_to_config->ccw_angle_limit;
		if(control_mode==dynamixel_servo::DynamixelFullConfig::WHEEL||control_mode==dynamixel_servo::DynamixelFullConfig::CONTINUOUS_ANGLE)
		{
			cw_angle_limit_temp=Servo::MIN_GOAL_POSITION;
			ccw_angle_limit_temp=Servo::MIN_GOAL_POSITION;
		}
		else if(control_mode==dynamixel_servo::DynamixelFullConfig::MULTI_TURN)
		{
			cw_angle_limit_temp=Servo::MAX_GOAL_POSITION; // 4095
			ccw_angle_limit_temp=Servo::MAX_GOAL_POSITION; // 4095
		}
	}
	else if(servo_to_config->inJointMode()==false&&control_mode==dynamixel_servo::DynamixelFullConfig::JOINT)
	{
		// just to be safe, ensure that the previous limits will enable the servo to go into joint mode
		if(servo_to_config->previous_cw_angle_limit==Servo::MIN_GOAL_POSITION&&servo_to_config->previous_ccw_angle_limit==Servo::MIN_GOAL_POSITION)
		{
			cw_angle_limit_temp=Servo::MIN_GOAL_POSITION;
			ccw_angle_limit_temp=Servo::MAX_GOAL_POSITION;
		}
		else if(servo_to_config->previous_cw_angle_limit==Servo::MAX_GOAL_POSITION&&servo_to_config->previous_ccw_angle_limit==Servo::MAX_GOAL_POSITION)
		{
			cw_angle_limit_temp=Servo::MIN_GOAL_POSITION;
			ccw_angle_limit_temp=Servo::MAX_GOAL_POSITION;
		}
		else
		{
			// it should be safe to restore the previous configs, so do so.
			cw_angle_limit_temp=servo_to_config->previous_cw_angle_limit;
			ccw_angle_limit_temp=servo_to_config->previous_ccw_angle_limit;
		}
		ROS_WARN("Servo ID 0x%X is being put in \"joint mode\" and will now have the safety limits of 0x%04X (Clockwise) and 0x%04X (CounterClockwise)!",servo_to_config->id, cw_angle_limit_temp.two_byte_value, ccw_angle_limit_temp.two_byte_value);
	}
	vector<uint8_t> params;
	params.push_back(Servo::CW_ANGLE_LIMIT_EPROM);
	params.push_back(cw_angle_limit_temp.bytes.low_byte);
	params.push_back(cw_angle_limit_temp.bytes.hi_byte);
	params.push_back(ccw_angle_limit_temp.bytes.low_byte);
	params.push_back(ccw_angle_limit_temp.bytes.hi_byte);

	string temp = com_port.sendAndReceive(servo_to_config->getID(), DynamixelSerialPort::INST_WRITE, params);

	if (temp != DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL)
	{
	     ROS_WARN("Error: %s didn't sendAndReceive to servo %u correctly. error_returned: %s",__func__, servo_to_config->getID(), temp.c_str());
	     return false;
	}
	else
	{
		servo_to_config->cw_angle_limit=cw_angle_limit_temp;
		servo_to_config->ccw_angle_limit=ccw_angle_limit_temp;
		// Modify the servo struct accordingly since communication was successful
		if (control_mode == dynamixel_servo::DynamixelFullConfig::CONTINUOUS_ANGLE)
		{
			servo_to_config->continuous_angle_mode=true;
		}
	}
	//ROS_INFO("%s sent id: %u the packet: %s",__func__, servo_to_config->getID(), com_port.printSendPacket().c_str());
	//ROS_INFO("%s received from id: %u the packet: %s",__func__,servo_to_config->getID(), com_port.printReceivePacket().c_str());
	return true;
}

void DynamixelServos::setTorqueLimit(const vector<Servo>::iterator servo_to_config, uint16_t torque_limit)
{
	if(servo_to_config->torque_limit==torque_limit)
	{
		// Don't tie up the bus if it is known to be the same value (the bus is slower than a few cpu instructions)
		ROS_DEBUG("Servo 0x%X already has its %s set to 0x%04X, dropping packet.", servo_to_config->getID(), __func__, servo_to_config->torque_limit.two_byte_value );
		return;
	}
	else if(torque_limit==Servo::OFF)
	{
		// An uninitialized message will have a default value of 0, which would effectively shutdown the servo; it isn't common to want to shutdown a servo, thus we will protect against this.
		ROS_WARN("Servo id: 0x%X is explicitly not allowed to set \"Torque Limit\" to 0x%X to prevent people from unintentionally disabling a servo due to uninitialized parameters in a messages.", servo_to_config->getID(), torque_limit);
		ROS_WARN("The %s is dropping the set \"Torque Limit\" message for servo ID 0x%X.",ros::this_node::getName().c_str(), servo_to_config->getID());
		return;
	}

	dynamixel_uint16_t torque_limit_params=torque_limit;
	vector<uint8_t> params;
	params.push_back(Servo::TORQUE_LIMIT_REG);
	if(torque_limit > servo_to_config->getMaxTorqueDefaultValue())
	{
		// When the power is first turned on, the Dynamixel "Torque Limit" reg (ram address 0x22 and 0x23) is initially set to the same value in "Max Torque" (eprom address 0x0E and 0x0F)
		//		It is not an error if the Torque Limit reg exceeds the initial value attained from Max Torque, so we will just inform ros.
		ROS_INFO("Servo %u will be configured with a \"Torque Limit\" (0x%X) larger than the initial default \"Max Torque\" (0x%X)- this is technically allowed", servo_to_config->getID(), torque_limit, servo_to_config->getMaxTorqueDefaultValue());
	}
	else if(torque_limit > servo_to_config->getMaxTorqueAllowed())
	{
		ROS_WARN("Error the value 0x%X is larger than the maximum value (0x%x) allowed for \"Torque Limit\" on servo id: 0x%X", torque_limit, servo_to_config->getMaxTorqueAllowed(), servo_to_config->getID());
		return;
	}

	// Since TORQUE_LIMIT_REG is the address of the lowest byte of Torque Limit, the low byte of torque_limit_params must be pushed back first then the high byte.
	params.push_back(torque_limit_params.bytes.low_byte);
	params.push_back(torque_limit_params.bytes.hi_byte);

	string temp = com_port.sendAndReceive(servo_to_config->getID(), DynamixelSerialPort::INST_WRITE, params);

	if (temp != DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL)
	{
	     ROS_WARN("Error: %s didn't sendAndReceive to servo %u correctly. error_returned: %s",__func__, servo_to_config->getID(), temp.c_str());
	}
	else
	{
		// Modify the servo struct accordingly since communication was successful
		servo_to_config->torque_limit=torque_limit_params;
	}
	//ROS_INFO("%s sent id: %u the packet: %s",__func__, servo_to_config->getID(), com_port.printSendPacket().c_str());
	//ROS_INFO("%s received from id: %u the packet: %s",__func__,servo_to_config->getID(), com_port.printReceivePacket().c_str());
	return;
}

void DynamixelServos::setGoalAcceleration(const vector<Servo>::iterator servo_to_config, float goal_acceleration_rad_per_second_squared)
{
	if(goal_acceleration_rad_per_second_squared<0.0 || goal_acceleration_rad_per_second_squared>((8.5826772*PI/180)*Servo::MAX_ACCELERATION))
	{
		ROS_WARN("The \"goal_acceleration\" should be specified in radians per second squared, rad/(s^2), between [0.0 to 2*%f] for id: 0x%X",(float)((8.5826772*PI/180)*Servo::MAX_ACCELERATION), servo_to_config->getID());
		return;
	}
	float goal_acceleration_deg_per_second_squared=goal_acceleration_rad_per_second_squared*180/PI;
	uint8_t goal_acceleration=(uint8_t)(goal_acceleration_deg_per_second_squared/8.5826772);
	if(goal_acceleration<Servo::MIN_ACCELERATION)
	{
		goal_acceleration=Servo::MIN_ACCELERATION;
	}
	else if(goal_acceleration>Servo::MAX_ACCELERATION)
	{
		goal_acceleration=Servo::MAX_ACCELERATION;
	}

	if(servo_to_config->goal_acceleration==goal_acceleration)
	{
		// Don't tie up the bus if it is known to be the same value (the bus is slower than a few cpu instructions)
		ROS_DEBUG("Servo 0x%X already has its %s set to %f rad/s^2 (0x%02X), dropping packet.", servo_to_config->getID(), __func__, goal_acceleration_rad_per_second_squared, servo_to_config->goal_acceleration );
		return;
	}

	vector<uint8_t> params;
	params.push_back(Servo::GOAL_ACCELERATION_REG);
	if(goal_acceleration>Servo::MAX_ACCELERATION)
	{
		ROS_WARN("Error the value 0x%X is larger than the maximum value (0x%x) allowed for \"Goal Acceleration\" on servo id %u", goal_acceleration, Servo::MAX_ACCELERATION, servo_to_config->getID());
		return;
	}
	else if(goal_acceleration==0x00)
	{
		ROS_INFO("Servo %u will be configured with a \"Goal Acceleration\" of 0x%X, which equates to the maximum acceleration of the motor- please make sure this is the desired behavior.", servo_to_config->getID(), goal_acceleration);
	}
	params.push_back(goal_acceleration);

	string temp = com_port.sendAndReceive(servo_to_config->getID(), DynamixelSerialPort::INST_WRITE, params);
	if (temp != DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL)
	{
		ROS_WARN("Error: %s didn't sendAndReceive to servo %u correctly. error_returned: %s",__func__, servo_to_config->getID(), temp.c_str());
	}
	else
	{
		// Modify the servo struct accordingly since communication was successful
		servo_to_config->goal_acceleration=goal_acceleration;
	}
	//ROS_INFO("%s sent id: %u the packet: %s",__func__, servo_to_config->getID(), com_port.printSendPacket().c_str());
	//ROS_INFO("%s received from id: %u the packet: %s",__func__,servo_to_config->getID(), com_port.printReceivePacket().c_str());
	return;
}

void DynamixelServos::setLed(const vector<Servo>::iterator servo_to_config, uint8_t led_state)
{
	if(servo_to_config->led==led_state)
	{
		// Don't tie up the bus if it is known to be the same value (the bus is slower than a few cpu instructions)
		ROS_DEBUG("Servo 0x%X already has its %s set to 0x%02X, dropping packet.", servo_to_config->getID(), __func__, led_state );
		return;
	}
	vector<uint8_t> params;
	params.push_back(Servo::LED_REG);

  if (led_state == Servo::ON)
  {
    // if led_status==true==1=="ON" then turn it on
    params.push_back(Servo::ON);
  }
  else if (led_state == Servo::OFF)
  {
    // if led_status==false==0=="OFF" then turn it off
    params.push_back(Servo::OFF);
  }
  else
  {
    ROS_WARN("Error id: 0x%X invalid led state parameter (0x%X) given to setLed", servo_to_config->getID(), led_state);
    return;
  }
  string temp = com_port.sendAndReceive(servo_to_config->getID(), DynamixelSerialPort::INST_WRITE, params);

  if (temp != DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL)
  {
    ROS_WARN("Error: %s didn't sendAndReceive to servo %u correctly. error_returned: %s",__func__, servo_to_config->getID(), temp.c_str());
  }
  else
  {
	  // Modify the servo struct accordingly since communication was successful
	  servo_to_config->led=led_state;
  }
  //ROS_INFO("%s sent id: %u the packet: %s",__func__, servo_to_config->getID(), com_port.printSendPacket().c_str());
  //ROS_INFO("%s received from id: %u the packet: %s",__func__,servo_to_config->getID(), com_port.printReceivePacket().c_str());
  return;
}

void DynamixelServos::setGoalPosition(const vector<Servo>::iterator servo_to_config, float goal_position_radians)
{
	// Note: the following conversions have implicit casting involved in each division operation.
	float goal_position_degrees=goal_position_radians*(180/PI); // see: http://goo.gl/0t2IRV
	uint16_t goal_position;

	if(servo_to_config->inJointMode())
	{
		if(goal_position_radians<0.0||goal_position_radians>2*PI)
		{
			// since goal_position is in radians it must be between 0 and 2*PI
			ROS_WARN("The \"goal_position\" for id: 0x%X should be specified in radians between [0.0 to 2*%f]",servo_to_config->getID(), PI);
			return;
		}
		goal_position = (uint16_t) (goal_position_degrees/(360.0/Servo::ENCODER_RESOLUTION)); // only the result of the math operation is explicitly cast to a uint16
		// verify there weren't any odd issues due to casting, and clip if need be.
		if(goal_position>Servo::MAX_GOAL_POSITION)
		{
			goal_position=Servo::MAX_GOAL_POSITION;
		}
		else if (goal_position<Servo::MIN_GOAL_POSITION)
		{
			goal_position=Servo::MIN_GOAL_POSITION;
		}
	}
	else if(servo_to_config->inMultiTurnMode())
	{
		ROS_WARN("MultiTurnMode hasn't fully been tested yet, or implemented! Thus we are not setting it.");
		return;
	}
	else if(servo_to_config->continuous_angle_mode)
	{
		// NOTE: As long as the server can read the status from the dynamixels fast enough, the angle will be achieved.
		servo_to_config->continuous_angle_goal=goal_position_radians;
		return;
	}
	else if(servo_to_config->inWheelMode())
	{
		ROS_DEBUG("Setting the \"goal_position\" to %f for id: 0x%X while in \"wheel mode\" has no affect.",goal_position_radians, servo_to_config->getID());
		return;
	}

	if(servo_to_config->goal_position==goal_position)
	{
		// Don't tie up the bus if it is known to be the same value (the bus is slower than a few cpu instructions)
		ROS_DEBUG("Servo 0x%X already has its %s set to %f radians (0x%04X), dropping packet.", servo_to_config->getID(), __func__, goal_position_radians, servo_to_config->goal_position.two_byte_value );
		return;
	}

	dynamixel_uint16_t goal_position_params=goal_position;
	vector<uint8_t> params;
	// Note: Goal position is a two byte register, and (per the sdk) both bytes should be set with a single packet.
	// So to write to the two addresses with one Instruction Packet- at the same time- we write at the lower address (GOAL_POSITION_REG).
	params.push_back(Servo::GOAL_POSITION_REG);
	if(servo_to_config->inJointMode())
	{
		// The goal position we wish to set must be in the range of cw_angle_limit and ccw_angle_limit when in joint mode
		if(goal_position_params<servo_to_config->getMinAngle() || goal_position_params>servo_to_config->getMaxAngle())
		{
			ROS_WARN("Error invalid goal position parameter (0x%04X) specified for Servo %u; it's out of the allowed clockwise (0x%04X) and counterclockwise (0x%04X) limits set while in \"Joint Mode\".", goal_position, servo_to_config->getID(), servo_to_config->getMinAngle(), servo_to_config->getMaxAngle());
			return;
		}
	}
	// Since GOAL_POSITION_REG is the address of the lowest byte of Goal Position, the low byte of goal_position_params must be pushed back first then the high byte.
	params.push_back(goal_position_params.bytes.low_byte);
	params.push_back(goal_position_params.bytes.hi_byte);

	string temp = com_port.sendAndReceive(servo_to_config->getID(), DynamixelSerialPort::INST_WRITE, params);

	if (temp != DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL)
	{
	    ROS_WARN("Error: %s didn't sendAndReceive to servo %u correctly. error_returned: %s",__func__, servo_to_config->getID(), temp.c_str());
	}
	else
	{
		// Modify the servo struct accordingly since communication was successful
		servo_to_config->goal_position=goal_position_params;
	}
	//ROS_INFO("%s sent id: %u the packet: %s",__func__, servo_to_config->getID(), com_port.printSendPacket().c_str());
	//ROS_INFO("%s received from id: %u the packet: %s",__func__,servo_to_config->getID(), com_port.printReceivePacket().c_str());
	return;
}

void DynamixelServos::setMovingSpeed(const vector<Servo>::iterator servo_to_config, float moving_speed_rad_per_sec, bool force_update)
{	// NOTE: setMovingSpeed has the extra third boolean (force_update) parameter because when you change from modes (like joint to wheel mode) you have to explicitly tell the servo to move again at the speed you want by sending it a moving speed.

	// 1 rad/s is approximately 9.5493 rpm, conversely .1047 rad/s is 1 rpm. see: http://goo.gl/52oeiy
	// The Dynamixel's moving speed has an approximate unit of .11443 rpm, given its resolution of 1023. Thus it can essential move +- 12.25869244063695 rad/s in wheel mode
	// the compiler will inline the folowing statement, but it is here to show the conversion
	static const float moving_speed_max=Servo::MOVING_SPEED_RESOLUTION*Servo::MOVING_SPEED_PER_UNIT_IN_RPM*Servo::RPM_TO_RAD_PER_SECOND;
	if(servo_to_config->inJointMode()||servo_to_config->inMultiTurnMode())
	{
		if(moving_speed_rad_per_sec<0.0||(moving_speed_rad_per_sec>moving_speed_max))
		{
			ROS_WARN("The \"moving_speed\" for id: 0x%X should be specified in radians per second between [0 to %f] not %f for joint and multi-turn mode",servo_to_config->getID(), moving_speed_max,moving_speed_rad_per_sec);
			return;
		}
	}
	else if(servo_to_config->inWheelMode())
	{
		if(moving_speed_rad_per_sec<-moving_speed_max||moving_speed_rad_per_sec>moving_speed_max)
		{
			ROS_WARN("The \"moving_speed\" for id: 0x%X should be specified in radians per second between [%f to %f] not %f for wheel mode",servo_to_config->getID(), -moving_speed_max, moving_speed_max, moving_speed_rad_per_sec);
			return;
		}
	}
	else
	{
		ROS_WARN("Can't set the \"moving_speed\" for id: 0x%X because it's in an unknown mode",servo_to_config->getID());
		return;
	}

	uint16_t moving_speed;
	if (moving_speed_rad_per_sec==0)
	{
		moving_speed=servo_to_config->getZeroMovingSpeed();
		if(servo_to_config->inJointMode())
		{
			ROS_WARN("Servo 0x%X is inJointMode, which doesn't have the ability to set a ZeroMovingSpeed since it is based off of position, thus it will be set to approximately ZeroMovingSpeed (0x%04X).",servo_to_config->getID(), moving_speed);
		}
	}
	else if(moving_speed_rad_per_sec>0.0)
	{
		// according to rep 103 ccw is positive
		moving_speed=Servo::WHEEL_MODE_MOVING_SPEED_ZERO_CCW+(uint16_t)((moving_speed_rad_per_sec*Servo::RAD_PER_SECOND_TO_RPM)/Servo::MOVING_SPEED_PER_UNIT_IN_RPM);
	}
	else if(servo_to_config->inWheelMode()&&moving_speed_rad_per_sec<0.0)
	{
		// According to ros rep 103 cw is in the negative direction hence the following conversion
		moving_speed=Servo::WHEEL_MODE_MOVING_SPEED_ZERO_CW+(uint16_t)((-moving_speed_rad_per_sec*Servo::RAD_PER_SECOND_TO_RPM)/Servo::MOVING_SPEED_PER_UNIT_IN_RPM);
	}

	if(servo_to_config->moving_speed==moving_speed&&force_update==false)
	{
		// Don't tie up the bus if it is known to be the same value (the bus is slower than a few cpu instructions)
		ROS_DEBUG("Servo 0x%X already has its %s set to %f rad/s (0x%04X), dropping packet.", servo_to_config->getID(), __func__, moving_speed_rad_per_sec, servo_to_config->moving_speed.two_byte_value );
		return;
	}
	else if(servo_to_config->moving_speed==moving_speed&&force_update==true)
	{
		ROS_DEBUG("Servo 0x%X already has its %s set to %f rad/s (0x%04X), but the moving speed will explicitly be set again.", servo_to_config->getID(), __func__, moving_speed_rad_per_sec, servo_to_config->moving_speed.two_byte_value );
	}

	dynamixel_uint16_t moving_speed_params=moving_speed;
	vector<uint8_t> params;
	// Note: Moving speed is a two byte register, and (per the sdk) both bytes should be set with a single packet.
	// So to write to the two addresses with one Instruction Packet- at the same time- we write at the lower address (MOVING_SPEED_REG).
	params.push_back(Servo::MOVING_SPEED_REG);
	if(servo_to_config->inJointMode()&&moving_speed>Servo::MAX_JOINT_MODE_MOVING_SPEED)
	{
		ROS_WARN("Error id: 0x%X invalid \"Moving Speed\" parameter (0x%04X) exceeds the max value allowed (0x%04X) for \"Joint Mode\"", servo_to_config->getID(), moving_speed, Servo::MAX_JOINT_MODE_MOVING_SPEED);
		return;
	}
	else if(servo_to_config->inMultiTurnMode()&&moving_speed>Servo::MAX_MULTTURN_MODE_MOVING_SPEED)
	{
		ROS_WARN("Error id: 0x%X invalid \"Moving Speed\" parameter (0x%04X) exceeds the max value allowed (0x%04X) for \"Mult-turn Mode\"", servo_to_config->getID(), moving_speed, Servo::MAX_MULTTURN_MODE_MOVING_SPEED);
		return;

	}
	else if(servo_to_config->inWheelMode()&&moving_speed>Servo::MAX_WHEEL_MODE_MOVING_SPEED)
	{
		ROS_WARN("Error id: 0x%X invalid \"Moving Speed\" parameter (0x%04X) exceeds the max value allowed (0x%04X) for \"Wheel Mode\"", servo_to_config->getID(), moving_speed, Servo::MAX_WHEEL_MODE_MOVING_SPEED);
		return;
	}
	else if(servo_to_config->inWheelMode())
	{
		if(moving_speed==Servo::WHEEL_MODE_MOVING_SPEED_ZERO_CW||moving_speed==Servo::WHEEL_MODE_MOVING_SPEED_ZERO_CCW)
		{
			ROS_INFO("Servo %u will be stopped because it is in \"Wheel Mode\" and its \"Moving Speed\" is being set to 0x%04X, which is the minimum value for the %s direction",servo_to_config->getID(), moving_speed, (moving_speed==Servo::WHEEL_MODE_MOVING_SPEED_ZERO_CW?"clockwise":"counterclockwise"));
		}
		else if(moving_speed>servo_to_config->getMaxMovingSpeed()&&servo_to_config->rotatingCounterClockWise())
		{
			// remember the bit position is 0 indexed
			ROS_INFO("Servo %u will change its direction of rotation (CCW to CW) since it's in \"Wheel Mode\" and the 10th bit is changing; \"Moving Speed\" is being set to %f (0x%04X)",servo_to_config->getID(),moving_speed_rad_per_sec, moving_speed);
		}
		else if(moving_speed<servo_to_config->getMinMovingSpeed()&&servo_to_config->rotatingClockWise())
		{
			ROS_INFO("Servo %u will change its direction of rotation (CW to CCW) since it's in \"Wheel Mode\" and the 10th bit is changing; \"Moving Speed\" is being set to %f (0x%04X)",servo_to_config->getID(),moving_speed_rad_per_sec, moving_speed);
		}
	}
	// Since MOVING_SPEED_REG is the address of the lowest byte of Moving Speed, the low byte of moving_speed_params must be pushed back first then the high byte.
	params.push_back(moving_speed_params.bytes.low_byte);
	params.push_back(moving_speed_params.bytes.hi_byte);

	string temp = com_port.sendAndReceive(servo_to_config->getID(), DynamixelSerialPort::INST_WRITE, params);

	if (temp != DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL)
	{
	    ROS_WARN("Error: %s didn't sendAndReceive to servo %u correctly. error_returned: %s",__func__, servo_to_config->getID(), temp.c_str());
	}
	else
	{
		// Modify the servo struct accordingly since communication was successful
		servo_to_config->moving_speed=moving_speed_params;
	}
	//ROS_INFO("%s sent id: %u the packet: %s",__func__, servo_to_config->getID(), com_port.printSendPacket().c_str());
	//ROS_INFO("%s received from id: %u the packet: %s",__func__,servo_to_config->getID(), com_port.printReceivePacket().c_str());
	return;
}

vector<uint8_t> DynamixelServos::servoRead(uint8_t id, uint8_t location, uint8_t num_bytes_to_read)
{
	vector<uint8_t> return_vector;
	vector<uint8_t> params;
	params.push_back(location);
	if(num_bytes_to_read>DynamixelSerialPort::MAXNUM_NUM_RX_PARAMS)
	{
		ROS_WARN("Error: %u bytes exceeds the max amount (%u bytes) that can be read from servo %u", num_bytes_to_read, DynamixelSerialPort::MAXNUM_NUM_RX_PARAMS, id);
		return return_vector;
	}
	else
	{
		params.push_back(num_bytes_to_read);
	}

	string temp = com_port.sendAndReceive(id, DynamixelSerialPort::INST_READ, params);

	if (com_port.comErrorPresent())
	{
		ROS_WARN("Error: %s didn't sendAndReceive to servo %u correctly. error_returned: %s", __func__, id, temp.c_str());
		return return_vector;
	}
	else if(temp != DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL)
	{
		// could be just a bad status message returned
		ROS_WARN("Error: %s didn't sendAndReceive to servo %u correctly. error_returned: %s", __func__, id, temp.c_str());
	}
	// ROS_INFO("%s sent id: %u the packet: %s", __func__, id, com_port.printSendPacket().c_str());
	// ROS_INFO("%s received from id: %u the packet: %s", __func__, id, com_port.printReceivePacket().c_str());

	return com_port.getReturnPacketParamaters();
}

float DynamixelServos::approximateSpeedControl(Servo & servo)
{
	float position_error = servo.continuous_angle_goal - servo.current_continuous_position_in_radians;
	
	float desired_velocity = 3 * position_error + servo.continuous_velocity_goal;
	if(fabs(desired_velocity) > Servo::MAX_MOVING_SPEED_IN_RAD_SEC*.99) {
		desired_velocity *= Servo::MAX_MOVING_SPEED_IN_RAD_SEC*.99/fabs(desired_velocity);
	}
	//std::cout << position_error << " " << desired_velocity << std::endl;
	return desired_velocity;
}


void DynamixelServos::stopAllServos()
{
	for(vector<Servo>::iterator i = servos.begin(); i != servos.end(); i++)
	{
		setMovingSpeed(i, 0, true);
	}
}


#endif // DYNAMIXEL_CLASS_H_
