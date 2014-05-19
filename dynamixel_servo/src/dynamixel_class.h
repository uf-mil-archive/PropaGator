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
#include "dynamixel_servo/DynamixelConfigParam.h" //message config parameters
#include "dynamixel_servo/DynamixelStatusParam.h" //message status return parameters
#include "dynamixel_servo/DynamixelControlTableRequest.h" //message control table request
#include "dynamixel_servo/DynamixelControlTableParam.h" //message control table return paramaters

using namespace std;

#define DYNAMIXEL_CLASS_DEBUG

class DynamixelServos
{
private:

	vector<Servo> servos;
	DynamixelSerialPort com_port;
	ros::NodeHandle n;
	ros::Publisher status_pub;
	ros::Subscriber config_subscriber;
	ros::Subscriber control_table_request_subscriber;
	ros::Publisher control_table_request_publisher;
	double poll_rate;
	static const string DEFAULT_PARAM_SERVER_DYNAMIXEL_NAMESPACE;
	static const string DEFAULT_PARAM_SERVER_SERVO_NAMESPACE;

public:
	DynamixelServos();
	~DynamixelServos();
	void run();

protected:
	bool pingServo(uint8_t id);
	void controlTableRequestCallback(const dynamixel_servo::DynamixelControlTableRequest::ConstPtr& msg);
	void configCallback(const dynamixel_servo::DynamixelConfigParam::ConstPtr& msg);

	void setTorqueEnable(uint8_t id, uint8_t torque_state);
	void setLed(uint8_t id, uint8_t led_state);
	void setGoalPosition(uint8_t id, uint16_t goal_position);
	void setMovingSpeed(uint8_t id, uint16_t moving_speed);

	vector<uint8_t> servoRead(uint8_t id, uint8_t location, uint8_t num_bytes_to_read);

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

	// Now that everything is setup on the server we will allow subscription and advertise
	status_pub= n.advertise<dynamixel_servo::DynamixelStatusParam>("dynamixel_status_post", 1000);
	//ControlTableRequest
	control_table_request_subscriber= n.subscribe("dynamixel_control_table_request", 1000, &DynamixelServos::controlTableRequestCallback, this);
	control_table_request_publisher=n.advertise<dynamixel_servo::DynamixelControlTableParam>("dynamixel_control_table_post",1000);
	config_subscriber= n.subscribe("dynamixel_config", 1000, &DynamixelServos::configCallback,this);

}

