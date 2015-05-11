#!/usr/bin/python
'''
Authors:
Khaled Hassan, 
Forrest Voight
Jacob Panikulam
'''
from __future__ import division
import rospy
import numpy as np
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Wrench, WrenchStamped, Vector3
from std_msgs.msg import String

rospy.init_node('joystick_command')
pub = rospy.Publisher('wrench', WrenchStamped, queue_size=1)

def got_joy_msg(joy_msg):
    # 6 start, 7 stop
    if joy_msg.buttons[6]:
        pass
    elif joy_msg.buttons[7]:
        pass
    if joy_msg.buttons[1]:
        pass
    elif joy_msg.buttons[3]:
        pass

    axes = list(joy_msg.axes)
    if np.fabs(axes[1]) < 0.1:
        axes[1] = 0.0

    if np.fabs(axes[0]) < 0.1:
        axes[0] = 0.0

    if np.fabs(axes[3]) < 0.1:
        axes[3] = 0.0

    for i in range(len(axes)):
        axes[i] = np.round(axes[i], 2)


    linear_x = axes[1] * max_x_force
    linear_y = axes[0] * max_y_force
    angular_z = axes[3] * -max_z_torque
    seq = joy_msg.header.seq

    wrench = WrenchStamped()
    wrench.wrench = Wrench()
    wrench.wrench.force = Vector3()
    wrench.wrench.torque = Vector3()

    wrench.wrench.force.x = linear_x
    wrench.wrench.force.y = linear_y
    wrench.wrench.force.z = 0
    
    wrench.wrench.torque.x = 0
    wrench.wrench.torque.y = 0
    wrench.wrench.torque.z = angular_z

    wrench.header.seq = seq
    wrench.header.frame_id = '/robot'
    wrench.header.stamp = joy_msg.header.stamp
    pub.publish(wrench)

# These values are hardcoded for now.
max_x_force = 75
max_y_force = 75
max_z_torque = 75

rospy.Subscriber('/joy', Joy, got_joy_msg, queue_size=2)

rospy.spin()    

