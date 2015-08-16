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
        self.controller = 'xbox_rc'
        self.controllers = [self.controller]

        # Services
        self.request_controller_srv = rospy.Service('request_controller', request_controller,
                self.request_controller_cb)
        self.register_controller_srv = rospy.Service('register_controller', register_controller,
                self.register_controller_cb)

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
        if self.controller != msg.controller:
            return

        self.continuous_angle_pub.publish(msg.config)

    def full_cb(self, msg):
        if self.controller != msg.controller:
            return

        self.full_pub.publish(msg.config)

    def joint_cb(self, msg):
        if self.controller != msg.controller:
            return

        self.joint_pub.publish(msg.config)

    def wheel_cb(self, msg):
        if self.controller != msg.controller:
            return

        self.wheel_pub.publish(msg.config)

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

    # Service callbacks
    def register_controller_cb(self, request):
        response = register_controllerResponse()
        if request.controller not in self.controllers:
            response.success = True
            self.controllers.append(request.controller)
        else:
            response.success = False
            response.failure_description = '%s is already a registered controller' % request.controller

        return response

    def request_controller_cb(self, request):
        response = request_controllerResponse()
        if request.controller in  self.controllers:
            response.success = True
            response.current_controller = request.controller
            self.controller = request.controller
        else:
            response.success = False
            response.current_controller = self.controller
            response.failure_description = request.controller + ' is not a registered controller. Registered controllers are ' + str(self.controllers)

        return response


if __name__ == '__main__':
    node = control_arbiter()
    rospy.spin()