#!/usr/bin/env python
import rospy
import roslib
roslib.load_manifest('server_interaction')
import requests
import json
import sys
import time
from std_msgs.msg import String
from server_interaction.msg import gate_code



def StoreCourseCode(courseCode):

	global course 
	course = courseCode.data

def SendRequest(serverUrl):
	try:
		mainUrl = serverUrl.data
		sublinkMain = '/obstacleAvoidance/%s/UF' %course
		url = mainUrl +  sublinkMain
		r = requests.get(url, verify = False)	

		if(r.status_code == 200):		
			gatecode_all = r.json()['gateCode']
			#parse json data obtained from server
			temp = gatecode_all.split(",")
			entrance = temp[0].replace("(","")
			exit = temp[1].replace(")","")
			#ready message data to be pubslished
			msg = gate_code()
			msg.entrance = entrance
			msg.exit= exit
			gatecode_pub = rospy.Publisher('gate_code_data', gate_code, queue_size=10)
			rate = rospy.Rate(1)			
			while not rospy.is_shutdown():
				rospy.loginfo(msg)				
				gatecode_pub.publish(msg)				
				rate.sleep()
		else:
			raise rospy.ServiceException	
	except (rospy.ServiceException, NameError) as e:		
		pass		
def main():
	rospy.init_node('gate_code_provider')
	rospy.Subscriber('course_code', String, StoreCourseCode)
	rospy.Subscriber('main_server_url', String, SendRequest)
	rospy.spin()

if __name__ == '__main__':
	try:
		main()
	except rospy.ROSInterruptException:		
		pass	
