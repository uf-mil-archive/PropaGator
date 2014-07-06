# This file contains the parameters for the ability to specify an (angle in radians) that you would like the Dynamixel to achieve.
#           The angle is not limited by a set degree amount or rotations. The Dynamixel Server will work to try and keep track
#           of the number of rotations to achieve the angle. Therefore it is CRITICAL that the Dynamixel Server be able to update
#           and talk with each servo relatively fast (so no rotations are missed).
# Note: This message type will utilize/require that the servo(s) be in Wheel mode (which has no safety limits on Clockwise and Counterclockwise rotation).
# The Baud rate is an EPROM adress value on each servo and must match value on the ROS parameter server.
#       Note: all servos on the bus need to have the same baud rate.

#The id specified here is to indicate which servo to talk to, it WILL NOT set the physical id
#       of any servo- as this is an EPROM value.
uint8 id

#       Note: the goal position doesn't correspond directly to the registers on the servo
#       "goal_position" will be in radians (per rep 103), hence it is a float32.
float32 goal_position