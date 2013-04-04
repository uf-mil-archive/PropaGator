#!/usr/bin/env python

import roslib
roslib.load_manifest('sandbox_opencv')
import rospy
import numpy
import cv,cv2,math
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
from visualization_msgs.msg import Marker,MarkerArray

rospy.init_node('ring_detection')
bridge = CvBridge()

#-----------------------------------------------------------------------------------

cv.NamedWindow("camera feed",1)
#cv.NamedWindow("H channel",1)
#cv.NamedWindow("S channel",1)
#cv.NamedWindow("V channel",1)
cv.NamedWindow("red threshold",1)
#cv.NamedWindow("green threshold",1)

#-----------------------------------------------------------------------------------

#parameters to set
RED_MIN = cv.fromarray(numpy.array([0, 125, 0],numpy.uint8),allowND = True)
RED_MAX = cv.fromarray(numpy.array([15, 255, 60],numpy.uint8),allowND = True)

GREEN_MIN = cv.fromarray(numpy.array([30, 160, 50],numpy.uint8),allowND = True)
GREEN_MAX = cv.fromarray(numpy.array([75, 210, 130],numpy.uint8),allowND = True)
OBJECT_AREA = 10000
IMAGE_SIZE = (640,480)

#-----------------------------------------------------------------------------------

#memory allocation
hsv_image = cv.CreateImage(IMAGE_SIZE,8,3)
blurred_image = cv.CreateImage(IMAGE_SIZE,8,3)

h_channel = cv.CreateImage(IMAGE_SIZE,8,1)
s_channel = cv.CreateImage(IMAGE_SIZE,8,1)  
v_channel = cv.CreateImage(IMAGE_SIZE,8,1)
blurred_bgr_image = cv.CreateImage(IMAGE_SIZE,8,3)

red_threshold_image = cv.CreateImage(IMAGE_SIZE,8,1) 
green_threshold_image = cv.CreateImage(IMAGE_SIZE,8,1)

red_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
green_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
red_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
red_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
green_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
green_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

#-----------------------------------------------------------------------------------

#publish marker array of objects found
global bouy_array
bouy_publisher=rospy.Publisher('buoys',MarkerArray)
bouy_array=MarkerArray()
def append_marker(pos,color):
	marker = Marker()
	marker.header.frame_id = "/camera_frame"
	marker.type = marker.SPHERE
	marker.id = pos[0]*pos[1]
	marker.lifetime = rospy.Duration(.5)
	marker.action = marker.ADD
	marker.scale.x = 0.5
	marker.scale.y = 0.5
	marker.scale.z = 0.5
	marker.color.a = 1.0
	marker.color.r = color[0]
	marker.color.g = color[1]
	marker.color.b = color[2]
	marker.pose.orientation.w = 1.0
	marker.pose.position.x = pos[0]
	marker.pose.position.y = pos[1]
	marker.pose.position.z = 0
	bouy_array.markers.append(marker)

#-----------------------------------------------------------------------------------

#--print HSV of pixel clicked on in image (used to find threshold values)
def mouse_callback(event,x,y,flags,image):

        if event==cv.CV_EVENT_LBUTTONDOWN:
                print cv.Get2D(image,y,x)                                     

#-----------------------------------------------------------------------------------

def image_callback(data):
      
        cv_image = bridge.imgmsg_to_cv(data,"bgr8")
        cv.CvtColor(cv_image,hsv_image,cv.CV_BGR2HSV)                         # --convert from BGR to HSV

        cv.Smooth(cv_image,blurred_bgr_image,cv.CV_GAUSSIAN,9,9)  
        cv.Smooth(hsv_image,blurred_image,cv.CV_GAUSSIAN,5,5)                 # --three parameters are (aperture_width,aperture_height,gaussian std_dev)
                                                                              # must  be an odd number, std_dev of 0 means that it is calculated from 
                                                                              # the kernel size
 
        cv.InRange(blurred_image,RED_MIN,RED_MAX,red_threshold_image)         #--threshold color based on HSV range
        cv.InRange(blurred_image,GREEN_MIN,GREEN_MAX,green_threshold_image)  
       
        cv.Split(hsv_image,h_channel,s_channel,v_channel,None)                #split HSV image into three seperate images
        
        
        cv.AdaptiveThreshold(h_channel,red_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY_INV,53,5)      #use hue channel to filter for red

       
        cv.Erode(red_adaptive,red_eroded_image,None,1)                        #erode and dilate the thresholded images
        cv.Erode(green_threshold_image,green_eroded_image,None,1)
        cv.Dilate(red_eroded_image,red_dilated_image,None,5)
        cv.Dilate(green_eroded_image,green_dilated_image,None,5)

        cv.ShowImage("red threshold",red_dilated_image)                       #show image here because findContours effects memory location
        
        red_contours,_ = cv2.findContours(image=numpy.asarray(red_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
        green_contours,_ = cv2.findContours(image=numpy.asarray(green_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
        #cv2.drawContours(numpy.asarray(cv_image[:,:]),red_contours,-1,(0,0,255),3) 
        #cv2.drawContours(numpy.asarray(cv_image[:,:]),green_contours,-1,(0,255,0),3)     
        
        if (red_contours):
                for i in red_contours:
                        moments = cv.Moments(cv.fromarray(i), binary = 1)              
                        area = cv.GetCentralMoment(moments, 0, 0)
                        if area > OBJECT_AREA:
                                x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                                y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                                radius = int(math.sqrt(area/math.pi))
                                cv.Circle(cv_image,(x,y),radius,(0,0,255),3)
                                append_marker((x,y),(1.0,0,0))
        if (green_contours):
                for i in green_contours:
                        moments = cv.Moments(cv.fromarray(i), binary = 1)             
                        area = cv.GetCentralMoment(moments, 0, 0)
                        if area > OBJECT_AREA:
                                x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                                y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                                radius = int(math.sqrt(area/math.pi))
                                cv.Circle(cv_image,(x,y),radius,(0,255,0),3)
                                append_marker((x,y),(0,1.0,0))

                      
                        
         
                 
        cv.SetMouseCallback("camera feed",mouse_callback,hsv_image)   
        #cv.ShowImage("test",dist_image)             
        cv.ShowImage("H channel",red_adaptive)
        #cv.ShowImage("S channel",s_channel)
        #cv.ShowImage("V channel",v_channel)
        cv.ShowImage("camera feed",cv_image)
       
        cv.WaitKey(3)

#-----------------------------------------------------------------------------------

def bouy_callback(event):
    global bouy_array
    bouy_publisher.publish(bouy_array)

#-----------------------------------------------------------------------------------

rospy.Timer(rospy.Duration(.1), bouy_callback)
rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)
rospy.spin()






