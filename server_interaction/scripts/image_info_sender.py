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

	# Test server:

	# putImageOnServerUrl = 'https://posttestserver.com/post.php'

	# sendImageInfoUrl = 'https://posttestserver.com/post.php'

	###################################### put image on server #############################################

	global path

	#find image on path where it is stored

	path = os.path.join(os.path.expanduser('~'), 'output', 'ServerImages', imageName)


	files = {'file': (imageName, open(path, 'rb'), 'multipart/mixed', {'Expires': '0'})}

	#Send image to server and get an image ID.
	#create request #1 (POST), put image file on server

	request1 = requests.post(putImageOnServerUrl, files=files, verify=False)

	#getting image ID from server. Will be a json structure like:
	#{"id":"a4aa8224-07f2-4b57-a03a-c8887c2505c7"}
	# wait two seconds... just for the heck of it
	time.sleep(2)

	imageID = "a4aa8224-07f2-4b57-a03a-c8887c2505c7"
	# imageID = request1.json()['id']

	#########################################################################################################

	######################################## send image information #########################################	
			
	#ready payload to send to server..

	payload = {'course':'temp','team':'UF','shape':'temp','imageID':'temp'} 

	payload['course'] = course

	payload['shape'] = shape

	payload['imageID'] = imageID

	headers = {'content-type':'application/json'}

	#create request #2, post image info json structure

	try:

		request2 = requests.post(sendImageInfoUrl, headers = headers, data = json.dumps(payload), verify = False)

		if request2.status_code == 200:

			#decode json response from server

			status = request2.json()['success']

			print(status)

			if status == "true":

				return True

			else:

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

	print('ready to receive image info')

	rospy.spin()

if __name__ == '__main__':
	
	send_image_info_server()
	
	
