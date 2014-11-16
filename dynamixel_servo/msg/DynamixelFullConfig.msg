# This message should not be used if a user wants to just simply control a Dynamixel servo, please see either the joint, wheel, or multiturn message.
# This file contains all the detailed parameters that can used in a ROS to configure the registers on a
#		Dynamixel servo. Note: not every field is in accordance with ros rep 103. This was done on purpose.
#       Please refer to the MX-64T Manual to properly configure the parameters not in accordance with rep 103.
#
#The Baud rate is an EPROM address value on each servo and must match value on the ROS parameter server.
#		Note: all servos on the bus need to have the same baud rate.

#The id specified here is to indicate which servo to talk to, it will not set the physical id
#		of any servo- as this is an EPROM value.
uint8 id

#		0X19	LED								LED On/Off
#		This value will on have the discrete value of off(0) and on(1), but occupies 1 byte in the dynamixel- hence it is a uint8.
uint8 led

#		0X1E	Goal Position(L)				Lowest byte of Goal Position
#		0X1F	Goal Position(H)				Highest byte of Goal Position
#		"goal_position" will be in radians (per rep 103), hence it is a float32 and not a uint16.
#           Depending on the mode of operation (and the manually configured resolution divider)
float32 goal_position


#		0X20	Moving Speed(L)					Lowest byte of Moving Speed
#		0X21	Moving Speed(H)					Highest byte of Moving Speed
#		"moving_speed" will be in radians per second (per rep 103).
#           Depending on the control mode, the allowed range is 0 to 12.25 rad/s or -12.25 to 12.25 rad/s
float32 moving_speed

#		0X22	Torque Limit(L)					Lowest byte of Torque Limit
#		0X23	Torque Limit(H)					Highest byte of Torque Limit
#       This is the percentage of the maximum torque limit to use.
#       The range 0 to 1023 (0x3FF) is available; each increment is about 0.1% the "max torque"
uint16 torque_limit

#		0X49	Goal Acceleration				Goal Acceleration
#		"goal_acceleration" will be in radians per second per second- e.g. rad/(s^2). (per rep 103)
#       the range is 0 to 38.04 rad/(s^2)
float32 goal_acceleration

#       The folowing isn't a register on the Dynamixel, but is used to simplify the mode of operation that the Dynimexel operates in.
uint8 control_mode

# The Folowing are enumerations to help set the operational mode of a dynamixel servo (e.g. joint, wheel, multi-turn)
# 0 i explicitly not an enumeration in the control method to protect against uninitalized messages.
uint8 JOINT=1
#NOTE: The following methods DO NOT have Clockwise and Counterclockwise Safety limits, and explicitly clear any that were set!
uint8 WHEEL=2
uint8 MULTI_TURN=3
# The folowing mode (CONTINUOUS_ANGLE) is similar to "MULTI_TURN" mode, but works to take out the turn limitation in software.
uint8 CONTINUOUS_ANGLE=4

float32 goal_velocity # only used in continuous angle mode
