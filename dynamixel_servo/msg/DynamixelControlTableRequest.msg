#This file contains all the paramaters that will be used by a ROS node to request the entire control table (eeprom and ram) of a Dynamixel servo.
#
#The Baud rate is an EPROM adress value on each servo and must match value on the ROS parameter server.
#		Note: all servos on the bus need to have the same baud rate.

#The id specified here is to indicate which servo to get the control table from
uint8 id 