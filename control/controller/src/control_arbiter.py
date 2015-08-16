#!/usr/bin/env python

import rospy
import roslib
from controller.msg import ControlDynamixelContinuousAngleConfig
from dynamixel_servo.msg import DynamixelContinuousAngleConfig
from controller.msg import ControlDynamixelFullConfig
from dynamixel_servo.msg import DynamixelFullConfig
from controller.msg import ControlDynamixelJointConfig
from dynamixel_servo.msg import DynamixelJointConfig
from controller.msg import ControlDynamixelWheelConfig
from dynamixel_servo.msg import DynamixelWheelConfig
from controller.msg import ControlThrustConfig
from std_msgs.msg import Float64
from controller.srv import register_controller, register_controllerResponse
from controller.srv import request_controller, request_controllerResponse


from std_msgs.msg import Bool, Int64

def callback(msg):

	admin = rospy.Publisher("control_arbiter", Bool, queue_size =1)
	if msg.data == True:
		admin.publish(False)

	if msg.data == False:
		admin.publish(True)

rospy.init_node("control_arbiter")
rospy.Subscriber("rc/status", Bool, callback)
rospy.spin()	




