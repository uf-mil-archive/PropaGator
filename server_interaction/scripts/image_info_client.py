#!/usr/bin/env python
import rospy
import roslib
roslib.load_manifest('server_interaction')
import sys
from server_interaction.srv import image_info

#This is an example of a client to use the service that sends
#the identified image to the server

def image_info_client():
	#wait for service to be available
	rospy.wait_for_service('send_image_info')
	try:
		#call service 'send_image_info' which receives a type image_info
		sendInfo = rospy.ServiceProxy('send_image_info', image_info)
		#once the right image is identified
		#return send the image name
		file_name = '2.jpg'
		#and the shape that was indentified
		image_shape = 'TWO'
		#the response objects returns the status that the server
		#returned either true or false depending on whether
		#the image wasidentified correctly
		response = sendInfo(file_name, image_shape)
		if response.is_right_image == True:
			print "\033[0;32m%s\033[0m" %response.is_right_image
			return response.is_right_image
		else:
			print "\033[0;31m%s\033[0m" %response.is_right_image
			return response.is_right_image
	except rospy.ServiceException:
		print("Service call failed")

if __name__ == '__main__':	
	image_info_client()			