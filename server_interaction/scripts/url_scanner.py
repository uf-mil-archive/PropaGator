#!/usr/bin/env python
import rospy
import sys
from std_msgs.msg import String

## Scans information pertaining the server
def ScanMainUrl():
	
	mainurl = raw_input("\033[1;34mWhat is the main SERVER URL?: \033[0m")
	if "http://" not in mainurl:
		mainurl = "http://"+mainurl
		return mainurl
	else:		
		return mainurl

def ScanCourseCode():

	coursecode = raw_input("\033[1;34mWhat COURSE are we attempting?: \033[0m")
	return coursecode

def main():

	mainurl = ScanMainUrl()
	coursecode = ScanCourseCode()
	mainurl_pub = rospy.Publisher('main_server_url', String, queue_size=10)
	coursecode_pub = rospy.Publisher('course_code', String, queue_size=10)
	rospy.init_node('url_challenge_course_provider')
	rate = rospy.Rate(1)

	while not rospy.is_shutdown():
		
		rospy.loginfo(mainurl)	
		rospy.loginfo(coursecode)
		mainurl_pub.publish(mainurl)
		coursecode_pub.publish(coursecode)
		rate.sleep()

if __name__ == '__main__':
	try:
		main()
	except rospy.ROSInterruptException:
		pass	
