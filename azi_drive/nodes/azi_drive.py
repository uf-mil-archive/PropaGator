#!/usr/bin/python
from __future__ import division
import roslib; roslib.load_manifest('azi_drive')
from azimuth_drive import Azi_Drive

import numpy as np
import rospy

from geometry_msgs.msg import WrenchStamped
from motor_control.msg import thrusterNewtons
from dynamixel_servo.msg import DynamixelFullConfig
from time import time

'''Debug ideas:
- Manually test jacobian
- Manually test that 's' produces the right value
- Manually test that this is actually linearized
- I suspect that one of the derivative functions is pointing the wrong direction or doing something nonlinear

Max thrust: 100
Min thrust: -100
Max angle: pi
min angle: -pi
'''

class Controller(object):
    def __init__(self, rate=20):

        self.rate = rate
        self.servo_max_rotation = 1
        self.controller_max_rotation = self.servo_max_rotation / self.rate

        Azi_Drive.set_max_delta_alpha(self.controller_max_rotation)

        rospy.init_node('azi_drive')
        
        self.thrust_pub = rospy.Publisher('thruster_config', thrusterNewtons, queue_size=5)
        self.servo_pub = rospy.Publisher('dynamixel/dynamixel_full_config', DynamixelFullConfig, queue_size=5)

        rospy.Subscriber('wrench', WrenchStamped, self._wrench_cb, queue_size=1)


        # Constants
        self.left_id = 3
        self.right_id = 2
        # self.control_timeout = 2
        self.control_timeout = np.inf

        # Left, right
        self.cur_theta_L = 0.1
        self.cur_theta_R = 0.1
        self.cur_angles = np.array([self.cur_theta_L, self.cur_theta_R])
        self.cur_force_L = 0.0
        self.cur_force_R = 0.0
        self.cur_forces = np.array([self.cur_force_L, self.cur_force_R])

        self.set_servo_angles(self.cur_angles)

        # initializations
        self.last_msg_time = time()
        self.des_fx, self.des_fy, self.des_T = 0.0, 0.0, 0.0

    def main_loop(self):
        rate = rospy.Rate(self.rate)
        while not rospy.is_shutdown():
            cur_time = time()

            rospy.loginfo("Targeting Fx: {} Fy: {} Torque: {}".format(self.des_fx, self.des_fy, self.des_T))
            if (cur_time - self.last_msg_time) > self.control_timeout:
                rospy.logwarn("AZI DRIVE: No control input in over {} seconds! Turning off motors".format(self.control_timeout))
                self.reset_all()
                continue

            p = Azi_Drive.map_thruster(
                fx_des=self.des_fx, 
                fy_des=self.des_fy, 
                m_des=self.des_T, 
                alpha_0= np.matrix(self.cur_angles).T,
                u_0=np.matrix(self.cur_forces).T,
            )
            if p.success == False:
                rospy.logwarn("AZI DRIVE: Did not succeed in finding control solution! Turning off motors")
                self.reset_all()
                continue

            delta_angle_1, delta_angle_2, delta_u_1, delta_u_2 = p.x
            d_theta = np.array([delta_angle_1, delta_angle_2])
            d_force = np.array([delta_u_1, delta_u_2])
            self.cur_angles += d_theta
            self.cur_forces += d_force
            self.set_servo_angles(self.cur_angles)
            self.set_forces(self.cur_forces)

            rospy.loginfo("Achieving net: {}".format(Azi_Drive.net_force(self.cur_angles, self.cur_forces).T))

            rate.sleep()

    def reset_all(self):
        rospy.logwarn("Resetting Azi Drive orientations and thrusts")
        self.set_servo_angles((0, 0))
        self.set_forces((0, 0))

    def _wrench_cb(self, msg):
        force = msg.wrench.force
        torque = msg.wrench.torque

        self.des_fx, self.des_fy = force.x, force.y
        self.des_T = torque.z

    def set_forces(self, (force_l, force_r)):
        rospy.loginfo("Assigning forces [{}, {}]".format(force_l, force_r))
        self.thrust_pub.publish(
            thrusterNewtons(
                id=self.left_id,
                thrust=force_l,
            )
        )
        self.thrust_pub.publish(
            thrusterNewtons(
                id=self.right_id,
                thrust=force_r,
            )
        )

    def set_servo_angles(self, (theta_left, theta_right)):
        rospy.loginfo("Assigning angles [{}, {}]".format(theta_left, theta_right))
        self.servo_pub.publish(DynamixelFullConfig(
            id=                    self.left_id,
            goal_position=         theta_left,
            moving_speed=          self.servo_max_rotation, # near maximum, not actually achievable ...
            torque_limit=          1023,
            goal_acceleration=     38,
            control_mode=          DynamixelFullConfig.JOINT,
            goal_velocity=         self.servo_max_rotation,
        ))
        self.servo_pub.publish(DynamixelFullConfig(
            id=                    self.right_id,
            goal_position=         theta_right,
            moving_speed=          self.servo_max_rotation, # near maximum, not actually achievable ...
            torque_limit=          1023,
            goal_acceleration=     38,
            control_mode=          DynamixelFullConfig.JOINT,
            goal_velocity=         self.servo_max_rotation,
        ))

if __name__ == '__main__':
    controller = Controller()
    controller.main_loop()