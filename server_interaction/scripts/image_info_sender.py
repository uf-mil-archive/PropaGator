#!/usr/bin/env python
import rospy
import roslib
roslib.load_manifest('server_interaction')
import sys
import requests
import json
import os
import time
from std_msgs.msg import String
from std_msgs.msg import Int8
from server_interaction.srv import image_info
import unicodedata
from collections import OrderedDict

### Sends information pertaining the identified image that was received from the server ####
### This is a service. It receives strings file_name (name of the picture), and image_shape(identified shape from image)
### returns boolean is_right_image according to server response.

def StoreCourseInfo(courseInfo):

	#receive the current course e.g. "courseA"
	#and stores it in "course"
	global course
	course = courseInfo.data

def StoreServerUrl(ServerUrl):

	#receive server url and store it in
	#mainUrl
	global mainUrl
	mainUrl = ServerUrl.data

def SendImageInfo(msg):

	imageName = msg.file_name
	shape = msg.image_shape
	postImageLink = '/interop/image/%s/UF' %course
	sendImageInfoLink = '/interop/report/%s/UF' %course
	#creating urls
	putImageOnServerUrl = mainUrl +  postImageLink
	sendImageInfoUrl = mainUrl +  sendImageInfoLink
	###################################### put image on server #############################################
	global path
	#find image on path where it is stored
	path = os.path.join(os.path.expanduser('~'), 'output', 'ServerImages', imageName)
	files = {'file': (imageName, open(path, 'rb'), 'multipart/mixed', {'Expires': '0'})}
	#Send image to server and get an image ID.
	#create request #1 (POST), put image file on server
	request1 = requests.post(putImageOnServerUrl, files=files, verify=False)
	print (" ")
	print "Information returned by server: "
	print request1.text
	print (" ")
	#getting image ID from server. Will be a json structure like:
	#{"imaeId":"a4aa8224-07f2-4b57-a03a-c8887c2505c7"}
	# wait one second... just for the heck of it
	time.sleep(1)
	imageID = request1.json()['imageId']
	######################################## send image information #########################################				
	#ready payload to send to server..
	imageID = unicodedata.normalize('NFKD', imageID).encode('ascii','ignore')
	headers = {'Content-Type':'application/json'}
	print "Payload being sent to server: "
	print json.dumps(OrderedDict([("course",course),("team","UF"),("shape",shape),("imageId",imageID)]))
	print (" ")
	payload = OrderedDict([("course",course),("team","UF"),("shape",shape),("imageId",imageID)])
	#create request #2, post image info json structure
	time.sleep(1)
	try:
		request2 = requests.post(sendImageInfoUrl, headers = headers, data = json.dumps(payload), verify = False)
		time.sleep(1)
		print "Request status code: "
		if request2.status_code == 200:
			print "\033[0;32m%s\033[0m" %(request2.status_code)
			print (" ")
		else:
			print "\033[0;31m%s\033[0m" %(request2.status_code)
			print (" ")	
		print "Infromation returned by sever: "
		print request2.text
		if request2.status_code == 200:
			#decode json response from server
			status = request2.json()['success']
			print (" ")
			if status == True:
				print "Is this the right image?"
				print "\033[0;32m%s\033[0m" %status
				return True
			else:
				print "Is this the right image?"
				print "\033[0;31m%s\033[0m" %status
				return False
		else:
			raise rospy.ServiceException		
	except rospy.ServiceException, e:			
		print "Server call failed: %s"%e				
# return status from server to know whether the right image was sent
# if false is returned by the server, i.e. the image shape was not correctly
# identified, return said status so that the node in charge of image 
# analyzing sends another image name

def send_image_info_server():	
	rospy.init_node('send_image_info_server')
	rospy.Subscriber('course_code', String, StoreCourseInfo)
	rospy.Subscriber('main_server_url', String, StoreServerUrl)
	s = rospy.Service('send_image_info', image_info, SendImageInfo)
	print('Ready to receive image info')
	rospy.spin()

if __name__ == '__main__':	
	send_image_info_server()
	
	
