/*
 * dynamixel_servo_definitions.h
 *
 *  Created on: Mar 7, 2014
 *      Author: stephen palecek
 */

#ifndef DYNAMIXEL_SERVO_DEFINITIONS_H_
#define DYNAMIXEL_SERVO_DEFINITIONS_H_
#include <string>
using namespace std;

// The idea behind a union is that two or more variables occupy the same memory space, and the union size is the biggest to make both variables fit in the same space.
// So since a uint16_t is made up of two uint8_t(s) we are essentially saying the same thing, except this allows us to get the individual byte values very easy.
union dynamixel_uint16_t
{
	uint16_t two_byte_value;
	struct
	{
		// A struck will fill the 2 byte space from the back to the front
		// So the low bytes must come before the high bytes. This way when they are mapped out they correspond to the correct locations.
		uint8_t low_byte;
		uint8_t hi_byte;
	} bytes;
	//
	dynamixel_uint16_t() : two_byte_value(0){}
	dynamixel_uint16_t(uint8_t passed_high_val, uint8_t passed_low_val){bytes.hi_byte=passed_high_val; bytes.low_byte=passed_low_val;}
	template<typename T>
	dynamixel_uint16_t(T passed_vall) : two_byte_value((uint16_t)passed_vall){}
	// uint16_t cast operator
	inline operator uint16_t&()
	{
		return two_byte_value;
	}
	/*
	// uint8_t cast operator
	inline operator uint8_t&()
	{
		return bytes.low_byte;
	}
	*/
	// assignment operator for any given type
	template<typename T>
	dynamixel_uint16_t& operator=(const T &rhs)
	{
		two_byte_value = (uint16_t)rhs;
		return *this;
	}
};



class Servo
{
	friend class DynamixelServos;
	// The operators "<" and "==" must be defined in a class to use the stl algorithm libraries; the servo's "id" will be the key used for its ranking.
	//			For more information on this see: http://www.engr.sjsu.edu/wbarrett/SortFind.htm
	// Note: if a uint8_t "id" is used as the "val" of an stl algorithm, an implicit cast will be made to compare the types. For more info see: http://goo.gl/LrzXJ0
	friend bool operator<(const Servo& left, const Servo& right){return left.id<right.id;};
	friend bool operator==(const Servo& left, const Servo& right){return left.id==right.id;};


