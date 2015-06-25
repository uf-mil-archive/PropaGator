#!/usr/bin/env python
import rospy
import roslib
roslib.load_manifest('server_interaction')
from server_interaction.srv import buoy_color

#this is an example client to use the service 
#that send the identified pinger buoy color

def buoy_color_client():
	#wait for service to be available
	rospy.wait_for_service('send_buoy_color')
	try:
		#set sendColor to use the service 'send_buoy_color' which
		#takes in a type buoy_color
		sendColor = rospy.ServiceProxy('send_buoy_color', buoy_color)
		#pass the parameter onto the method. The paramenter is the 
		#color of the buoy that was identified as the pinger buoy
		response = sendColor('red')
		#response returns the status that the server returns
		#either true or false depending on whether the right buoy
		#was identified
		if response.is_right_buoy == True:
			print "Was the right buoy identified?"
			print "\033[0;32m%s\033[0m" %response.is_right_buoy
			print (" ")
			return response.is_right_buoy
		else:
			print "Was the right buoy identified?"
			print "\033[0;31m%s\033[0m" %response.is_right_buoy
			print (" ")
			return response.is_right_buoy	
	except rospy.ServiceException:
		print("Service call failed")

if __name__ == '__main__':	
	buoy_color_client()			
