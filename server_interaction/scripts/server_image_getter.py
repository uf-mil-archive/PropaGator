#!/usr/bin/env python

import rospy
import roslib
roslib.load_manifest('server_interaction')
import sys
import requests
import shutil
import json
import os
import time
from std_msgs.msg import String
from std_msgs.msg import Bool
from server_interaction.msg import images_info

## Gets images from server and publishes strings file_path and image_count
## Path where images were stored and the number of images that were downloaded


# custom iterator
def my_range(start, end, step):

    while start < end:
        yield start
        start += step

#Store the server's ip address        
def StoreServerUrl(ServerUrl):

	global mainUrl
	try:
		mainUrl = ServerUrl.data
	except NameError:
		pass		

#store the course info
def StoreCourseInfo(courseInfo):

	global course
	try:
		course = courseInfo.data
	except NameError:
		pass 

def getImages(send):
	# main sublink
	# the last part of the link needs to be removed i.e.
	# the actual link is: /interop/images/%s/UF
	# this is just to test on my server


	sublinkMain = '/interop/images/%s/UF' %course
	#creating url
	url = mainUrl +  sublinkMain
	headers = {'Content-Type':'text/html'}
	
	try:
		try:
			#creating request object
			r = requests.get(url, headers = headers, verify = False)
			print r.status_code
		except ConnectionError:
			pass
	except NameError:	
		main()			
	time.sleep(2)
	try:
		if r.status_code == 200:
			print (r.text)
			string = r.text
			links = string.split('"')
			links2 = []
			imageNames = []
			counter = -1
			for index in my_range(0, len(links), 1):
			    if index % 2 != 0:
			    	links2.append(links[index])
			for sublink in links2:
				imageNames.append(sublink.split("/")[5])
			global imgCount
			global path
			imgCount = 0					
			for sublink in links2:				
				requestLink = mainUrl + sublink
				print requestLink
				counter = counter + 1
				imageName = imageNames[counter]
				print imageName
				#after having parsed the html that the server returned
				#this generates as many requests as links the server provides
				#and saves the images to ~/output/ServerImages/					
				time.sleep(3)
				r = requests.get(requestLink, stream = True)
				path = os.path.join(os.path.expanduser('~'), 'output', 'ServerImages/')
				if os.path.isdir(path):
					print("Path exists. All good")
				else:
					os.makedirs(path)
				time.sleep(1)						
				with open(path+imageName,'wb') as out_file:					
					shutil.copyfileobj(r.raw, out_file)					
				del r
				imgCount = imgCount + 1
		else:			
			raise rospy.ServiceException('Bad Request')
	except rospy.ServiceException:
		main()		

	msg = images_info()
	msg.file_path = str(path)
	#the test server returned images accompanied by a 
	#metadata files. one per image. Said files are not accessible
	#however they affect the image count. I am doing image count
	#divided by 2 to account for this. Actual server might be different.
	msg.image_count = int(imgCount/2)		

	status_pub = rospy.Publisher('server_images_info', images_info, queue_size=10)
	rate = rospy.Rate(1)	
			
	while not rospy.is_shutdown():

		rospy.loginfo(msg) 			
		status_pub.publish(msg) 				
		rate.sleep()			

def main():
	
	rospy.init_node('from_server_image_getter')	
	#no drone challege, where we receive an image list
	# in html and each request needs to be done separately
	# and we have to process and identify one of them
	rospy.Subscriber('main_server_url', String, StoreServerUrl) 
	rospy.Subscriber('course_code', String, StoreCourseInfo)
	rospy.Subscriber('start_server_interaction', Bool, getImages)
	rospy.spin()
			

if __name__ == '__main__':
	try:
		main()
	except rospy.ROSInterruptException:
		pass	

