#!/usr/bin/env python
import rospy
import sys
from std_msgs.msg import String
from server_interaction.srv import url_and_course

## Scans information pertaining the server
# def ScanMainUrl():
	
# 	mainurl = raw_input("\033[1;34mWhat is the main SERVER URL?: \033[0m")
# 	if "http://" not in mainurl:
# 		mainurl = "http://"+mainurl
# 		return mainurl
# 	else:		
# 		return mainurl

# def ScanCourseCode():

# 	coursecode = raw_input("\033[1;34mWhat COURSE are we attempting?: \033[0m")
# 	return coursecode

def publishUrlandCourse(url_and_course):
	
	mainurl = url_and_course.url
	coursecode = url_and_course.course 

	mainurl_pub = rospy.Publisher('main_server_url', String, queue_size=10)
	coursecode_pub = rospy.Publisher('course_code', String, queue_size=10)

	rate = rospy.Rate(2)

	return True

	while not rospy.is_shutdown():
		
		rospy.loginfo(mainurl)	
		rospy.loginfo(coursecode)
		mainurl_pub.publish(mainurl)
		coursecode_pub.publish(coursecode)
		rate.sleep()   


def main():

	rospy.init_node('url_course_provider')
	s = rospy.Service('url_course_service', url_and_course, publishUrlandCourse)
	print "Ready to receive url and course"
	rospy.spin()

if __name__ == '__main__':
	try:
		main()
	except rospy.ROSInterruptException:
		pass	
