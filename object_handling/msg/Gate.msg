# Center of the gate
geometry_msgs/Point position

# Composing buoys
object_handling/Buoy buoy1
object_handling/Buoy buoy2

#
# Geometry varibles pre-calculated for the ease of the user
#
# Smallest angle betwen the x unit vector and the line drawn between the two composing buoys
#	in the frame specified in the encompising msg
float32 yaw
# Gate width, distance between buoys centers (the radius of the buoys is not accounted for)
float32 width