	// below is the memory map for the servo
protected:
	//------------ Servo Identification ----------------------------------//
	string system_membership; // the system that that this servo is a member of (ex. lidar or z_drive)
	string description; // information about how the servo is a member in a given system (ex. if the servo rotated the port thruster of the z_drive system, "port" would suffice)
	//------------ Memory mapped registers for a Dynamixel Servo ---------//
	dynamixel_uint16_t model_num; // Addresses 0x00 and 0x01
	uint8_t firmware_ver; // Address 0x02
	uint8_t id; // Address 0x03
	uint8_t baud;
	uint8_t return_delay_time;
	dynamixel_uint16_t cw_angle_limit;
	dynamixel_uint16_t ccw_angle_limit;
	// note 1 byte is skipped here according the the manual
	uint8_t high_temp_limit;
	uint8_t low_voltage_limit;
	uint8_t high_voltage_limit;
	dynamixel_uint16_t max_torque;
	uint8_t status_return_level;
	uint8_t alarm_led;
	uint8_t alarm_shutdown;
	// note 1 byte is skipped here per FIRMWARE version 36
	dynamixel_uint16_t multi_turn_offset;
	uint8_t resolution_divider;
	// note 1 byte is skipped here per FIRMWARE version 36
	//----------------------------------------------------
	uint8_t torque_enable;
	uint8_t led;
	uint8_t pid_d_gain;
	uint8_t pid_i_gain;
	uint8_t pid_p_gain;
	// note 2 byte are skipped here per the manual
	dynamixel_uint16_t goal_position;
	dynamixel_uint16_t moving_speed;
	dynamixel_uint16_t torque_limit;
	dynamixel_uint16_t present_position;
	dynamixel_uint16_t present_speed;
	dynamixel_uint16_t present_load;
	uint8_t present_voltage;
	uint8_t present_temp;
	uint8_t instruction_registered;
	//note 1 byte is skipped here per the manual
	uint8_t is_moving;
	uint8_t eeprom_lock; //dont touch this, it's just here for consistency with documentation
	dynamixel_uint16_t punch;
	// note 19 byte are skipped here per the manual
	dynamixel_uint16_t consuming_current;
	uint8_t torque_control_mode;
	dynamixel_uint16_t goal_torque;
	uint8_t goal_acceleration;
public:
	// While the following may look messy, it is technical faster to initialize variables in initializer lists
	//		Also note the presence of default arguments in the class definition- allowing for more concise code.
	//				The order in which the arguments have been written for this constructor were specifically chosen on purpose. For more on default values see: http://goo.gl/MK0RZ2
	Servo(uint8_t input_id=0x00, string input_system_membership="", string input_description="", uint8_t input_baud=0x22) :
		system_membership(input_system_membership),
		description(input_description),
		// these are the default values according to the manual
		//---------------- Start of eeprom ------------------
		model_num(0x0136),
		firmware_ver(0x01),
		id(input_id), // set to 0 innately since we will only allow ids [1-253]
		baud(input_baud), // this is set to the default according to the documentation initially
		return_delay_time(0xFA),
		cw_angle_limit(0x0000),
		ccw_angle_limit(0x0FFF),
		high_temp_limit(0x50),
		low_voltage_limit(0x3C),
		high_voltage_limit(0xA0),
		max_torque(0x03FF),
		status_return_level(0x02),
		alarm_led(0x24),
		alarm_shutdown(0x24),
		multi_turn_offset(0x00),
		resolution_divider(0x01),
		//---------------- start of ram ------------------------
		torque_enable(0x00),
		led(0x00),
		pid_d_gain(0x00),
		pid_i_gain(0x00),
		pid_p_gain(0x20),
		goal_position(0x0000),//unknown until pulled
		moving_speed(0x0000),//unknown until pulled
		torque_limit(0x0000),//unknown until pulled
		present_position(0x0000),
		present_speed(0x0000),
		present_load(0x0000),
		present_voltage(0x00),
		present_temp(0x00),
		instruction_registered(0x00),
		is_moving(0x00),
		eeprom_lock(0x00), //dont touch this, it's just here for consistency with documentation
		punch(0x0000),
		consuming_current(0x0000),
		torque_control_mode(0x00),
		goal_torque(0x0000),
		goal_acceleration(0x00)
	{};
	void setID(uint8_t input_id){id=input_id;};
	uint8_t getID(){return id;};
	void setSystemMembership(string input){system_membership=input;};
	string getSystemMembership(){return system_membership;};
	void setDescription(string input){description=input;};
	string getDescription(){return description;};

