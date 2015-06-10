#!/usr/bin/env python
import rospy
import sys
from std_msgs.msg import String

## Scans information pertaining the server



def ScanMainUrl():
	
	mainurl = raw_input("What is the MAIN server URL?: ")	
	return mainurl

def ScanCourseCode():

	coursecode = raw_input("What COURSE are we attempting?: ")
	return coursecode

def ScanCurrentChallenge():

	currentchallenge = raw_input("what CHALLENGE are we attemting?: ")
	return currentchallenge	


def main():

	mainurl = ScanMainUrl()
	coursecode = ScanCourseCode()
	currentchallenge = ScanCurrentChallenge()
	mainurl_pub = rospy.Publisher('main_server_url', String, queue_size=10)
	coursecode_pub = rospy.Publisher('course_code', String, queue_size=10)
	currentchallenge_pub = rospy.Publisher('current_challenge', String, queue_size=10)
	rospy.init_node('url_challenge_course_provider')
	rate = rospy.Rate(1)

	while not rospy.is_shutdown():
		
		rospy.loginfo(mainurl)	
		rospy.loginfo(coursecode)
		rospy.loginfo(currentchallenge)
		mainurl_pub.publish(mainurl)
		coursecode_pub.publish(coursecode)
		currentchallenge_pub.publish(currentchallenge)
		rate.sleep()

if __name__ == '__main__':
	try:
		main()
	except rospy.ROSInterruptException:
		pass	
