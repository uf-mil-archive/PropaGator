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
wrench_publisher = rospy.Publisher('wrench',WrenchStamped)

#-----------------------------------------------------------------------------------

#parameters to set
CENTER = True
global error,previous_error,desired_state,p,d
error = [0,0]
previous_error = [0,0]
desired_state = [320,140]
p = 1
d = 0

RED_MIN = cv.fromarray(numpy.array([0, 125, 0],numpy.uint8),allowND = True)
RED_MAX = cv.fromarray(numpy.array([15, 255, 60],numpy.uint8),allowND = True)

ORANGE_MIN = cv.fromarray(numpy.array([30, 160, 50],numpy.uint8),allowND = True)
ORANGE_MAX = cv.fromarray(numpy.array([75, 210, 130],numpy.uint8),allowND = True)
OBJECT_AREA = 1000
IMAGE_SIZE = (640,480)

#-----------------------------------------------------------------------------------

#memory allocation
hsv_image = cv.CreateImage(IMAGE_SIZE,8,3)
blurred_image = cv.CreateImage(IMAGE_SIZE,8,3)

h_channel = cv.CreateImage(IMAGE_SIZE,8,1)
s_channel = cv.CreateImage(IMAGE_SIZE,8,1)  
v_channel = cv.CreateImage(IMAGE_SIZE,8,1)
blurred_bgr_image = cv.CreateImage(IMAGE_SIZE,8,3)

s_not = cv.CreateImage(IMAGE_SIZE,8,1) 
h_not = cv.CreateImage(IMAGE_SIZE,8,1) 
h_s_not = cv.CreateImage(IMAGE_SIZE,8,1) 
h_not_s = cv.CreateImage(IMAGE_SIZE,8,1) 

red_threshold_image = cv.CreateImage(IMAGE_SIZE,8,1) 
purple_threshold_image = cv.CreateImage(IMAGE_SIZE,8,1)
orange_threshold_image = cv.CreateImage(IMAGE_SIZE,8,1)

red_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
purple_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
orange_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)

red_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
red_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

orange_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
orange_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

purple_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
purple_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

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
        cv.InRange(blurred_image,ORANGE_MIN,ORANGE_MAX,purple_threshold_image)  
       
        cv.Split(hsv_image,h_channel,s_channel,v_channel,None)                #split HSV image into three seperate images
        
        cv.Not(h_channel,h_not)
        cv.Mul(h_not,s_channel,h_not_s)
        
        
        #red
        cv.AdaptiveThreshold(s_channel,red_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,103,-55)      
        cv.Erode(red_adaptive,red_eroded_image,None,2)                        
        cv.Dilate(red_eroded_image,red_dilated_image,None,9)
        #purple
        cv.AdaptiveThreshold(h_channel,purple_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY_INV,103,25)     
        cv.Erode(purple_adaptive,purple_eroded_image,None,3)
        cv.Dilate(purple_eroded_image,purple_dilated_image,None,9)
        

        cv.ShowImage("H channel",h_channel)
        cv.ShowImage("S channel",s_channel)
        cv.ShowImage("V channel",v_channel)
        cv.ShowImage("red",red_dilated_image)                             
        cv.ShowImage("purple",purple_dilated_image)                       #show image here because findContours affects memory location
        
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
                                red_center = [x,y]
                                radius = int(math.sqrt(area/math.pi))
                                cv.Circle(cv_image,(x,y),radius,(0,0,255),3)
                                append_marker((x,y),(1.0,0,0))
        if (purple_contours):
                for i in purple_contours:
                        moments = cv.Moments(cv.fromarray(i), binary = 1)             
                        area = cv.GetCentralMoment(moments, 0, 0)
                        if area > OBJECT_AREA:
                                x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                                y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                                radius = int(math.sqrt(area/math.pi))
                                cv.Circle(cv_image,(x,y),radius,(128,0,128),3)
                                append_marker((x,y),(0,1.0,0))

        if (CENTER == True):
                error = desired_state - red_center
                p_term = p*error
                d_term = d*(error-previous_error)
                output = p_term + d_term
                
                wrench_publisher.publish(WrenchStamped(
						header = Header(
							stamp=rospy.Time.now(),
							frame_id="/base_link",
							),
						wrench=Wrench(
							force = Vector3(x=output[0],y= output[1],z= 0),
							torque = Vector3(x=0,y= 0,z= 0),
							))
							)	

                
                 
        cv.SetMouseCallback("camera feed",mouse_callback,hsv_image)   
      
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