	//----------- dynamixel memory map locations -----------------
	//	Note: While the address locations could be specified with a uint8_t, uint16_t is used- if required- to maintain consistency
	// EEPROM addresses will survive a power cycling of the servo and hence have been designated with "EPROM"
	static const uint16_t	MODEL_EPROM; //uint16_t worth of space occupied
	static const uint8_t	FIRMWARE_EPROM;
	static const uint8_t ID_EPROM;
	static const uint8_t BAUD_EPROM;
	static const uint8_t RETURN_DELAY_EPROM;
	static const uint16_t CW_ANGLE_LIMIT_EPROM; //uint16_t worth of space occupied
	static const uint16_t CCW_ANGLE_LIMIT_EPROM; //uint16_t worth of space occupied
	//note 1 byte is skipped here per the manual
	static const uint8_t HIGH_TEMP_LIMIT_EPROM;
	static const uint8_t LOW_VOLTAGE_LIMIT_EPROM;
	static const uint8_t HIGH_VOLTAGE_LIMIT_EPROM;
	static const uint16_t MAX_TORQUE_EPROM; //uint16_t worth of space occupied
	static const uint8_t STATUS_RETURN_LEVEL_EPROM;
	static const uint8_t ALARM_LED_EPROM;
	static const uint8_t ALARM_SHUTDOWN_EPROM;
	// note 1 byte is skipped here per FIRMWARE version 36
	static const uint16_t MULTI_TURN_OFFSET_EPROM;
	static const uint8_t RESOLUTION_DIVIDER_EPROM;
	// note 1 byte is skipped here per FIRMWARE version 36
	// RAM addresses will be reset back to their default values when the servo is power cycled.
	static const uint8_t TORQUE_ENABLE_REG;
	static const uint8_t LED_REG;
	static const uint8_t PID_D_REG;
	static const uint8_t PID_I_REG;
	static const uint8_t PID_P_REG;
	// note 2 byte are skipped here per the manual
	static const uint16_t GOAL_POSITION_REG; //this is the lowest byte of goal position. The highest byte sits at 0x1F.
	static const uint16_t MOVING_SPEED_REG; //uint16_t worth of space occupied
	static const uint16_t TORQUE_LIMIT_REG; //uint16_t worth of space occupied
	static const uint16_t PRESENT_POSITION_REG; //uint16_t worth of space occupied
	static const uint16_t PRESENT_SPEED_REG; //uint16_t worth of space occupied
	static const uint16_t PRESENT_LOAD_REG; //uint16_t worth of space occupied
	static const uint8_t PRESENT_VOLTAGE_REG;
	static const uint8_t PRESENT_TEMP_REG;
	static const uint8_t INSTRUCTION_REGISTERED_REG;
	//note 1 byte is skipped here per the manual
	static const uint8_t IS_MOVING_REG;
	static const uint8_t EEPROM_LOCK_REG; //dont touch this, it's just here for consistency with documentation
	static const uint16_t PUNCH_REG;
	// note 19 byte are skipped here per the manual
	static const uint16_t CONSUMING_CURRENT_REG;
	static const uint8_t TORQUE_CONTROL_MODE_REG;
	static const uint16_t GOAL_TORQUE_REG;
	static const uint8_t GOAL_ACCELERATION_REG;

	//----------- Dynamixel Error bit locations -----------------
	static const uint8_t ERROR_VOLTAGE_INPUT=0b00000001;
	static const uint8_t ERROR_ANGLE_LIMIT=0b00000010;
	static const uint8_t ERROR_OVERHEATING=0b00000100;
	static const uint8_t ERROR_RANGE=0b00001000;
	static const uint8_t ERROR_CHECKSUM=0b00010000;
	static const uint8_t ERROR_OVERLOAD_CURRENT=0b00100000;
	static const uint8_t ERROR_INSTRUCTION=0b01000000;

	//----------- General Dynamixel Constants -----------------
	static const uint8_t ON;
	static const uint8_t OFF;
	static const uint16_t MIN_GOAL_POSITION=0;// the minimum allowed value for the goal position
	static const uint16_t MAX_GOAL_POSITION=0xFFF; // the maximum allowed value 0xFFF (4095) to be set for the goal position;
	static const uint16_t MAX_GOAL_POSITION_AX12A=0x3FF;// Note: An AX-12A can have a MAX_GOAL_POSITION of 0x3ff
};

