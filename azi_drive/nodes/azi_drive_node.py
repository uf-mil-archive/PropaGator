#!/usr/bin/python
from __future__ import division
import roslib; roslib.load_manifest('azi_drive')
from azimuth_drive import Azi_Drive
from azimuth_drive import clamp_angles
import numpy as np
import rospy
from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster

from geometry_msgs.msg import WrenchStamped
from motor_control.msg import thrusterNewtons
from dynamixel_servo.msg import DynamixelFullConfig
from time import time
from std_msgs.msg import Bool, Float64

from azi_drive.srv import AziFloat

'''
Max thrust: 100
Min thrust: -100
Max angle: pi/2
min angle: -pi/2
'''

class Controller(object):
    def __init__(self, rate=20):
        '''I can't swim on account of my ass-mar'''
        self.rate = rate
        self.servo_max_rotation = 0.3
        self.controller_max_rotation = self.servo_max_rotation / self.rate
        print rospy.get_param('~simulate')
        if rospy.get_param('~simulate', False) is False:
            print 'simulate is false'
            rospy.Subscriber("control_arbiter", Bool, self.control_callback)
        else: print "simulate is true"
        rospy.Subscriber("pwm1_alias", Float64, self.pwm1_cb)
        rospy.Subscriber("pwm2_alias", Float64, self.pwm2_cb)
        
        # rospy.init_node('azi_drive', log_level=rospy.WARN)

        # Set up kill handling
        self.kill_listener = KillListener(self.on_kill, self.on_unkill)
        self.kill_broadcaster = KillBroadcaster(id='Azi_drive', description='Azi drive node shutdown')
        try:
            self.kill_broadcaster.clear()
        except rospy.service.ServiceException, e:
            rospy.logwarn(str(e))
        self.killed = False
	self.float = False
        self.rc_takeover = False

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

	# Service Initializations
	float_srv = rospy.Service('float_mode', AziFloat, self.set_float)

    def on_kill(self):
        self.killed = True
        rospy.logwarn('Azi drive killed because: ' + str(self.kill_listener.get_kills()))

    def on_unkill(self):
        self.killed = False
        rospy.loginfo('Azi drive unkilled')

    def convertNewtonsToPW(self, newtons):
        out = newtons

        if out < .0015:
            #out = 0.0055*out**3 + 0.224*out**2 + 3.9836 * out + 86.679
            out = ((self.REV_CONV * out + self.ZERO_NEWTONS)+150)*-1.4
            #print("Reverse: %i" % out)
        elif out > .0015:
            #out = 0.0016*out**3 - 0.1027*out**2 + 2.812*out + 96.116
            out = ((self.FWD_CONV * out + self.ZERO_NEWTONS)-75)*((1/out)/150)
            #print("Forward: %i" % out)
        else:
            #out = ZERO_DEG;
            out = self.ZERO_NEWTONS
            #print("Zero: %i" % ZERO_PW)

        #Bounds should be taken care of in the motor callback
        #Just in case we double check the bounds after conversion
        #print "Bounded out: %i" % out
        return out

    def pwm1_cb(self, msg):
        temp = self.convertNewtonsToPW(msg.data)
        self.pwm_forces[0] = temp

    def pwm2_cb(self, msg):
        temp = self.convertNewtonsToPW(msg.data)
        self.pwm_forces[1] = temp


    def main_loop(self):
        rate = rospy.Rate(self.rate)
        iteration_num = 0
        while not rospy.is_shutdown():
            # If killed zero everything
            if self.killed == True:
                angles = np.array([0, 0])
                self.set_servo_angles(angles)
                self.set_forces((0.0, 0.0))
            # If rc takeover set thruster angles to 0 and give thrust control to RC
            elif self.rc_takeover == True:
                angles = np.array([0, 0])
                self.set_servo_angles(angles)
                print self.pwm_forces
                self.set_forces(self.pwm_forces)
	    elif self.float == True:
                angles = np.array([0, 0])
                self.set_servo_angles(angles)
                self.set_forces((0.0, 0.0))
            # Otherwise normal operation
            else:
                iteration_num += 1

                cur_time = time()
                rospy.logdebug("Targeting Fx: {} Fy: {} Torque: {}".format(self.des_fx, self.des_fy, self.des_torque))
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

                # toc = time() - cur_time
                # print 'Took {} seconds'.format(toc)

                d_theta, d_force, success = thrust_solution
                if any(np.fabs(self.cur_angles + d_theta) > np.pi / 2):
                    rospy.logwarn('|cur_angles + d_theata| > pi/2, setting angles to 0')
                    self.cur_angles = np.array([0.0, 0.0])
                    continue

                self.cur_angles += d_theta
                self.cur_forces += d_force

                if iteration_num > 8:
                    iteration_num = 0

                    self.set_servo_angles(self.cur_angles)
                    if success:
                        self.set_forces(self.cur_forces)
                    else:
                        rospy.logwarn("AZI_DRIVE: Failed to attain valid solution setting forces to 0")
                        rospy.logwarn("Offending wrench:\n" + str(self.last_wrench))
                        self.set_forces((0.0, 0.0))

                    rate.sleep()
                    rospy.logdebug("Achieving net: {}".format(np.round(Azi_Drive.net_force(self.cur_angles, self.cur_forces)), 2))


    def control_callback(self, msg):
        
        if msg.data == True:
            self.rc_takeover = True
        if msg.data == False:
            self.rc_takeover = False

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

        Compute the minimum and maximum wrenches by multiplying the minimum and maximum thrusts 
          by the thrust matrix at the current thruster orientations. This gives a strong estimate
          for what is a feasible wrench in the neighborhood of linearity

        The 0.9 is just a fudge factor for extra certainty
        '''                                                                                                              
        self.last_wrench = msg

        force = msg.wrench.force
        torque = msg.wrench.torque

        # Compute the minimum and maximum wrenches the boat can produce
        #  By linearity, everything in between should be reasonably achievable
        max_fx, max_fy, _ = Azi_Drive.net_force(self.cur_angles, [80, 80])
        min_fx, min_fy, _ = Azi_Drive.net_force(self.cur_angles, [-70, -70])

        _, _, max_torque = Azi_Drive.net_force(self.cur_angles, [-70, 80])
        _, _, min_torque = Azi_Drive.net_force(self.cur_angles, [80, -70])

        # max_f* could be positive, and min_f* could be negative; clip requires ordered inputs
        fx_range = (min_fx, max_fx)
        fy_range = (min_fy, max_fy)
        tz_range = (min_torque, max_torque)
        #print '----Ranges-----'
        #print 'fx\t', fx_range
        #print 'fy\t', fy_range
        #print 'z\t', tz_range

        self.des_fx = np.clip(force.x, min(fx_range), max(fx_range)) * 0.9 
        # I put a negative sign here to work with Forrest's pd_controller
        self.des_fy = -np.clip(force.y, min(fy_range), max(fy_range)) * 0.9
        self.des_torque = np.clip(torque.z, min(tz_range), max(tz_range)) * 0.9

        self.last_msg_time = time()

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
            DynamixelFullConfig(
                id=servo,
                goal_position=clamp_angles(angle),
                moving_speed=self.servo_max_rotation * 16,
                torque_limit=1023,
                goal_acceleration=38,
                control_mode=DynamixelFullConfig.JOINT,
                goal_velocity=0.0  # This is for wheel mode, we don't use it
            )
        )

    # Used to place boat in floating mode (Thrusters are turned off)
    def set_float(self, mode):
	if self.float is not mode.float:
		self.float = mode.float
		rospy.loginfo('Azi drive float-mode is ' + str(mode.float))
		#return True	# Used to determine if the change was succesful 
		return mode.float
	else:
		rospy.loginfo('Azi drive float-mode is unchanged & currently ' + str(self.float))
		#mode.out = mode.float
		#return True
		return mode.float

if __name__ == '__main__':
    rospy.init_node('azi_drive')#, log_level=rospy.DEBUG)
    controller = Controller()
    # Add a shutdown hook to stop motors and servos when we die
    rospy.on_shutdown(controller.shutdown)
    controller.main_loop()
    rospy.spin()
