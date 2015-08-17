#!/usr/bin/env python

import rospy
import roslib
import threading
from controller.msg import ControlDynamixelContinuousAngleConfig
from dynamixel_servo.msg import DynamixelContinuousAngleConfig
from controller.msg import ControlDynamixelFullConfig
from dynamixel_servo.msg import DynamixelFullConfig
from controller.msg import ControlDynamixelJointConfig
from dynamixel_servo.msg import DynamixelJointConfig
from controller.msg import ControlDynamixelWheelConfig
from dynamixel_servo.msg import DynamixelWheelConfig
from controller.msg import ControlThrustConfig
from std_msgs.msg import Float64, Bool, String
from controller.srv import register_controller, register_controllerResponse
from controller.srv import request_controller, request_controllerResponse
from controller.srv import FloatMode
from controller.srv import get_all_controllers, get_all_controllersResponse
from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster

PORT = ControlThrustConfig.PORT
STARBOARD = ControlThrustConfig.STARBOARD
ZERO_PWM = 1.5e-3

# TODO
# Add service to get all avalable controllers
# Find way that servos and thruster call backs can all fire simltaniously but still
#   are thread locked with request_controller

#Notes
# Switching controllers happens in a different thread than the thrusters getting published
#   It is possible that after a switch occurs and the zero thrusters method has been called
#   the other thread will still call its last cb so thread lock

class control_arbiter:
    def __init__(self):
        # Initilize ros
        rospy.init_node('control_arbiter')

        # Vars
        self.controller = 'none'
        self.controllers = []
        self.floating = False
        self.killed = False
        self.continuous_angle_lock = threading.Lock()
        self.joint_lock = threading.Lock()
        self.full_lock = threading.Lock()
        self.wheel_lock = threading.Lock()
        self.thrust_lock = threading.Lock()

        # Services
        self.request_controller_srv = rospy.Service('request_controller', request_controller,
                self.request_controller_cb)
        self.register_controller_srv = rospy.Service('register_controller', register_controller,
                self.register_controller_cb)
        self.float_srv = rospy.Service('float_mode', FloatMode, 
                self.set_float_mode)
        self.get_all_controllers_srv = rospy.Service('get_all_controllers', get_all_controllers,
                self.get_all_controllers_cb)


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
        self.float_status_pub = rospy.Publisher('float_status', Bool, queue_size=1)
        self.current_controller_pub = rospy.Publisher('current_controller', String, queue_size=1)

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

        # Timers
        self.update_timer = rospy.Timer(rospy.Duration(0.1), self.status_update)

        # Kill
        self.kill_listener = KillListener(self.set_kill_cb, self.clear_kill_cb)
        self.kill_broadcaster = KillBroadcaster(id = 'control_arbiter', description = 'control_arbiter has shutdown')
        try:
            self.kill_broadcaster.clear()
        except rospy.service.ServiceException, e:
            rospy.logwarn(str(e))

        rospy.on_shutdown(self.shutdown)

    # Utility functions
    # Zero thrusters
    def _zero_thrusters(self):
        self.port_pub.publish(Float64(ZERO_PWM))
        self.starboard_pub.publish(Float64(ZERO_PWM))

    def isValidController(self, controller):
        if controller == 'always_on':
            return True

        if controller == self.controller:
            return True

        return False


    # Servo callbacks
    def continuous_angle_cb(self, msg):
        with self.continuous_angle_lock:
            if not self.isValidController(msg.controller):
                return

            self.continuous_angle_pub.publish(msg.config)


    def full_cb(self, msg):
        with self.full_lock:
            if not self.isValidController(msg.controller):
                return

            self.full_pub.publish(msg.config)

    def joint_cb(self, msg):
        with self.joint_lock:
            if not self.isValidController(msg.controller):
                return

            self.joint_pub.publish(msg.config)

    def wheel_cb(self, msg):
        with self.wheel_lock:
            if not self.isValidController(msg.controller):
                return

            self.wheel_pub.publish(msg.config)

    # Thrust callback
    def thruster_cb(self, msg):
        with self.thrust_lock:
            if not self.isValidController(msg.controller):
                return

            if self.floating or self.killed:
                self._zero_thrusters()
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
            
            self.continuous_angle_lock.acquire()
            self.joint_lock.acquire()
            self.full_lock.acquire()
            self.wheel_lock.acquire()
            self.thrust_lock.acquire()

            self.controller = request.controller
            self._zero_thrusters()
            
            self.continuous_angle_lock.release()
            self.joint_lock.release()
            self.full_lock.release()
            self.wheel_lock.release()
            self.thrust_lock.release()
        else:
            response.success = False
            response.current_controller = self.controller
            response.failure_description = request.controller + ' is not a registered controller. Registered controllers are ' + str(self.controllers)

        return response

    def get_all_controllers_cb(self, request):
        response = get_all_controllersResponse()
        response.controllers = self.controllers
        return response

    # Float functions
    # Used to place boat in floating mode (Thrusters are turned off)
    def set_float_mode(self, mode):
        self.floating = mode.float
        if self.floating:
            self._zero_thrusters()

        rospy.loginfo('PropaGator float-mode is ' + str(mode.float))

        return self.floating

    # Status update function
    def status_update(self, event):
        self.float_status_pub.publish(self.floating)
        self.current_controller_pub.publish(self.controller)

    # kill funcitons
    def shutdown(self):
        self.kill_broadcaster.send(True)

    def set_kill_cb(self):
        self.killed = True
        self._zero_thrusters()
        rospy.logwarn('control_arbiter killed because: %s' % self.kill_listener.get_kills())

    def clear_kill_cb(self):
        self.killed = False
        rospy.loginfo('control_arbiter unkilled')


if __name__ == '__main__':
    node = control_arbiter()
    rospy.spin()