#!/usr/bin/python
'''Contains stopping services
'''
from __future__ import division
from azi_drive.srv import *
import roslib; roslib.load_manifest('azi_drive')
from azimuth_drive import clamp_angles
import numpy as np
import rospy
from motor_control.msg import thrusterNewtons
from dynamixel_servo.msg import DynamixelFullConfig
from azimuth_drive import clamp_angles

thrust_pub = rospy.Publisher('thruster_config', thrusterNewtons, queue_size=4)
servo_pub = rospy.Publisher('dynamixel/dynamixel_full_config', DynamixelFullConfig, queue_size=4)


def reset_thrust():
    for thruster in [2, 3]:
        thrust_pub.publish(
            thrusterNewtons(
                id=thruster,
                thrust=0.0,
            )
        )


def reset_servo_angles():
    for servo in [2, 3]:
        servo_pub.publish(
            DynamixelFullConfig(
                id=servo,
                goal_position=clamp_angles(0.0),
                moving_speed=1.0,
                torque_limit=1023,
                goal_acceleration=38,
                control_mode=DynamixelFullConfig.JOINT,
                goal_velocity=1.0,
            )
        )


def stop_boat_service(srv):
    reset_servo_angles()
    reset_thrust()
    return AziStopResponse()


if __name__ == '__main__':
    rospy.init_node('azi_drive_control_manager')
    rospy.Service('azi_drive/stop', AziStop, stop_boat_service)
    rospy.spin()