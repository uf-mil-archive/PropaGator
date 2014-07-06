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

/*
 * Notes:
 * 		All bits and locations referenced are 0 indexed- in concurrence with the dynamixel manual.
 */

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
	uint8_t resolution_divider; //When resolution is lowered, revolutions (in both directions) can be increased (up to 28 turns in each direction when in multi turn mode). Range (1~4)
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

	// the following are variables to help with continuous angle mode, and the ability to change modes while still keeping their safety limits
	float previous_continuious_position_in_radians;
	float current_continuious_position_in_radians;
	float continuious_angle_goal;
	bool continuious_angle_mode;
	dynamixel_uint16_t previous_cw_angle_limit;
	dynamixel_uint16_t previous_ccw_angle_limit;

public:
	// While the following may look messy, it is technical faster to initialize variables in initializer lists
	//		Also note the presence of default arguments in the class definition- allowing for more concise code.
	//				The order in which the arguments have been written for this constructor were specifically chosen on purpose. For more on default values see: http://goo.gl/MK0RZ2
	Servo(uint8_t input_id=0x00, string input_system_membership="", string input_description="", uint8_t input_baud=0x22) :
		system_membership(input_system_membership),
		description(input_description),
		previous_continuious_position_in_radians(0.0),
		current_continuious_position_in_radians(0.0),
		continuious_angle_goal(0.0),
		previous_cw_angle_limit(0x0000),
		previous_ccw_angle_limit(0x0FFF),
		continuious_angle_mode(false),
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
	void setPresentPosition(dynamixel_uint16_t position);
	float getPresentPositionInRadians();
	void setGoalPosition(dynamixel_uint16_t position);
	float getGoalPositionInRadians();
	uint8_t getID(){return id;};
	void setSystemMembership(string input){system_membership=input;};
	string getSystemMembership(){return system_membership;};
	void setDescription(string input){description=input;};
	string getDescription(){return description;};

	uint16_t getMinAngle(){return cw_angle_limit;};
	uint16_t getMaxAngle(){return ccw_angle_limit;};
	// When the power is first turned on, the Dynamixel "Torque Limit" reg (ram address 0x22 and 0x23) is initially set to the same value in "Max Torque" (eprom address 0x0E and 0x0F)- which may not necessarily be 0x3FF.
	//		With that being said, the functions "getMaxTorqueDefaultValue" and "getMaxTorqueAllowed" were created to avoid naming confusion.
	// 	Note: The naming convention in the Dynamixel manuals is not the best.
	uint16_t getMaxTorqueDefaultValue(){return max_torque;};
	uint16_t getMaxTorqueAllowed(){return 0x03FF;};
	bool inMultiTurnMode(){return (cw_angle_limit==4095&&ccw_angle_limit==4095)?true:false;};
	bool inWheelMode(){return (cw_angle_limit==0&&ccw_angle_limit==0)?true:false;};
	bool inJointMode(){return (inWheelMode()==false&&inMultiTurnMode()==false)?true:false;};
	// When in wheel mode the 10th bit indicates the direction, if it's set (i.e. it's a 1 ) the direction of rotation is clockwise. Note: the index of the bits starts at 0.
	bool rotatingClockWise(){return (inWheelMode() && (moving_speed&0b10000000000))?true:false;};
	bool rotatingCounterClockWise(){return (inWheelMode() && !(moving_speed&0b10000000000))?true:false;};
	float getMovingSpeedRadPerSec();
	float getPresentSpeedRadPerSec();
	uint16_t getMinMovingSpeed()
	{// returns the min moving speed RELATIVE to the direction the servo is spinning (in rpm)- conversion to ros's rep 103 with rad/s should take place higher up!
		if(inWheelMode()&&rotatingCounterClockWise())
		{
			return 0x0000;
		}
		else if(inWheelMode() && rotatingCounterClockWise())
		{
			return 0x0400;
		}
		return 0x0000;
	};
	uint16_t getMaxMovingSpeed()
	{// returns the max moving speed RELATIVE to the direction the servo is spinning (in rpm)
		if(inWheelMode()&&rotatingCounterClockWise())
		{
			return 0x03FF;
		}
		else if(inWheelMode() && rotatingCounterClockWise())
		{
			return 0x07FF;
		}
		return 0x03FF;
	};
	float getMaxMovingSpeedInRadSec()
	{
		// for ros rep 103 ccw is + and cw is negative.
		// If the direction of rotation isn't fully definable (like in Joint or multi-turn mode) a positive number will be returned.
		if(inWheelMode()&&rotatingCounterClockWise())
		{
			return MOVING_SPEED_RESOLUTION*MOVING_SPEED_PER_UNIT_IN_RPM*RPM_TO_RAD_PER_SECOND;
		}
		else if (inWheelMode()&&rotatingClockWise())
		{
			return -(MOVING_SPEED_RESOLUTION*MOVING_SPEED_PER_UNIT_IN_RPM*RPM_TO_RAD_PER_SECOND);
		}
		// joint mode and multiturn mode
		return MOVING_SPEED_RESOLUTION*MOVING_SPEED_PER_UNIT_IN_RPM*RPM_TO_RAD_PER_SECOND;
	}
	uint16_t getZeroMovingSpeed()
	{
		//If a value in the range of 1024~2047 is used, it is stopped by setting to 1024 while rotating to CW direction.
		if(inWheelMode()&&rotatingCounterClockWise())
		{
			return WHEEL_MODE_MOVING_SPEED_ZERO_CW;//1024
		}
		//If a value in the range of 0~1023 is used, it is stopped by setting to 0 while rotating to CCW direction.
		return 0x0000;
	}


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
	static const uint16_t ENCODER_RESOLUTION;
	static const uint16_t MAX_GOAL_POSITION;
	//static const uint16_t MAX_GOAL_POSITION_AX12A;
	static const uint16_t MAX_JOINT_MODE_MOVING_SPEED;
	static const uint16_t MAX_MULTTURN_MODE_MOVING_SPEED;
	static const uint16_t MAX_WHEEL_MODE_MOVING_SPEED;
	static const uint16_t WHEEL_MODE_MOVING_SPEED_ZERO_CW;
	static const uint16_t WHEEL_MODE_MOVING_SPEED_ZERO_CCW;
	static const uint16_t MOVING_SPEED_RESOLUTION;
	static const float MOVING_SPEED_PER_UNIT_IN_RPM;
	static const uint8_t MAX_ACCELERATION;
	static const uint8_t MIN_ACCELERATION;
	static const float ACCELERATION_PER_UNIT_IN_DEGREES;
	static const float MAX_MOVING_SPEED_IN_RAD_SEC;

	//----------- General Constants -----------------
	static const float RPM_TO_RAD_PER_SECOND;
	static const float RAD_PER_SECOND_TO_RPM;
};

