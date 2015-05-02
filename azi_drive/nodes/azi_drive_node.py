#!/usr/bin/python
from __future__ import division
import roslib; roslib.load_manifest('azi_drive')
from azimuth_drive import Azi_Drive
from azimuth_drive import clamp_angles
import numpy as np
import rospy

from geometry_msgs.msg import WrenchStamped
from motor_control.msg import thrusterNewtons
from dynamixel_servo.msg import DynamixelFullConfig
from azi_drive.srv import *
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
        self.servo_max_rotation = 0.3
        self.controller_max_rotation = self.servo_max_rotation / self.rate

        rospy.init_node('azi_drive', log_level=rospy.DEBUG)
        # rospy.init_node('azi_drive', log_level=rospy.WARN)

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

        self.default_angles = np.array([0.0, 0.0], dtype=np.float32)
        self.default_forces = np.array([0.0, 0.0], dtype=np.float32)

        # Left, Right
        self.cur_angles = self.default_angles[:]
        self.cur_forces = self.default_forces[:]

        # self.prev_angles = np.copy(self.cur_angles)
        self.prev_angles = None
        self.set_servo_angles(self.cur_angles)
        self.set_forces(self.cur_forces)

        # Initializations
        self.last_msg_time = time()
        self.des_fx, self.des_fy, self.des_torque = 0.0, 0.0, 0.0

        # Prepare a shutdown service
        rospy.loginfo("----------Waiting for control_manager service-------------")
        rospy.wait_for_service('azi_drive/stop')
        self.stop_boat_proxy = rospy.ServiceProxy('azi_drive/stop', AziStop)

    def main_loop(self):
        rate = rospy.Rate(self.rate)
        while not rospy.is_shutdown():
            cur_time = time()

            rospy.loginfo("Targeting Fx: {} Fy: {} Torque: {}".format(self.des_fx, self.des_fy, self.des_torque))
            if (cur_time - self.last_msg_time) > self.control_timeout:
                rospy.logerr("AZI_DRIVE: No control input in over {} seconds! Turning off motors".format(self.control_timeout))
                self.stop()

            thrust_solution = Azi_Drive.map_thruster(
                fx_des=self.des_fx,
                fy_des=self.des_fy,
                m_des=self.des_torque, 
                alpha_0= self.cur_angles,
                u_0=self.cur_forces,
            )

            toc = time() - cur_time
            print 'Took {} seconds'.format(toc)

            d_theta, d_force = thrust_solution
            self.cur_angles += d_theta
            self.cur_forces += d_force
            self.set_servo_angles(self.cur_angles)
            self.set_forces(self.cur_forces)

            rospy.loginfo("Achieving net: {}".format(np.round(Azi_Drive.net_force(self.cur_angles, self.cur_forces)), 2))

            rate.sleep()

    def shutdown(self):
        rospy.logwarn("AZI_DRIVE: Shutting down Azi Drive")
        self.stop_boat_proxy()

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
        self.last_msg_time = time()

    def set_forces(self, (force_left, force_right)):
        '''Issue commands to the thrusters'''

        # Avoid commanding force less than 0.01 N due to numerical errors
        if np.fabs(force_left) < 0.01:
            force_left = 0.0

        if np.fabs(force_right) < 0.01:
            force_right = 0.0

        # self.cur_forces = np.array([force_left, force_right])
        self.send_thrust(force_right, self.right_id)
        self.send_thrust(force_left, self.left_id)

        rospy.loginfo("Assigning forces [{}, {}]".format(round(force_left, 2), round(force_right, 2)))

    def send_thrust(self, force, thruster):
        '''Publish thrust for a particular thruster'''
        force = np.clip(force, -75, 75)
        self.thrust_pub.publish(
            thrusterNewtons(
                id=thruster,
                thrust=force,
            )
        )

    def set_servo_angles(self, (theta_left, theta_right)):
        if np.fabs(theta_left) < 0.01:
            theta_left = 0.0

        if np.fabs(theta_right) < 0.01:
            theta_right = 0.0

        if self.prev_angles is not None:
            if (all(np.fabs(np.array([theta_left, theta_right]) - self.prev_angles))) < 0.02:
                rospy.loginfo("Angle change too small, holding {}".format(np.round(self.cur_angles, 2)))
                return
        # else:
            # self.prev_angles = np.copy(self.cur_angles)
        rospy.loginfo("Assigning angles [{}, {}]".format(round(theta_left, 2), round(theta_right, 2)))

        self.prev_angles = np.copy(self.cur_angles)

        theta_left = np.round(theta_left, 1)
        theta_right = np.round(theta_right, 1)

        # Got the weird angle offset from zdrive2. Not...sure...why...gearing?
        self.servo_pub.publish(
            DynamixelFullConfig(
                id=self.left_id,
                # goal_position=((2 * theta_left) + np.pi) % (2 * np.pi),
                goal_position=clamp_angles(theta_left),
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
                # goal_position=((2 * theta_right) + np.pi) % (2 * np.pi),
                goal_position=clamp_angles(theta_right),
                moving_speed=self.servo_max_rotation,
                torque_limit=1023,
                goal_acceleration=38,
                control_mode=DynamixelFullConfig.JOINT,
                goal_velocity=self.servo_max_rotation,
            )
        )


if __name__ == '__main__':
    controller = Controller()
    # Add a shutdown hook to stop motors and servos when we die
    rospy.on_shutdown(controller.shutdown)
    controller.main_loop()