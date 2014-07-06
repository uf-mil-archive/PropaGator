# This file contains only the most basic paramater that most nodes will be concerned with setting, the goal_position of the servo.
# Note: This message type is specifically for when the Dynamixel Servo(s) are in "JOINT" mode
#The Baud rate is an EPROM adress value on each servo and must match value on the ROS parameter server.
#		Note: all servos on the bus need to have the same baud rate.

#The id specified here is to indicate which servo to talk to, it WILL NOT set the physical id
#		of any servo- as this is an EPROM value.
uint8 id

#		0X1E	Goal Position(L)				Lowest byte of Goal Position
#		0X1F	Goal Position(H)				Highest byte of Goal Position
#		"goal_position" will be in radians (per rep 103), hence it is a float32.
float32 goal_position