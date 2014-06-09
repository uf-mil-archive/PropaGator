#This file contains all the paramaters that will be used in a ROS message to configure the registers on a
#		Dynamixel servo.
#
#The Baud rate is an EPROM adress value on each servo and must match value on the ROS parameter server.
#		Note: all servos on the bus need to have the same baud rate.

#The id specified here is to indicate which servo to talk to, it will not set the physical id
#		of any servo- as this is an EPROM value.
uint8 id

#		0X19	LED								LED On/Off
#		This value will on have the discrete value of off(0) and on(1), hence it is a uint8
uint8 led

#		0X1E	Goal Position(L)				Lowest byte of Goal Position
#		0X1F	Goal Position(H)				Highest byte of Goal Position
#		"goal_position" will be in radians (per rep 103), hence it is a float32.
float32 goal_position

#		0X20	Moving Speed(L)					Lowest byte of Moving Speed
#		0X21	Moving Speed(H)					Highest byte of Moving Speed
#		"moving_speed" will be in radians per second (per rep 103).
uint16 moving_speed

#		0X22	Torque Limit(L)					Lowest byte of Torque Limit
#		0X23	Torque Limit(H)					Highest byte of Torque Limit
uint16 torque_limit

#		0X49	Goal Acceleration				Goal Acceleration
#		"goal_acceleration" will be in radians per second per second- e.g. rad/(s^2). (per rep 103)
float32 goal_acceleration