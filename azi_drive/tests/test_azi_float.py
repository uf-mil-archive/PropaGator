#!/usr/bin/env python

 
import sys
import rospy
import roslib
roslib.load_manifest('azi_drive')
from azi_drive.srv import *
 
def change_float_client(mode):
    rospy.wait_for_service('/float_mode')
    try:
        float_mode = rospy.ServiceProxy('/float_mode', AziFloat)
        resp1 = float_mode(mode)
        return resp1.out
	print resp1.out
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

if __name__ == "__main__":
    if len(sys.argv) == 2:
        x = str(sys.argv[1])
	resp = bool([])
	if x == "True" or x == "true" or x == "1":
		resp = True
	elif x == "False" or x == "false" or x == "0":
		resp = False
    else:
        sys.exit(1)
    print (resp, change_float_client(resp))
