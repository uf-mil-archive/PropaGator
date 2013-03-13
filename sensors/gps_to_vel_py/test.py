
from __future__ import print_function
from math import *
speed = 1
direction = 270

y_vel = speed*sin(radians(direction))
x_vel = speed*cos(radians(direction))

print("x vel = " + str(x_vel) + "\ny vel = " + str(y_vel) + "\n")
