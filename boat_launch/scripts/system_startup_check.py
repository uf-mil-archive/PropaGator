#!/usr/bin/python

import math

import numpy

import rospy

import cv2

from std_msgs.msg import Float64
from std_msgs.msg import UInt8

#from dynamixel_servo.msg import DynamixelFullConfig
from dynamixel_servo.msg import DynamixelJointConfig



def system_test():
    rospy.init_node('system_startup_check')

    servo_joint_pub = rospy.Publisher('dynamixel/dynamixel_joint_config', DynamixelJointConfig, queue_size=10)
    port_propeller = rospy.Publisher('stm32f3discovery_imu_driver/pwm1', Float64, queue_size = 10)
    stbd_propeller = rospy.Publisher('stm32f3discovery_imu_driver/pwm2', Float64, queue_size = 10)
    rate = rospy.Rate(20)

    exit = 0 
    x = 0

    

    try:
        while not rospy.is_shutdown():  
            print ("Tilting LIDAR up...")     
            for x in range(0, 20):
                servo_joint_pub.publish(DynamixelJointConfig(
                    id=            1,
                    goal_position= 3.80
                ))
                rate.sleep()
            raw_input("Press enter to continue")

            print ("Tilting LIDAR down...")    
            for x in range(0, 20):
                servo_joint_pub.publish(DynamixelJointConfig(
                    id=            1,
                    goal_position= 2.65
                ))
                rate.sleep()
            raw_input("Press enter to continue")

            print ("Tilting LIDAR to horizontal...")     
            for x in range(0, 20):
                servo_joint_pub.publish(DynamixelJointConfig(
                    id=            1,
                    goal_position= 3.14
                ))
                rate.sleep()
            raw_input("Press enter to continue")

            print ("Deploying SONAR...")     
            for x in range(0, 125):
                servo_joint_pub.publish(DynamixelJointConfig(
                    id=            4,
                    goal_position= -2
                ))
                rate.sleep()
            raw_input("Press enter to continue")

            print ("Recovering SONAR...")    
            for x in range(0, 175):
                servo_joint_pub.publish(DynamixelJointConfig(
                    id=            4,
                    goal_position= 4.2
                ))
                rate.sleep()
            raw_input("Press enter to continue")

            print ("Moving port thruster to the left...")     
            for x in range(0, 20):
                servo_joint_pub.publish(DynamixelJointConfig(
                    id=            3,
                    goal_position= 0.0
                ))
                rate.sleep()
            raw_input("Press enter to continue")

            print ("Moving port thruster to the right...")     
            for x in range(0, 20):
                servo_joint_pub.publish(DynamixelJointConfig(
                    id=            3,
                    goal_position= 6.28
                ))
                rate.sleep()
            raw_input("Press enter to continue")

            print ("Centering port thruster...")     
            for x in range(0, 20):
                servo_joint_pub.publish(DynamixelJointConfig(
                    id=            3,
                    goal_position= math.pi
                ))
                rate.sleep()
            raw_input("Press enter to continue")

#*******************************************************************

            print ("Spinning port thruster forward...") 
            for x in range(0, 20):
                speed = 1.55e-3
                port_propeller.publish(speed)                 
                rate.sleep()
            raw_input("Press enter to continue")

            print ("Spinning port thruster backward...") 
            for x in range(0, 20):
                speed = 1.45e-3
                port_propeller.publish(speed)                 
                rate.sleep()
            raw_input("Press enter to continue")

            print ("stopping port thruster...") 
            for x in range(0, 10):
                speed = 1.50e-3
                port_propeller.publish(speed)                 
                rate.sleep()
            raw_input("Press enter to continue")

#*******************************************************************

            print ("Moving stbd thruster to the left...")     
            for x in range(0, 20):
                servo_joint_pub.publish(DynamixelJointConfig(
                    id=            2,
                    goal_position= 0.0
                ))
                rate.sleep()
            raw_input("Press enter to continue")
            
            print ("Moving stbd thruster to the right...")     
            for x in range(0, 20):
                servo_joint_pub.publish(DynamixelJointConfig(
                    id=            2,
                    goal_position= 6.28
                ))
                rate.sleep()
            raw_input("Press enter to continue")

            print ("Centering stbd thruster...")     
            for x in range(0, 20):
                servo_joint_pub.publish(DynamixelJointConfig(
                    id=            2,
                    goal_position= math.pi
                ))
                rate.sleep()
            raw_input("Press enter to continue")

#*******************************************************************

            print ("Spinning port thruster forward...") 
            for x in range(0, 20):
                speed = 1.55e-3
                stbd_propeller.publish(speed)                 
                rate.sleep()
            raw_input("Press enter to continue")

            print ("Spinning port thruster backward...") 
            for x in range(0, 20):
                speed = 1.45e-3
                stbd_propeller.publish(speed)                 
                rate.sleep()
            raw_input("Press enter to continue")

            print ("stopping port thruster...") 
            for x in range(0, 10):
                speed = 1.50e-3
                stbd_propeller.publish(speed)                 
                rate.sleep()
#*******************************************************************

            break
    except KeyboardInterrupt:
        pass


if __name__ == '__main__':
    try:
        system_test()
    except rospy.ROSInterruptException:
        pass
