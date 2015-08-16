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

PORT = ControlThrustConfig.PORT
STARBOARD = ControlThrustConfig.STARBOARD

class control_arbiter:
    def __init__(self):
        # Initilize ros
        rospy.init_node('control_arbiter')

        # Vars
        self.controller = 'azi_drive'

        # Services


        # Publishers
        self.continuous_angle_pub = rospy.Publisher('/dynamixel/dynamixel_continuous_angle_config', 
            DynamixelContinuousAngleConfig, queue_size = 10)
        self.full_pub = rospy.Publisher('/dynamixel/dynamixel_full_config', 
            DynamixelFullConfig, queue_size = 10)
        self.joint_pub = rospy.Publisher('/dynamixel/dynamixel_joint_config', 
            DynamixelJointConfig, queue_size = 10)
        self.wheel_pub = rospy.Publisher('/dynamixel/dynamixel_wheel_config', 
            DynamixelWheelConfig, queue_size = 10)
        self.port_pub = rospy.Publisher('/stm32f3discovery_imu_driver/pwm1', 
            Float64, queue_size = 10)
        self.starboard_pub = rospy.Publisher('/stm32f3discovery_imu_driver/pwm2', 
            Float64, queue_size = 10)

        # Subscribers
        self.continuous_angle_sub = rospy.Subscriber('dynamixel_continuous_angle_config', 
            ControlDynamixelContinuousAngleConfig, self.continuous_angle_cb, queue_size = 10)
        self.full_sub = rospy.Subscriber('dynamixel_full_config', 
            ControlDynamixelFullConfig, self.full_cb, queue_size = 10)
        self.joint_sub = rospy.Subscriber('dynamixel_joint_config', 
            ControlDynamixelJointConfig, self.joint_cb, queue_size = 10)
        self.wheel_sub = rospy.Subscriber('dynamixel_wheel_config', 
            ControlDynamixelWheelConfig, self.wheel_cb, queue_size = 10)
        self.thruster_sub = rospy.Subscriber('thruster_config', 
            ControlThrustConfig, self.thruster_cb, queue_size = 10)

    # Servo callbacks
    def continuous_angle_cb(self, msg):
        pass

    def full_cb(self, msg):
        pass

    def joint_cb(self, msg):
        pass

    def wheel_cb(self, msg):
        pass

    # Thrust callback
    def thruster_cb(self, msg):
        if self.controller != msg.controller:
            return

        if msg.id == PORT:
            self.port_pub.publish(Float64(msg.pulse_width))
        elif msg.id == STARBOARD:
            self.starboard_pub.publish(Float64(msg.pulse_width))
        else:
            rospy.logerr(str(msg.id) + ' is not a vaild thruster id, valid ids are 2 (starboard) or 3 (port)')



if __name__ == '__main__':
    node = control_arbiter()
    rospy.spin()