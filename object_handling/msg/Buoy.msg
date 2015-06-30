# Buoys found with the lidar are found using RANSAC with a circle model

# Center of the best fit circle
geometry_msgs/Point position

# Radius of best fit circle
float32 radius

# Color of buoy as seen by a camera
int8 color

# Constants for a bunch of different colors
int8 UNKNOWN=0
int8 RED=1
int8 BLUE=2
int8 YELLOW=3
int8 BLACK=4
int8 GREEN=5
int8 WHITE=6
int8 ORANGE=7
int8 BROWN=8
int8 PINK=9
int8 GREY=10
int8 RAINBOW=11