#This file contains all the paramaters that will be used by ROS to configure paramaters on the
#		Dynamixel servo.
#
#EPROM Adresses will NOT be writen to by the ROS node, to configure these adresses you must
#		connect and configure each Dynamixel servo individualy. Be sure to remeber what
#		changes are made to each individual servo. Note: EPROM adresses survive a power cycle.
#		The EPROM adresses are as folows:
#		0x00	Model Number(L)					Lowest byte of model number
#		0X01	Model Number(H)					Highest byte of model number
#		0X02	Version of Firmware				Information on the version of firmware
#		0X03	ID								ID of Dynamixel
#		0X04	Baud Rate						Baud Rate of Dynamixel
#		0X05	Return Delay Time				Return Delay Time
#		0X06	CW Angle Limit(L)				Lowest byte of clockwise Angle Limit
#		0X07	CW Angle Limit(H)				Highest byte of clockwise Angle Limit
#		0X08	CCW Angle Limit(L)				Lowest byte of counterclockwise Angle Limit
#		0X09	CCW Angle Limit(H)				Highest byte of counterclockwise Angle Limit
#		0X0B	the Highest Limit Temperature	Internal Limit Temperature
#		0X0C	the Lowest Limit Voltage		Lowest Limit Voltage
#		0X0D	the Highest Limit Voltage		Highest Limit Voltage
#		0X0E	Max Torque(L)					Lowest byte of Max. Torque
#		0X0F	Max Torque(H)					Highest byte of Max. Torque
#		0X10	Status Return Level				Status Return Level
#		0X11	Alarm LED						LED for Alarm
#		0X12	Alarm Shutdown					Shutdown for Alarm
#The Baud rate is an EPROM adress value and must be pulled from the ROS config server.
#		Note: all servos need to have the same baud rate.

#The id specified here is to indicate which servo to talk to, it will not set the physical id
#		of any servo- as this is an EPROM value.
uint8 id

#		0X19	LED								LED On/Off
uint8 led

#		0X1E	Goal Position(L)				Lowest byte of Goal Position
#		0X1F	Goal Position(H)				Highest byte of Goal Position
uint16 goal_position

#		0X20	Moving Speed(L)					Lowest byte of Moving Speed
#		0X21	Moving Speed(H)					Highest byte of Moving Speed
uint16 moving_speed

#		0X47	Goal Torque(L)					Lowest byte of goal torque value
#		0X48	Goal Torque(H)					Highest byte of goal torque value
uint16 goal_torque

#		0X49	Goal Acceleration				Goal Acceleration
uint8 goal_acceleration