void DynamixelServos::run()
{

	// there must be an active handle on the com_port to communicate with the servo(s) in subsequent callbacks
	if(com_port.isOpen()==true)
	{
		ROS_INFO("%s is running with a handle on port: %s",ros::this_node::getName().c_str(),com_port.getPortName().c_str());

		ros::Rate loop_rate(poll_rate);
		vector<uint8_t> read_return;
		dynamixel_servo::DynamixelStatusParam status_message;

		boost::shared_ptr<dynamixel_servo::DynamixelControlTableRequest> test=boost::make_shared<dynamixel_servo::DynamixelControlTableRequest>();
		//dynamixel_servo::DynamixelControlTableRequest test;
		test->id=2;
		controlTableRequestCallback(test);

		// We will poll the servo(s) based on the loop rate; so because we need to do that work we will use the "while(ros::ok)" paradigm.
		while(ros::ok())
		{
			// Note: The servo(s) will not be update with the posting of each status message in order to keep this routine relatively fast.
			for(vector<Servo>::iterator i=servos.begin();i!=servos.end();i++)
			{
				// Do a block read from the servo's (contiguous) ram- which is faster than multiple read request for each register.
				// 		The block of memory that is associated with a status post is from the [GOAL_POSITION_REG to the IS_MOVING_REG]. Note: don't forget that it's inclusive, hence the plus 1.
				read_return=servoRead(i->getID(), Servo::GOAL_POSITION_REG, (Servo::IS_MOVING_REG-Servo::GOAL_POSITION_REG+1));
				// verify that it read the correct number of bytes- note: the size of the vector returned on an error will be 0.
				if(read_return.size()==(Servo::IS_MOVING_REG-Servo::GOAL_POSITION_REG+1))
				{
					status_message.id=i->getID();
					status_message.goal_position=dynamixel_uint16_t(read_return[1],read_return[0]);
					status_message.moving_speed=dynamixel_uint16_t(read_return[3],read_return[2]);
					status_message.torque_limit=dynamixel_uint16_t(read_return[5],read_return[4]);
					status_message.present_position=dynamixel_uint16_t(read_return[7],read_return[6]);
					status_message.present_speed=dynamixel_uint16_t(read_return[9],read_return[8]);
					status_message.present_load=dynamixel_uint16_t(read_return[11],read_return[10]);
					status_message.present_voltage=read_return[12];
					status_message.present_temp=read_return[13];
					status_message.registered=read_return[14];
					// Note: Byte 45 (0x2D) isn't in the ram, this corresponds to byte 15 in the read_return here,
					status_message.is_moving=read_return[16];
					status_pub.publish(status_message);
				}
				else
				{
					ROS_WARN("Can't post status message for id: 0x%X due to bad read request", i->getID());
				}
			}
			ros::spinOnce();
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
	// Ensure that the com_port is closed when the variable goes out of scope, remember this can happen when the ros node is killed.
	if(com_port.isOpen()==true)
	{
		// Note: fprintf is used here instead of ROS_INFO because ROS_INFO does not run once rosnode kill is called (ctrl+c).
		fprintf(stdout,"%s is closing its handle on port %s.\n",ros::this_node::getName().c_str(),com_port.getPortName().c_str());
		com_port.closePort();
	}
}

//control_table_request
void DynamixelServos::controlTableRequestCallback(const dynamixel_servo::DynamixelControlTableRequest::ConstPtr& msg)
{
	if(msg->id==0x00)
	{
		ROS_WARN("Servo ID 0x00 is explicitly not allowed to prevent people from requesting info from the wrong servo due to uninitialized ID parameters in the messages.");
		ROS_WARN("The dynamixel_servo_server is dropping the Status Request message for servo ID 0x%X.",msg->id);
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
			// For debugging purposes, we will actually fill in the servo structs with their relevant data
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
			dynamixel_servo::DynamixelControlTableParam control_table_message;
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

	ROS_INFO("%s sent id: %u the packet: %s",__func__, id, com_port.printSendPacket().c_str());
	ROS_INFO("%s received from id: %u the packet: %s",__func__,id, com_port.printReceivePacket().c_str());

	if (temp != DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL)
	{
		ROS_WARN("Error: %s didn't sendAndReceive to servo %u correctly. error_returned: %s",__func__, id, temp.c_str());
		return false;
	}

	return true;
}

void DynamixelServos::configCallback(const dynamixel_servo::DynamixelConfigParam::ConstPtr& msg)
{
	if(msg->id==0x00)
	{
		ROS_WARN("Servo ID 0x00 is explicitly not allowed to prevent people from sending configs to the wrong servo due to uninitialized ID parameters in the messages.");
		ROS_WARN("The dynamixel_servo_server is dropping the config message for servo ID 0x%X.",msg->id);
		return;
	}
	if(binary_search(servos.begin(),servos.end(),msg->id))
	{
		//setLed(msg->id,msg->led);
		setGoalPosition(msg->id,msg->goal_position);
	}
	else
	{
		ROS_WARN("The dynamixel_servo_server wasn't originally configured to know about ID: 0x%X, so the config message will be dropped.",msg->id);
		return;
	}
	return;
}

void DynamixelServos::setTorqueEnable(uint8_t id, uint8_t torque_state)
{
	vector<uint8_t> params;
	params.push_back(Servo::TORQUE_ENABLE_REG);
	if(torque_state==Servo::ON)
	{
		// Setting the value to 1 enables the torque
		// if torque_state==true==1==ON then turn it on
		params.push_back(Servo::ON);
	}
	else if(torque_state==Servo::OFF)
	{
		// When the power is first turned on, the Dynamixel actuator enters the Torque Free Run condition (zero torque).
		// if torque_state==false==0==OFF then turn it off
		params.push_back(Servo::OFF);
	}
	else
	{
		ROS_WARN("Error invalid torque_state parameter (0x%X) given to setTorqueEnable", torque_state);
		return;
	}
	string temp = com_port.sendAndReceive(id, DynamixelSerialPort::INST_WRITE, params);

	if (temp != DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL)
	{
	     ROS_WARN("Error: %s didn't sendAndReceive to servo %u correctly. error_returned: %s",__func__, id, temp.c_str());
	}
	ROS_INFO("%s sent id: %u the packet: %s",__func__, id, com_port.printSendPacket().c_str());
	ROS_INFO("%s received from id: %u the packet: %s",__func__,id, com_port.printReceivePacket().c_str());
	return;
}

void DynamixelServos::setLed(uint8_t id, uint8_t led_state)
{
  vector<uint8_t> params;
  params.push_back(Servo::LED_REG);
  if (led_state == Servo::ON)
  {
    // if led_status==true==1==ON then turn it on
    params.push_back(Servo::ON);
  }
  else if (led_state == Servo::OFF)
  {
    // if led_status==false==0==OFF then turn it off
    params.push_back(Servo::OFF);
  }
  else
  {
    ROS_WARN("Error invalid led state parameter (0x%X) given to setLed", led_state);
    return;
  }
  string temp = com_port.sendAndReceive(id, DynamixelSerialPort::INST_WRITE, params);

  if (temp != DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL)
  {
    ROS_WARN("Error: %s didn't sendAndReceive to servo %u correctly. error_returned: %s",__func__, id, temp.c_str());
  }
  ROS_INFO("%s sent id: %u the packet: %s",__func__, id, com_port.printSendPacket().c_str());
  ROS_INFO("%s received from id: %u the packet: %s",__func__,id, com_port.printReceivePacket().c_str());
  return;
}

void DynamixelServos::setGoalPosition(uint8_t id, uint16_t goal_position)
{
	dynamixel_uint16_t goal_position_params=goal_position;
	vector<uint8_t> params;
	// Note: Goal position is a two byte register, and (per the sdk) both bytes should be set with a single packet.
	// So to write to the two addresses with one Instruction Packet- at the same time- we write at the lower address (GOAL_POSITION_REG).
	params.push_back(Servo::GOAL_POSITION_REG);
	if(goal_position_params.two_byte_value>= Servo::MIN_GOAL_POSITION && goal_position_params.two_byte_value<= Servo::MAX_GOAL_POSITION)// must be in the range of 0x0000 and 0x0FFF
	{
		// Since GOAL_POSITION_REG is the address of the lowest byte of Goal Position, the low bytes of goal_position must be pushed back first then the high bytes.
		params.push_back(goal_position_params.bytes.low_byte);
		params.push_back(goal_position_params.bytes.hi_byte);
	}
	else
	{
		ROS_WARN("Error invalid goal position parameter (0x%04X) is out of the allowed range (0x%04X - 0x%04X)", goal_position_params.two_byte_value, Servo::MIN_GOAL_POSITION, Servo::MAX_GOAL_POSITION);
		return;
	}
	string temp = com_port.sendAndReceive(id, DynamixelSerialPort::INST_WRITE, params);

	if (temp != DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL)
	{
	    ROS_WARN("Error: %s didn't sendAndReceive to servo %u correctly. error_returned: %s",__func__, id, temp.c_str());
	}
	ROS_INFO("%s sent id: %u the packet: %s",__func__, id, com_port.printSendPacket().c_str());
	ROS_INFO("%s received from id: %u the packet: %s",__func__,id, com_port.printReceivePacket().c_str());
	return;
}

void DynamixelServos::setMovingSpeed(uint8_t id, uint16_t moving_speed)
{
	dynamixel_uint16_t moving_speed_params=moving_speed;
	vector<uint8_t> params;
	// Note: Moving speed is a two byte register, and (per the sdk) both bytes should be set with a single packet.
	// So to write to the two addresses with one Instruction Packet- at the same time- we write at the lower address (MOVING_SPEED_REG).
	params.push_back(Servo::MOVING_SPEED_REG);



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

	if (temp != DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL)
	{
		ROS_WARN("Error: %s didn't sendAndReceive to servo %u correctly. error_returned: %s", __func__, id, temp.c_str());
		return return_vector;
	}
	// ROS_INFO("%s sent id: %u the packet: %s", __func__, id, com_port.printSendPacket().c_str());
	// ROS_INFO("%s received from id: %u the packet: %s", __func__, id, com_port.printReceivePacket().c_str());

	return com_port.getReturnPacketParamaters();
}





#endif // DYNAMIXEL_CLASS_H_
