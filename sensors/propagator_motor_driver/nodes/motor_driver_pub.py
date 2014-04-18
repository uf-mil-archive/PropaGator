#!/usr/bin/env python

import time
import traceback

import rospy
from std_msgs.msg import Header

from thruster_handling.broadcaster import ThrusterBroadcaster
from uf_common import interpolate

from propagator_motor_driver import MotorDriver
from propagator_motor_driver.msg import motor_driver_statistics


rospy.init_node('motor_driver')

motor_driver_statistics_publisher = rospy.Publisher('motor_driver_statistics', motor_driver_statistics)

message_received = False
port = rospy.get_param('~port')
thruster_id = rospy.get_param('~id')
thruster_position = rospy.get_param('~position')
thruster_direction = rospy.get_param('~direction')
output_list = rospy.get_param('~output_list')
assert sorted(output_list) == output_list
force_list = rospy.get_param('~force_list')
assert sorted(force_list) == force_list
assert output_list[0] == -1
assert output_list[-1] == 1

motordriver = MotorDriver.MotorDriver(port)

lifetime = rospy.Duration(1.)

def apply_command(force):
    global message_received
    message_received = True
    #print 'speed: ',str(int(force*200/max_force)),' motor driver: ',thruster_id
    output = interpolate.sample_curve((force_list, output_list), force)
    if output > 0:
        motordriver.set_forward_speed(thrust)
    elif output < 0:
        motordriver.set_reverse_speed(-thrust)
    else:
        motordriver.stop()
    
    motordriverstat_msg = motor_driver_statistics(
        header=Header(
            stamp=rospy.Time.now(),
            frame_id="/base_link",
        ),
        id=thruster_id,
        current          = "0",#motordriver.get_current(),
        out_voltage  = "0",#motordriver.get_out_voltage(),
        batt_voltage = "0",#motordriver.get_batt_voltage(),
        )
    #motor_driver_statistics_publisher.publish(motordriverstat_msg)
thruster_broadcaster = ThrusterBroadcaster('/base_link', thruster_id,
    lifetime, thruster_position, thruster_direction,
    min(force_list), max(force_list), [0, 0, 0], apply_command)

def timeout(event):
    global message_received
    
    thruster_broadcaster.send()
    
    if not message_received:
        motordriver.stop()
    else:
        message_received = False
rospy.Timer(lifetime/2., timeout)

rospy.spin()

motordriver.stop()
