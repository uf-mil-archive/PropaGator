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
# import ssl
# from functools import wraps

#this service send the start/end run request to notify the server
#that a run is being started or ended.
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
	headers = {'Content-Type':'Application/json'}
	# solution found online to bypass SSL verification
	# def sslwrap(func):
	# 	@wraps(func)
	# 	def bar(*args, **kw):
	# 		kw['ssl_version'] = ssl.PROTOCOL_TLSv1
	# 		return func(*args, **kw)
	# 	return bar
	# ssl.wrap_socket = sslwrap(ssl.wrap_socket)	
	# try:
	if start_or_end == "start":
		fullLink = url + startLink
		print fullLink
		r = requests.post(fullLink, headers = headers, verify = False)
		print r.status_code
		print r.text
		if r.json() == 'true':
			return True
		else:
			return False	
	if start_or_end== "end":
		fullLink = url + endLink
		print fullLink
		r = requests.post(fullLink, headers = headers, verify = False)
		print r.status_code
		print r.text
		if r.json() == 'true':
			return True	
		else:
			return False
	# except ConnectionError as e:
	# 	pass			

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