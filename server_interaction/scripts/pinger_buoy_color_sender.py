#!/usr/bin/env python
import rospy
import roslib
roslib.load_manifest('server_interaction')
import requests
import json
import sys
import time
from std_msgs.msg import String
from server_interaction.srv import buoy_color

### This is a service that send the identified pinger buoy color to the server
### It receives buoy_color (string) and return is_right_buoy (bool) according to server response

def StoreServerUrl(serverUrl):

	#store the main url for the server

	global url
	url = serverUrl.data

def StoreCourseCode(courseCode):

	#store course that we are currently attempting e.g. "courseA"

	global course
	course = courseCode.data	

def SendBuoyColor(buoyColor):
	
	#color of the pinger buoy e.g. "blue"

	color = buoyColor.buoy_color
	reportColorLink = '/pinger/%s/UF' %course
	fullLink = url + reportColorLink

	#post test server
	#fullLink = 'https://posttestserver.com/post.php'

	jsonString = {'course':'temp','team':'UF','buoyColor':'temp'}
	jsonString['course'] = course
	jsonString['buoyColor'] = color
	headers = {'content-type': 'application/json'}
	
	try:

		#create request objet: post link is "fullLink", headers, data is the json string.
			
		r = requests.post(fullLink, headers=headers, data=json.dumps(jsonString), verify=False)

		if(r.status_code == 200): 

			success = r.json()

			#if the right buoy color was sent, server return a json structure: {"success":"true"}.. else false

			if(success['success'] == "true"):
			
			# return status from server to tell whether the right buoy was identified or not

				return True
			
			else:
				
				return False
		
		else:
			
			raise rospy.ServiceException
	
	except rospy.ServiceException:
			
			print "Server call failed"
						

def send_buoy_color_server():

	#initialize node

	rospy.init_node('buoy_color_sender')

	#subscribe to topics

	rospy.Subscriber('main_server_url', String, StoreServerUrl)
	rospy.Subscriber('course_code', String, StoreCourseCode)

	#initialize service

	s = rospy.Service('send_buoy_color', buoy_color, SendBuoyColor)
	print('ready to receive buoy color')
	rospy.spin()

if __name__ == '__main__':
	
	try:

		send_buoy_color_server()

	except rospy.ROSInterruptException:
		pass		
