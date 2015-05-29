#!/usr/bin/env python

import rospy
import roslib


from std_msgs.msg import Bool, Int64

def callback(msg):

	admin = rospy.Publisher("control_arbiter", Bool, queue_size =1)
	if msg.data == True:
		admin.publish(False)

	if msg.data == False:
		admin.publish(True)

rospy.init_node("control_arbiter")
rospy.Subscriber("rc/status", Bool, callback)
rospy.spin()	




