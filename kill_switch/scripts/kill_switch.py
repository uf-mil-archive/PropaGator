#!/usr/bin/env python
import rospy
import serial
import time
from std_msgs.msg import String
from kill_handling.broadcaster import KillBroadcaster

s = serial.Serial('/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0',baudrate = 9600)

def ReadTerminal():

	global s

	data = s.readline()

	return data

if __name__ == '__main__':

	rospy.init_node('kill_switch')

	kill_broadcaster = KillBroadcaster(rospy.get_name(),"kill switch kill")

	try:

		kill_broadcaster.clear()

	except rospy.service.ServiceException, e:

		rospy.logwarn(str(e))

	kill_switch_pub = rospy.Publisher('kill_switch', String, queue_size = 10)	
	
	while not rospy.is_shutdown():

		global status

		status = ReadTerminal()

		status = str(status)

		if 'O' in status:

			msg = "OK"

			rospy.loginfo(msg)

			kill_switch_pub.publish(msg)

		else:

			kill_broadcaster.send(True)	

			msg = "KILL"

			rospy.loginfo(msg)

			kill_switch_pub.publish(msg)

	rospy.spin()	 	
