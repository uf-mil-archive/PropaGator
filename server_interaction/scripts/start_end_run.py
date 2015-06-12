#!/usr/bin/env python
import rospy
import roslib
roslib.load_manifest('server_interaction')
import requests
import json
import sys
import time
from std_msgs.msg import String
from server_interaction.srv import start_end_run

def StoreServerUrl(serverUrl):

	global url
	url = serverUrl.data

def StoreCourseCode(courseCode):

	global course
	course = courseCode.data

def SendStartEndRun(runStatus):
	
	start_or_end = runStatus.status
	startLink = '/run/start/%s/UF' %(course)
	endLink = '/run/end/%s/UF' %(course)
	headers = {'content-type':'application/json'}	

	if start_or_end == "start":
		fullLink = url + startLink
		r = requests.post(fullLink, headers = headers, verify = False)
		print r.text
		if r.json() == 'true':
			return True
		else:
			return False	
	if start_or_end== "end":
		fullLink = url + endLink
		r = requests.post(fullLink, headers = headers, verify = False)
		print r.text
		if r.json() == 'true':
			return True	
		else:
			return False	

def start_end_run_server():

	rospy.init_node('start_end_run_sender')
	rospy.Subscriber('main_server_url', String, StoreServerUrl)
	rospy.Subscriber('course_code', String, StoreCourseCode)
	s = rospy.Service('send_start_end_run', start_end_run, SendStartEndRun)
	print('ready to send run information')
	rospy.spin()


if __name__ == '__main__':
	try:
		start_end_run_server()

	except rospy.ROSInterruptException:
		pass		