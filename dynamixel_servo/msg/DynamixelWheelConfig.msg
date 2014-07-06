# Note: This message type is specifically for when the Dynamixel Servo(s) are in "WHEEL" mode
#The Baud rate is an EPROM adress value on each servo and must match value on the ROS parameter server.
#       Note: all servos on the bus need to have the same baud rate.

#The id specified here is to indicate which servo to talk to, it WILL NOT set the physical id
#       of any servo- as this is an EPROM value.
uint8 id

# This is the moving speed in a goal direction. This will essential configure the memory locations bellow.
# Note: the 10th bit indicates the directions
#       0X20    Moving Speed(L)                 Lowest byte of Moving Speed
#       0X21    Moving Speed(H)                 Highest byte of Moving Speed
#       "moving_speed" will be in radians per second (per rep 103). also an angular velocity in the ccw direction is considered positive.
float32 moving_speed
