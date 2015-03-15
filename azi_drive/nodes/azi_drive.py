#!/usr/bin/python
from __future__ import division
import roslib; roslib.load_manifest('azi_drive')
import azimuth_drive
# import pdb; pdb.set_trace()
print dir(azi_drive)
azi_drive.azi_drive
# azi_drive.Azi_Drive
azi_drive.azimuth_drive.Azi_Drive
from azi_drive import Azi_Drive

import numpy as np
from geometry_msgs.msg import WrenchStamped
from motor_control.msg import thrusterNewtons
from dynamixel_servo.msg import DynamixelFullConfig
from time import time


class Controller(object):
    def __init__(self, rate):

        self.rate = 20
        self.servo_max_rotation = 1
        self.controller_max_rotation = servo_max_rotation / self.rate

        Azi_Drive.set_max_delta_alpha(self.controller_max_rotation)

        rospy.init_node('azi_drive')
        
        self.thrust_pub = rospy.Publisher('thruster_config', thrusterNewtons, queue_size=1)
        self.servo_pub = rospy.Publisher('dynamixel/dynamixel_full_config', DynamixelFullConfig, queue_size=1)

        rospy.Subscriber('wrench', WrenchStamped, self._wrench_cb, queue_size=1)


        self.left_id = 3
        self.right_id = 2

        # Left, right
        self.cur_theta_L = np.pi
        self.cur_theta_R = np.pi
        self.cur_angles = np.array([self.cur_theta_L, self.cur_theta_R])
        self.cur_force_L = 10
        self.cur_force_R = 10
        self.cur_forces = np.array([self.cur_force_L, self.cur_force_R])

        self.set_servo_angles(self.cur_angles)

    def main_loop(self):
        rate = rospy.Rate(self.rate)
        while not rospy.is_shutdown():
            p = Azi_Drive.map_thruster(
                fx_des=self.des_fx, 
                fy_des=self.des_fy, 
                m_des=self.des_T, 
                alpha_0= alpha_nought,
                u_0=u_nought,
            )
            if p.Success == False:
                print "AZI DRIVE: Did not succeed in finding control solution! Turning off motors"
                self.set_servo_angles((0, 0))
                self.set_forces((0, 0))

            delta_theta_1, delta_theta_2, delta_u_1, delta_u_2 = p.x
            d_theta = np.array([delta_angle_1, delta_angle_2])
            d_force = np.array([delta_u_1, delta_u_2])
            self.cur_angles += d_theta
            self.cur_forces += d_force
            self.set_servo_angles(self.cur_angles)
            self.set_forces(self.cur_forces)


            rate.sleep()

    def _wrench_cb(self, msg):
        force = msg.wrench.force
        torque = msg.wrench.torque

        self.des_fx, self.des_fy = force.x, force.y
        self.des_T = torque.z

    def set_forces(self, (force_l, force_r)):
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