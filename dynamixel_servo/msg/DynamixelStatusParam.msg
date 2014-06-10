#This file contains all the status parameters that will be returned by default for every servo
#		that the dynamixel server knows about; it will poll every servo at the poll_rate specified on the parameter server.
#		It should be noted that the values that are being returned are in 16 contiguous bytes;
#		this is done specifically to allow for a single read of all the data at once.
#
#The Baud rate is an EPROM adress value on each servo and must match value on the ROS parameter server.
#		Note: all servos on the bus need to have the same baud rate.

#The id specified here is to indicate which servo to talk to, it will not set the physical id
#		of any servo- as this is an EPROM value.
uint8 id

#		30(0X1E)	Goal Position(L)		Lowest byte of Goal Position
#		31(0X1F)	Goal Position(H)		Highest byte of Goal Position
#		"goal_position" will be in radians (per rep 103), hence it is a float32.
float32 goal_position

#		32(0X20)	Moving Speed(L)			Lowest byte of Moving Speed
#		33(0X21)	Moving Speed(H)			Highest byte of Moving Speed
uint16 moving_speed

#		34(0X22)	Torque Limit(L)			Lowest byte of Torque Limit
#		35(0X23)	Torque Limit(H)			Highest byte of Torque Limit
uint16 torque_limit

#		36(0X24)	Present Position(L)		Lowest byte of Current Position
#		37(0X25)	Present Position(H)		Highest byte of Current Position
#		"present_position" will be in radians (per rep 103), hence it is a float32.
float32 present_position

#		38(0X26)	Present Speed(L)		Lowest byte of Present Speed
#		39(0X27)	Present Speed(H)		Highest byte of Present Speed
uint16 present_speed

#		40(0X28)	Present Load(L)			Lowest byte of Current Load
#		41(0X29)	Present Load(H)			Highest byte of Current Load
uint16 present_load

#		42(0X2A)	Present Voltage			Current Voltage
#		"present_position" will be in volts (per rep 103), hence it is a float32.
float32 present_voltage

#		43(0X2B)	Present Temperature		Current Temperature
#		"present_temp" will be in degrees Celsius (per rep 103). Note the value in the servo's reg is identical to the actual temp in Celsius.
uint8 present_temp

#		44(0X2C)	Registered				Means if Instruction is registered (used to synchronously move multiple servos at once)
uint8 registered

#		46(0X2E)	Moving					Means if there is any movement (0=no movement, 1=movement)
uint8 is_moving