#!/usr/bin/python
from __future__ import division
import roslib; roslib.load_manifest('azi_drive')
from azimuth_drive import Azi_Drive
from azimuth_drive import clamp_angles
import numpy as np
import rospy
from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster
from controller.srv import register_controller

from geometry_msgs.msg import WrenchStamped
from motor_control.msg import thrusterNewtons
from controller.msg import ControlDynamixelFullConfig
from dynamixel_servo.msg import DynamixelFullConfig
from time import time
from std_msgs.msg import Bool, Float64

'''
Max thrust: 100
Min thrust: -100
Max angle: pi/2
min angle: -pi/2
'''

class Controller(object):
    def __init__(self, rate=100):
        '''I can't swim on account of my ass-mar'''

        # Register azi_drive as a controller
        rospy.wait_for_service('/controller/register_controller', 30.0)
        rospy.ServiceProxy('/controller/register_controller', register_controller)('azi_drive')

        self.rate = rate
        self.servo_max_rotation = 0.1
        self.controller_max_rotation = self.servo_max_rotation / self.rate
        
        # rospy.init_node('azi_drive', log_level=rospy.WARN)

        # Set up kill handling
        self.kill_listener = KillListener(self.on_kill, self.on_unkill)
        self.kill_broadcaster = KillBroadcaster(id='Azi_drive', description='Azi drive node shutdown')
        try:
            self.kill_broadcaster.clear()
        except rospy.service.ServiceException, e:
            rospy.logwarn(str(e))
        self.killed = False

        rospy.logwarn("Setting maximum rotation speed to {} rad/s".format(self.controller_max_rotation))
        Azi_Drive.set_delta_alpha_max(self.controller_max_rotation)
        #Azi_Drive.set_thrust_bound((-50,50))
        
        # These should not be queued! Old commands are garbage.
        # Unfortunately, we have to queue these, because the subscriber cannot process two sequential
        #  thrust messages as quickly as they are sent
        self.thrust_pub = rospy.Publisher('thruster_config', thrusterNewtons, queue_size=4)
        self.servo_pub = rospy.Publisher('controller/dynamixel_full_config', ControlDynamixelFullConfig, queue_size=4)
        self.next_wrench = WrenchStamped().wrench
        rospy.Subscriber('wrench', WrenchStamped, self._wrench_cb, queue_size=1)

        # Thrust topic id's for each thruster
        self.left_id = 3
        self.right_id = 2

        # Time between messages before azi_drive shuts off
        # self.control_timeout = 2  # secs
        self.control_timeout = np.inf

        self.default_angles = np.array([0.0, 0.0], dtype=np.float32)
        self.default_forces = np.array([0.0, 0.0], dtype=np.float32)

        self.pwm_forces = np.array([0.0, 0.0], dtype=np.float64)

        # Left, Right
        self.cur_angles = self.default_angles[:]
        self.cur_forces = self.default_forces[:]

        self.prev_angles = None
        self.set_servo_angles(self.cur_angles)
        self.set_forces(self.cur_forces)

        # Initializations
        self.last_msg_time = time()
        self.des_fx, self.des_fy, self.des_torque = 0.0, 0.0, 0.0

        self.MAX_NEWTONS =  100.0         #Full forward Jacksons motors
        self.MIN_NEWTONS =  -100.0        #Full reverse Jacksons
        self.ABS_MAX_PW = .002
        self.ABS_MIN_PW = .001
        self.ZERO_NEWTONS = 0
        self.REV_CONV = (self.ZERO_NEWTONS - self.MIN_NEWTONS) / (0 - self.ABS_MIN_PW)
        self.FWD_CONV = (self.MAX_NEWTONS - self.ZERO_NEWTONS) / self.ABS_MAX_PW



    def on_kill(self):
        self.killed = True
        rospy.logwarn('Azi drive killed because: ' + str(self.kill_listener.get_kills()))

    def on_unkill(self):
        self.killed = False
        rospy.loginfo('Azi drive unkilled')


    def main_loop(self):
        rate = rospy.Rate(self.rate)
        iteration_num = 0
        while not rospy.is_shutdown():
            # If killed zero everything
            if self.killed == True:
                angles = np.array([0, 0])
                self.set_servo_angles(angles)
                self.set_forces((0.0, 0.0))
            # Otherwise normal operation
            else:
                iteration_num += 1
                cur_time = time()

                rospy.logdebug("Targeting Fx: {} Fy: {} Torque: {}".format(self.des_fx, self.des_fy, self.des_torque))
                if (cur_time - self.last_msg_time) > self.control_timeout:
                    rospy.logwarn("AZI_DRIVE: No control input in over {} seconds! Turning off motors".format(self.control_timeout))
                    self.stop()

                thrust_solution = Azi_Drive.map_thruster(
                    fx_des=self.des_fx,
                    fy_des=-1 * self.des_fy,
                    m_des= -1 * self.des_torque, 
                    alpha_0= self.cur_angles,
                    u_0=self.cur_forces,
                )

                # toc = time() - cur_time
                # print 'Took {} seconds'.format(toc)

                d_theta, d_force, success = thrust_solution
                if any(np.fabs(self.cur_angles + d_theta) > np.pi/2):
                    self.cur_angles = np.array([0.0, 0.0])
                    continue

                self.cur_angles += d_theta
                self.cur_forces += d_force

                if iteration_num > 10:
                    iteration_num = 0
                    self._reset_desired_state()
                    self.set_servo_angles(self.cur_angles)
                    if success:
                        self.set_forces(self.cur_forces)
                    else:
                        rospy.logwarn("AZI_DRIVE: Failed to attain valid solution setting forces to 0")
                        self.set_forces((0.0, 0.0))
                    rospy.logdebug("Achieving net: {}".format(np.round(Azi_Drive.net_force(self.cur_angles, self.cur_forces)), 2))

                rate.sleep()

    def shutdown(self):
        rospy.logwarn("AZI_DRIVE: Shutting down Azi Drive")
        self.kill_broadcaster.send(True)

    def stop(self):
        self.des_fx, self.des_fy, self.des_torque = 0.0, 0.0, 0.0

    def reset_all(self):
        rospy.logwarn("Resetting Azi Drive orientations and thrusts")
        self.set_servo_angles(self.default_angles)
        self.set_forces(self.default_forces)

    def _wrench_cb(self, msg):
        '''Recieve a force, torque command for the mapper to achieve
        '''
        self.next_wrench = msg.wrench

        self.last_msg_time = time()

    def _reset_desired_state(self):
        force = self.next_wrench.force
        torque = self.next_wrench.torque

        self.des_fx = force.x
        self.des_fy = force.y
        self.des_torque = torque.z

    def set_forces(self, (force_left, force_right)):
        '''Issue commands to the thrusters'''

        # Avoid commanding force less than 0.01 N due to numerical errors
        if np.fabs(force_left) < 0.01:
            force_left = 0.0

        if np.fabs(force_right) < 0.01:
            force_right = 0.0

        self.send_thrust(force_right, self.right_id)
        self.send_thrust(force_left, self.left_id)

        rospy.logdebug("Assigning forces [{}, {}]".format(round(force_left, 2), round(force_right, 2)))

    def send_thrust(self, force, thruster):
        '''Publish thrust for a particular thruster'''
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
            if (all(np.fabs(np.array([theta_left, theta_right]) - self.prev_angles) < 0.2)):
                rospy.logdebug("Angle change too small, holding {}".format(np.round(self.cur_angles, 2)))
                return

        rospy.logdebug("Assigning angles [{}, {}]".format(round(theta_left, 2), round(theta_right, 2)))

        self.prev_angles = np.copy(self.cur_angles)

        theta_left = np.round(theta_left, 1)
        theta_right = np.round(theta_right, 1)

        self.send_angle(theta_left, self.left_id)
        self.send_angle(theta_right, self.right_id)

    def send_angle(self, angle, servo):
        self.servo_pub.publish(
            ControlDynamixelFullConfig(
                controller = 'azi_drive',
                config = DynamixelFullConfig(
                    id=servo,
                    goal_position=clamp_angles(angle),
                    moving_speed=self.servo_max_rotation * 16,
                    torque_limit=1023,
                    goal_acceleration=38,
                    control_mode=DynamixelFullConfig.JOINT,
                    goal_velocity=0.0  # This is for wheel mode, we don't use it
                )
            )
        )

if __name__ == '__main__':
    rospy.init_node('azi_drive')#, log_level=rospy.DEBUG)
    controller = Controller()
    # Add a shutdown hook to stop motors and servos when we die
    rospy.on_shutdown(controller.shutdown)
    controller.main_loop()
    rospy.spin()