//const float Servo::ACCELERATION_PER_UNIT_IN_DEGREES=8.5826772;

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
const uint16_t Servo::ENCODER_RESOLUTION=0xFFF;
const uint16_t Servo::MAX_GOAL_POSITION=0xFFF; // the maximum allowed value 0xFFF (4095) to be set for the goal position
//const uint16_t Servo::MAX_GOAL_POSITION_AX12A=0x3FF;// Note: An AX-12A can have a MAX_GOAL_POSITION of 0x3ff
const uint16_t Servo::MAX_JOINT_MODE_MOVING_SPEED=0x3FF;
const uint16_t Servo::MAX_MULTTURN_MODE_MOVING_SPEED=0x3FF;
const uint16_t Servo::MAX_WHEEL_MODE_MOVING_SPEED=0x7FF;
const uint16_t Servo::MOVING_SPEED_RESOLUTION=0x3FF;
const uint16_t Servo::WHEEL_MODE_MOVING_SPEED_ZERO_CW=0x400;
const uint16_t Servo::WHEEL_MODE_MOVING_SPEED_ZERO_CCW=0x000;
const float Servo::MOVING_SPEED_PER_UNIT_IN_RPM=.11443;
const uint8_t Servo::MAX_ACCELERATION=0xFE;
const uint8_t Servo::MIN_ACCELERATION=0x01;// since 0x00 is the same as max acceleration the min acceleration is 0x01
const float Servo::ACCELERATION_PER_UNIT_IN_DEGREES=8.5826772;
const float Servo::MAX_MOVING_SPEED_IN_RAD_SEC=MAX_MULTTURN_MODE_MOVING_SPEED*MOVING_SPEED_PER_UNIT_IN_RPM*RPM_TO_RAD_PER_SECOND;


//----------- General Constants -----------------
const float Servo::RPM_TO_RAD_PER_SECOND=0.104719755;
const float Servo::RAD_PER_SECOND_TO_RPM=9.54929659643;

