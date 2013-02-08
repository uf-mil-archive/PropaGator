#!/usr/bin/env python

import roslib
roslib.load_manifest('xbox_controller')
import rospy
from geometry_msgs.msg import WrenchStamped, Vector3, Point, Wrench
from sensor_msgs.msg import Joy
from std_msgs.msg import Header
#from pygame import *
import time,math

X360_AXIS_IDS = {
'LEFT_X': 0,
'LEFT_Y': 1,
'LEFT_TRIGGER': 2,
'RIGHT_X': 3,
'RIGHT_Y': 4,
'RIGHT_TRIGGER': 5,
'D_PAD_X': 6,
'D_PAD_Y': 7,
}
X360_BUTTON_IDS = {
'A': 0,
'B': 1,
'X': 2,
'Y': 3,
'L_BUMPER': 4,
'R_BUMPER': 5,
'BACK': 6,
'START': 7,
'GUIDE': 8,
'L_STICK': 9,
'R_STICK': 10,
}
max_torque = 2		#remember to change value in motor driver package also

rospy.init_node('xbox_controller')
controller_wrench = rospy.Publisher('wrench', WrenchStamped)


def joystick_callback(msg):
	
	controller_wrench.publish(WrenchStamped(
						header = Header(
							stamp=rospy.Time.now(),
							frame_id="/base_link",
							),
						wrench=Wrench(
							force = Vector3(x=msg.axes[1]*max_torque,y= -msg.axes[0]*max_torque,z= 0),
							torque = Vector3(x=0,y= 0,z= msg.axes[3]),
							))
							)	
	rospy.sleep(.2)
		

rospy.Subscriber('joy', Joy, joystick_callback,queue_size=1)
rospy.spin()


