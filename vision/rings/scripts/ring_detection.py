#!/usr/bin/env python

import roslib
roslib.load_manifest('rings')
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
#cv.NamedWindow("purple threshold",1)

#-----------------------------------------------------------------------------------

#parameters to set
RED_MIN = cv.fromarray(numpy.array([0, 125, 0],numpy.uint8),allowND = True)
RED_MAX = cv.fromarray(numpy.array([15, 255, 60],numpy.uint8),allowND = True)

purple_MIN = cv.fromarray(numpy.array([30, 160, 50],numpy.uint8),allowND = True)
purple_MAX = cv.fromarray(numpy.array([75, 210, 130],numpy.uint8),allowND = True)
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
purple_threshold_image = cv.CreateImage(IMAGE_SIZE,8,1)

red_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
purple_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
red_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
red_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
purple_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
purple_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

#-----------------------------------------------------------------------------------

#publish marker array of objects found
global red_ring_pos
global purple_ring_pos
global purple_ring_pos

red_ring_pos = (0,0)
purple_ring_pos = (0,0)
purple_ring_pos = (0,0)

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
        cv.InRange(blurred_image,purple_MIN,purple_MAX,purple_threshold_image)  
       
        cv.Split(hsv_image,h_channel,s_channel,v_channel,None)                #split HSV image into three seperate images
        
        
        cv.AdaptiveThreshold(s_channel,red_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,33,-26)      #use hue channel to filter for red
        cv.AdaptiveThreshold(s_channel,purple_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY_INV,53,26)
       
        cv.Erode(red_adaptive,red_eroded_image,None,1)                        #erode and dilate the thresholded images
        cv.Erode(purple_threshold_image,purple_eroded_image,None,15)
        cv.Dilate(red_eroded_image,red_dilated_image,None,10)
        cv.Dilate(purple_eroded_image,purple_dilated_image,None,9)

        cv.ShowImage("red threshold",red_dilated_image)                       #show image here because findContours effects memory location
        
        red_contours,_ = cv2.findContours(image=numpy.asarray(red_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
        purple_contours,_ = cv2.findContours(image=numpy.asarray(purple_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
        #cv2.drawContours(numpy.asarray(cv_image[:,:]),red_contours,-1,(0,0,255),3) 
        #cv2.drawContours(numpy.asarray(cv_image[:,:]),purple_contours,-1,(0,255,0),3)     
        
        if (red_contours):
                for i in red_contours:
                        moments = cv.Moments(cv.fromarray(i), binary = 1)              
                        area = cv.GetCentralMoment(moments, 0, 0)
                        if area > OBJECT_AREA:
                                x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                                y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                                radius = int(math.sqrt(area/math.pi))
                                cv.Circle(cv_image,(x,y),radius,(0,0,255),3)
                                red_ring_pos = (x,y)
                                
        if (purple_contours):
                for i in purple_contours:
                        moments = cv.Moments(cv.fromarray(i), binary = 1)             
                        area = cv.GetCentralMoment(moments, 0, 0)
                        if area > OBJECT_AREA:
                                x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                                y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                                radius = int(math.sqrt(area/math.pi))
                                cv.Circle(cv_image,(x,y),radius,(0,255,0),3)
                                purple_ring_pos = (x,y)

                      
                        
         


                 
        cv.SetMouseCallback("camera feed",mouse_callback,hsv_image)   
        #cv.ShowImage("test",dist_image)             
        #cv.ShowImage("H channel",h_channel)
        #cv.ShowImage("S channel",s_channel)
        #cv.ShowImage("V channel",v_channel)
        cv.ShowImage("camera feed",cv_image)
       
        cv.WaitKey(3)


#-----------------------------------------------------------------------------------
rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)
rospy.spin()






