#!/usr/bin/env python

import rospy
import roslib
roslib.load_manifest('server_interaction')
from server_interaction.srv import start_end_run

# This is an example of a client code to send "start run"
# or "end run" on call to the competition server

def start_end_run_client():
	#wait until the service is available
	rospy.wait_for_service('send_start_end_run')

	try:
		#define sendStatus as the service of type start_end_run
		sendStatus = rospy.ServiceProxy('send_start_end_run', start_end_run)
		#response will be the return from the service
		#passing an argument to the service
		#The argument would be 'start' if starting a run
		#or 'end' if ending a run
		response = sendStatus('start')
		#print the response from the service. This is a boolean
		#based on what the server returned: either true or false
		#as per the rules, return from server is false is run void
		print response.success
		return response.success

	except rospy.ServiceException:
		print("Service call failed")

if __name__ == '__main__':
	
	start_end_run_client()			
