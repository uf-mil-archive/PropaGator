#!/usr/bin/env python

import roslib
roslib.load_manifest('spock')
import rospy
import numpy
import cv,cv2,math
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
from visualization_msgs.msg import Marker,MarkerArray

rospy.init_node('hand_detection')
bridge = CvBridge()

#-----------------------------------------------------------------------------------
#memory allocation
IMAGE_SIZE = (640,480)
hsv_image = cv.CreateImage(IMAGE_SIZE,8,3)
h_channel = cv.CreateImage(IMAGE_SIZE,8,1)
s_channel = cv.CreateImage(IMAGE_SIZE,8,1)  
v_channel = cv.CreateImage(IMAGE_SIZE,8,1)

adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
eroded = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
dilated = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

#-----------------------------------------------------------------------------------

def image_callback(data):
      
        cv_image = bridge.imgmsg_to_cv(data,"bgr8")
        cv.CvtColor(cv_image,hsv_image,cv.CV_BGR2HSV)                         # --convert from BGR to HSV
        cv.Split(hsv_image,h_channel,s_channel,v_channel,None)

        cv.AdaptiveThreshold(h_channel,adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,91,-2)
        cv.Erode(adaptive,eroded,None,1)
        cv.Dilate(eroded,dilated,None,1) 
        cv.ShowImage("test",dilated)

        contours,_ = cv2.findContours(image=numpy.asarray(dilated[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
        for i in contours:
                moments = cv.Moments(cv.fromarray(i), binary = 1)              
                area = cv.GetCentralMoment(moments, 0, 0)
                if (area > 1500):
                        cv2.drawContours(numpy.asarray(cv_image[:,:]),i,-1,(0,0,255),3) 
                

        cv.ShowImage("camera feed",cv_image)
        #cv.ShowImage("H channel",h_channel)
        #cv.ShowImage("S channel",s_channel)
        #cv.ShowImage("V channel",v_channel)
        cv.WaitKey(3)





rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)
rospy.spin()


