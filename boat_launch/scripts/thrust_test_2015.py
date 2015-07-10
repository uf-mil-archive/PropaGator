#!/usr/bin/python

import math
import numpy
import rospy
import cv2
from std_msgs.msg import Float64
from std_msgs.msg import UInt8



if __name__ == '__main__':

    rospy.init_node('thrust_test')

    port_propeller = rospy.Publisher('stm32f3discovery_imu_driver/pwm1', Float64, queue_size = 10)
    stbd_propeller = rospy.Publisher('stm32f3discovery_imu_driver/pwm2', Float64, queue_size = 10)
    rate = rospy.Rate(2)

    thrust = 1.5e-3

    print "Starting thrust test"

    try:

        for x in xrange(0,11):
            port_propeller.publish(thrust) 
            stbd_propeller.publish(thrust)
            rate.sleep()
            thrust += .05e-3 

        raw_input("Press enter to continue...")

        for x in xrange(0,12):
            port_propeller.publish(thrust) 
            stbd_propeller.publish(thrust)
            rate.sleep()
            thrust -= .05e-3 
    except: pass

    port_propeller.publish(1.5e-3)
    stbd_propeller.publish(1.5e-3)
    print "Done with thrust test"