void Servo::setPresentPosition(dynamixel_uint16_t input_position)
{
	//an approximation of how the position amount increaseses for a given direction is below.
	//				2048
	//
	//		2412				1561
	//
	//3122						1072
	//
	//		3612				636
	//
	//				4095,0
	//
	//
	//			MX-64T Logo
	//
	static const float PI=3.14159265359;
	float offset=0.0;
	if(continuious_angle_mode==true)
	{
		if(rotatingClockWise()&&input_position>present_position)
		{
			// going clockwise it will hit the transition from 0 to 4095, This should be the only time input_position is > than present_position
			// rep 103 makes clockwise negative
			offset=-(present_position-0+4095-input_position)*(360.0/Servo::ENCODER_RESOLUTION);
		}
		else if(rotatingClockWise())
		{
			offset=-(present_position-input_position)*(360.0/Servo::ENCODER_RESOLUTION);
		}
		else if(rotatingCounterClockWise()&&input_position<present_position)
		{
			//going counterclockwise it will go from 4095 -> to -> 0
			// rep 103 makes counter clockwise positive
			offset=(4095-present_position+input_position-0)*(360.0/Servo::ENCODER_RESOLUTION);
		}
		else if(rotatingCounterClockWise())
		{
			offset=(input_position-present_position)*(360.0/Servo::ENCODER_RESOLUTION);
		}
	}
	//assing the representation of the register
	present_position=input_position;
	// also extrapolate the radian data
	previous_continuious_position_in_radians=current_continuious_position_in_radians;
	float position_degrees=0.0;
	float position_radians=0.0;
	position_degrees=input_position*(360.0/Servo::ENCODER_RESOLUTION);
	current_continuious_position_in_radians=position_degrees*(PI/180)+offset;
	return;
}
float Servo::getPresentPositionInRadians()
{
	if(continuious_angle_mode==true)
	{
		return current_continuious_position_in_radians;
	}

	static const float PI=3.14159265359;
	float position_degrees=0.0;
	if(inMultiTurnMode())
	{// only in multi_turn mode is this special
		//per the manual Present position = (Real Position / Resolution Divider) + Multi-turn Offset
		position_degrees=(present_position/resolution_divider+multi_turn_offset)*(360.0/Servo::ENCODER_RESOLUTION);
	}
	else
	{
		position_degrees=present_position*(360.0/Servo::ENCODER_RESOLUTION);
	}
	return position_degrees*(PI/180);//return radians
}
float Servo::getGoalPositionInRadians()
{
	if(continuious_angle_mode==true)
	{
		return continuious_angle_goal;
	}
	static const float PI=3.14159265359;
	float position_degrees=0.0;
	float position_radians=0.0;
	position_degrees=goal_position*(360.0/Servo::ENCODER_RESOLUTION);
	return position_degrees*(PI/180);
}
float Servo::getMovingSpeedRadPerSec()
{
	// 1 rad/s is approximately 9.5493 rpm, conversely .1047 rad/s is 1 rpm. see: http://goo.gl/52oeiy
	// The Dynamixel's moving speed has an approximate unit of .11443 rpm, given its resolution of 1023. Thus it can essential move +- 112.25869244063695 rad/s in wheel mode
	// the compiler will inline the folowing statement, but it is here to show the conversion
	if (moving_speed==getZeroMovingSpeed())
	{
		return 0.0;
	}
	else if(rotatingCounterClockWise()||inJointMode()||inMultiTurnMode())
	{
		// according to rep 103 ccw is positive
		return (float)(moving_speed/9.54929659643);
	}
	// its clockwise
	// According to ros rep 103 cw is in the negative direction hence the following conversion
	return (float)(-moving_speed/9.54929659643);
}

float Servo::getPresentSpeedRadPerSec()
{
	// If a value is in the rage of 0~1023 then the motor rotates to the CCW direction.
	// If a value is in the rage of 1024~2047 then the motor rotates to the CW direction.
	// The 10th bit becomes the direction bit to control the direction; 0 and 1024 are equal.
	// The value unit is about 0.11rpm.
	//inWheelMode() && (moving_speed&0b10000000000))?true:false;
	if(present_speed==0b00000000000||present_speed==0b10000000000)
	{
		return 0.0;

	}

	else if (inWheelMode()&&(present_speed&0b10000000000))
	{
		// clockwise rep 103 says its negative
		return -(present_speed-1024)*(.11443);
	}
	return present_speed*(.11443);

}





#endif /*DYNAMIXEL_SERVO_DEFINITIONS_H_*/
