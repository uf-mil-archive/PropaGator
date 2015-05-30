#! /usr/bin/env python

from std_msgs.msg import Float64
import rospy
import boat_scripting

def less_gain():
	rospy.init_node("gain_adjust")
	d_pub = rospy.Publisher("pd_d_gain", Float64, queue_size = 1)
	p_pub = rospy.Publisher("pd_p_gain", Float64, queue_size = 1)
	d_pub.publish(10)
	p_pub.publish(.05)

def more_gain():
	rospy.init_node("gain_adjust")
	d_pub = rospy.Publisher("pd_d_gain", Float64, queue_size = 1)
	p_pub = rospy.Publisher("pd_p_gain", Float64, queue_size = 1)
	d_pub.publish(20)
	p_pub.publish(.15)

