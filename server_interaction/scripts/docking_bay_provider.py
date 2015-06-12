#!/usr/bin/env python
import rospy
import roslib
roslib.load_manifest('server_interaction')
import requests
import json
import sys
import time
from std_msgs.msg import String
from server_interaction.msg import docking_bay_sequence

## This node publishes the docking bay sequence received frmom the server
## It publishes a message containing:
#(string) first_dock_symbol
#(string) first_dock_color
#(string) second_dock_symbol
#(string) second_dock_color
# In that order


def StoreCourseCode(courseCode):

	global course
	course = courseCode.data
	postDockingSequence()

def StoreServerUrl(serverUrl):

	global mainUrl
	mainUrl = serverUrl.data

def postDockingSequence():	

	try:
		sublinkMain = '/automatedDocking/%s/UF' %course
		url = mainUrl +  sublinkMain
		r = requests.get(url, verify = False) #creating request object
		#evaluating response status code
		try:
			if(r.status_code == 200):
				#decoding json received from server				
				dataList = r.json()['dockingBaySequence']
				firstDockInfo = dataList[0]
				secondDockInfo = dataList[1]
				firstDockSymbol = firstDockInfo['symbol']				
				firstDockColor = firstDockInfo['color']				
				secondDockSymbol = secondDockInfo['symbol']							
				secondDockColor =  secondDockInfo['color']

				####### message with data to be published #######

				msg = docking_bay_sequence()
				msg.first_dock_symbol = firstDockSymbol
				msg.first_dock_color = firstDockColor
				msg.second_dock_symbol = secondDockSymbol
				msg.second_dock_color = secondDockColor
				docking_sequence_pub = rospy.Publisher('docking_bay_sequence', docking_bay_sequence, queue_size=10)				
				rate = rospy.Rate(1)				
				while not rospy.is_shutdown():					
					rospy.loginfo(msg)					
					docking_sequence_pub.publish(msg)					
					rate.sleep()			
			else:			
				raise rospy.ServiceException
		except rospy.ServiceException:
			time.sleep(3)			
			postDockingSequence()				
	except NameError:
		pass			
				

def main():
	
	rospy.init_node('docking_bay_provider')
	#subscrie to 'course_code'	
	rospy.Subscriber('course_code', String, StoreCourseCode)
	#subscribe to 'main_server_url'
	rospy.Subscriber('main_server_url', String, StoreServerUrl)
	#wait five seconds 
	time.sleep(5)
	rospy.spin()

if __name__ == '__main__':	
	try:	
		main()	
	except rospy.ROSInterruptException:
		pass		