// Note: All of the Register Addresses must be defined outside the class to be able to take the address of the static member variable (even though they are still integral data types). If it is only used by value this isn't required.
//					Things like STL containers use a const reference (e.g. address of operator) to the variable when doing operations like push_back.
//					For more info see: http://www.stroustrup.com/bs_faq2.html#in-class or http://codewrangler.home.comcast.net/~codewrangler/tech_info/cpp_const.html
//----------- dynamixel memory map locations -----------------
const uint16_t Servo::MODEL_EPROM=0x00; //uint16_t worth of space occupied
const uint8_t Servo::FIRMWARE_EPROM=0x02;
const uint8_t Servo::ID_EPROM=0x03;
const uint8_t Servo::BAUD_EPROM=0x04;
const uint8_t Servo::RETURN_DELAY_EPROM=0x05;
const uint16_t Servo::CW_ANGLE_LIMIT_EPROM=0x06; //uint16_t worth of space occupied
const uint16_t Servo::CCW_ANGLE_LIMIT_EPROM=0x08; //uint16_t worth of space occupied
//note 1 byte is skipped here per the manual
const uint8_t Servo::HIGH_TEMP_LIMIT_EPROM=0x0B;
const uint8_t Servo::LOW_VOLTAGE_LIMIT_EPROM=0x0C;
const uint8_t Servo::HIGH_VOLTAGE_LIMIT_EPROM=0x0D;
const uint16_t Servo::MAX_TORQUE_EPROM=0x0E; //uint16_t worth of space occupied
const uint8_t Servo::STATUS_RETURN_LEVEL_EPROM=0x10;
const uint8_t Servo::ALARM_LED_EPROM=0x11;
const uint8_t Servo::ALARM_SHUTDOWN_EPROM=0x12;
// note 1 byte is skipped here per FIRMWARE version 36
const uint16_t Servo::MULTI_TURN_OFFSET_EPROM=0x14; //uint16_t worth of space occupied
const uint8_t Servo::RESOLUTION_DIVIDER_EPROM=0x16;
// note 1 byte is skipped here per FIRMWARE version 36
// ----------------------------------------------------------------------
const uint8_t Servo::TORQUE_ENABLE_REG=0x18;
const uint8_t Servo::LED_REG=0x19;
const uint8_t Servo::PID_D_REG=0x1A;
const uint8_t Servo::PID_I_REG=0x1B;
const uint8_t Servo::PID_P_REG=0x1C;
// note 2 byte are skipped here per the manual
const uint16_t Servo::GOAL_POSITION_REG=0x1E; //this is the lowest byte of goal position. The highest byte sits at 0x1F.
const uint16_t Servo::MOVING_SPEED_REG=0x20; //uint16_t worth of space occupied
const uint16_t Servo::TORQUE_LIMIT_REG=0x22; //uint16_t worth of space occupied
const uint16_t Servo::PRESENT_POSITION_REG=0x24; //uint16_t worth of space occupied
const uint16_t Servo::PRESENT_SPEED_REG=0x26; //uint16_t worth of space occupied
const uint16_t Servo::PRESENT_LOAD_REG=0x28; //uint16_t worth of space occupied
const uint8_t Servo::PRESENT_VOLTAGE_REG=0x2A;
const uint8_t Servo::PRESENT_TEMP_REG=0x2B;
const uint8_t Servo::INSTRUCTION_REGISTERED_REG=0x2C;
//note 1 byte is skipped here per the manual
const uint8_t Servo::IS_MOVING_REG=0x2E;
const uint8_t Servo::EEPROM_LOCK_REG=0x2F; //don't touch this, it's just here for consistency with documentation
const uint16_t Servo::PUNCH_REG=0x30; //uint16_t worth of space occupied
// note 19 byte are skipped here per the manual
const uint16_t Servo::CONSUMING_CURRENT_REG=0x44; //uint16_t worth of space occupied
const uint8_t Servo::TORQUE_CONTROL_MODE_REG=0x46;
const uint16_t Servo::GOAL_TORQUE_REG=0x47; //uint16_t worth of space occupied
const uint8_t Servo::GOAL_ACCELERATION_REG=0x49;

//----------- General Dynamixel Constants -----------------
const uint8_t Servo::ON=0x01;
const uint8_t Servo::OFF=0x00;

#endif /*DYNAMIXEL_SERVO_DEFINITIONS_H_*/
