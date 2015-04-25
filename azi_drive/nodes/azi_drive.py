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

'''
Max thrust: 100
Min thrust: -100
Max angle: pi
min angle: -pi
'''

class Controller(object):
    def __init__(self, rate=20):
        '''I can't swim on account of my ass-mar'''
        self.rate = rate
        self.servo_max_rotation = 1
        self.controller_max_rotation = self.servo_max_rotation / self.rate

        rospy.init_node('azi_drive')
        rospy.logwarn("Setting maximum rotation speed to {} rad/s".format(self.controller_max_rotation))
        Azi_Drive.set_delta_alpha_max(self.controller_max_rotation)
        
        # These should not be queued! Old commands are garbage.
        # Unfortunately, we have to queue these, because the subscriber cannot process two sequential
        #  thrust messages as quickly as they are sent
        self.thrust_pub = rospy.Publisher('thruster_config', thrusterNewtons, queue_size=4)
        self.servo_pub = rospy.Publisher('dynamixel/dynamixel_full_config', DynamixelFullConfig, queue_size=4)
        rospy.Subscriber('wrench', WrenchStamped, self._wrench_cb, queue_size=1)

        # Thrust topic id's for each thruster
        self.left_id = 3
        self.right_id = 2
        # Time between messages before azi_drive shuts off
        # self.control_timeout = 2  # secs
        self.control_timeout = np.inf

        self.default_angles = np.array([np.pi, np.pi])
        self.default_forces = np.array([0.0, 0.0])

        # Left, Right
        self.cur_angles = self.default_angles[:]
        self.cur_forces = self.default_forces[:]

        self.set_servo_angles(self.cur_angles)
        self.set_forces(self.cur_forces)

        # Initializations
        self.last_msg_time = time()
        self.des_fx, self.des_fy, self.des_torque = 0.0, 10.0, 0.0

    def main_loop(self):
        rate = rospy.Rate(self.rate)
        while not rospy.is_shutdown():
            cur_time = time()

            rospy.loginfo("Targeting Fx: {} Fy: {} Torque: {}".format(self.des_fx, self.des_fy, self.des_torque))
            if (cur_time - self.last_msg_time) > self.control_timeout:
                rospy.logwarn("AZI DRIVE: No control input in over {} seconds! Turning off motors".format(self.control_timeout))
                self.stop()
                continue

            thrust_solution = Azi_Drive.map_thruster(
                fx_des=self.des_fx,
                fy_des=self.des_fy,
                m_des=self.des_torque, 
                alpha_0= self.cur_angles,
                u_0=self.cur_forces,
            )

            d_theta, d_force = thrust_solution
            self.cur_angles += d_theta
            self.cur_forces += d_force
            self.set_servo_angles(self.cur_angles)
            self.set_forces(self.cur_forces)

            rospy.loginfo("Achieving net: {}".format(Azi_Drive.net_force(self.cur_angles, self.cur_forces)))

            rate.sleep()

        # On shutdown, kill thrust
        self.reset_all()
        rospy.sleep(0.2)

    def stop(self):
        self.des_fx, self.des_fy, self.des_torque = 0.0, 0.0, 0.0

    def reset_all(self):
        rospy.logwarn("Resetting Azi Drive orientations and thrusts")
        self.set_servo_angles(self.default_angles)
        self.set_forces(self.default_forces)

    def _wrench_cb(self, msg):
        '''Recieve a force, torque command for the mapper to achieve'''
        force = msg.wrench.force
        torque = msg.wrench.torque

        self.des_fx, self.des_fy = force.x, force.y
        self.des_torque = torque.z

    def set_forces(self, (force_left, force_right)):
        '''Issue commands to the thrusters'''

        # Avoid commanding force less than 0.01 N due to numerical errors
        if np.fabs(force_left) < 0.01:
            force_left = 0.0

        if np.fabs(force_right) < 0.01:
            force_right = 0.0

        self.cur_forces = np.array([force_left, force_right])
        self.send_thrust(force_right, self.right_id)
        self.send_thrust(force_left, self.left_id)

        rospy.loginfo("Assigning forces [{}, {}]".format(force_left, force_right))

    def send_thrust(self, force, thruster):
        '''Publish thrust for a particular thruster'''
        self.thrust_pub.publish(
            thrusterNewtons(
                id=thruster,
                thrust=force,
            )
        )

    def set_servo_angles(self, (theta_left, theta_right)):
        rospy.loginfo("Assigning angles [{}, {}]".format(theta_left, theta_right))
        if np.fabs(theta_left) < 0.01:
            theta_left = 0.0

        if np.fabs(theta_right) < 0.01:
            theta_right = 0.0

        self.cur_angles = np.array([theta_left, theta_right])

        # Got the weird angle offset from zdrive2. Not...sure...why...gearing?
        self.servo_pub.publish(
            DynamixelFullConfig(
                id=self.left_id,
                goal_position=(2 * theta_left) + np.pi,
                moving_speed=self.servo_max_rotation,
                torque_limit=1023,
                goal_acceleration=38,
                control_mode=DynamixelFullConfig.JOINT,
                goal_velocity=self.servo_max_rotation,
            )
        )

        self.servo_pub.publish(
            DynamixelFullConfig(
                id=self.right_id,
                goal_position=(2 * theta_right) + np.pi,
                moving_speed=self.servo_max_rotation,
                torque_limit=1023,
                goal_acceleration=38,
                control_mode=DynamixelFullConfig.JOINT,
                goal_velocity=self.servo_max_rotation,
            )
        )


if __name__ == '__main__':
    controller = Controller()
    controller.main_loop()