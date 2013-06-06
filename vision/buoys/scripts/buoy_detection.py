#!/usr/bin/env python

import roslib
roslib.load_manifest('buoys')
import rospy
import numpy as np
import cv,cv2,math
from pointcloud2xyz import *
from std_msgs.msg import String
from sensor_msgs.msg import Image,PointCloud2,PointField
from cv_bridge import CvBridge, CvBridgeError
from visualization_msgs.msg import Marker,MarkerArray
import matplotlib.pyplot as plt

rospy.init_node('buoy_detection')
bridge = CvBridge()

#-----------------------------------------------------------------------------------

#parameters to set
RED_MIN = cv.fromarray(np.array([0, 125, 0],np.uint8),allowND = True)
RED_MAX = cv.fromarray(np.array([15, 255, 60],np.uint8),allowND = True)

OBJECT_AREA = 100
IMAGE_SIZE = (640,480)

#-----------------------------------------------------------------------------------
#camera properties
intrinsic_mat = cv.CreateMat(3,3,cv.CV_32FC1)
distortion_coeffs = cv.CreateMat(1,4,cv.CV_32FC1)
rotation_vector = cv.CreateMat(1,3,cv.CV_32FC1)
translation_vector = cv.CreateMat(1,3,cv.CV_32FC1)

cv.SetZero(intrinsic_mat)
intrinsic_mat[0,0] = 972.229466052934
intrinsic_mat[0,2] = 382.460781842932
intrinsic_mat[1,1] = 972.448983311633
intrinsic_mat[1,2] = 204.387221594527
intrinsic_mat[2,2] = 1.0

distortion_coeffs[0,0] = -0.224485262763680 
distortion_coeffs[0,1] =  0.149920901726708 
distortion_coeffs[0,2] = 0.002369076650198 
distortion_coeffs[0,3] = -0.001989320823012 
      
rotation_vector[0,0] = 1.14111
rotation_vector[0,1] =  -0.98959
rotation_vector[0,2] =  1.32332 

translation_vector[0,0] = -0.060589
translation_vector[0,1] = 0.297001
translation_vector[0,2] = -1.042495 

#-----------------------------------------------------------------------------------

#memory allocation
hsv_image = cv.CreateImage(IMAGE_SIZE,8,3)
blurred_image = cv.CreateImage(IMAGE_SIZE,8,3)

h_channel = cv.CreateImage(IMAGE_SIZE,8,1)
s_channel = cv.CreateImage(IMAGE_SIZE,8,1)  
v_channel = cv.CreateImage(IMAGE_SIZE,8,1)
s_inv = cv.CreateImage(IMAGE_SIZE,8,1)
h_inv = cv.CreateImage(IMAGE_SIZE,8,1)
h_s_not = cv.CreateImage(IMAGE_SIZE,8,1)
h_not_s = cv.CreateImage(IMAGE_SIZE,8,1)
s_v = cv.CreateImage(IMAGE_SIZE,8,1)
blurred_bgr_image = cv.CreateImage(IMAGE_SIZE,8,3)

red_threshold_image = cv.CreateImage(IMAGE_SIZE,8,1) 
green_threshold_image = cv.CreateImage(IMAGE_SIZE,8,1)
yellow_threshold_image = cv.CreateImage(IMAGE_SIZE,8,1)

red_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
yellow_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
green_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
red_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
red_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
green_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
green_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
yellow_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
yellow_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

global cloudx,cloudy,cloud
cloud = []
cloudx = []
cloudy = []

#-----------------------------------------------------------------------------------

#publish marker array of objects found
global bouy_array
bouy_publisher=rospy.Publisher('buoy_markers',MarkerArray)
bouy_array=MarkerArray()
def append_marker(pos,color):
	marker = Marker()
	marker.header.frame_id = "/bluefox"
	marker.type = marker.SPHERE
	marker.id = pos[0]*pos[1]
	marker.lifetime = rospy.Duration(2      )
	marker.action = marker.ADD
	marker.scale.x = 0.5
	marker.scale.y = 0.5
	marker.scale.z = 0.5
	marker.color.a = 1.0
	marker.color.r = color[0]
	marker.color.g = color[1]
	marker.color.b = color[2]
	marker.pose.orientation.w = 1.0
	marker.pose.position.x = pos[2]
	marker.pose.position.y = pos[1]
	marker.pose.position.z = pos[0]
	bouy_array.markers.append(marker)

#-----------------------------------------------------------------------------------

#--print HSV of pixel clicked on in image (used to find threshold values)
def mouse_callback(event,x,y,flags,image):

        if event==cv.CV_EVENT_LBUTTONDOWN:
                print cv.Get2D(image,y,x)  
#----------------------------------------------------------------------------------

def distance(p0, p1):
    return math.sqrt((p0[0] - p1[0])**2 + (p0[1] - p1[1])**2)

def check_lidar((x,y)):
        error = 200
        coi = False
        index = 0
        for i,j in zip(cloudx,cloudy):        
                dist = distance((i,j),(x,y))
                if (dist < 10 and dist < error):          
                        coi = index
                        error = dist
                index = index + 1

        if (coi):        
                return (cloud[coi])
        else:
                return [1000,0,0]
              
#-----------------------------------------------------------------------------------

def image_callback(data):
      
        cv_image = bridge.imgmsg_to_cv(data,"bgr8")
        cv.CvtColor(cv_image,hsv_image,cv.CV_BGR2HSV)                         # --convert from BGR to HSV

        cv.Smooth(cv_image,blurred_bgr_image,cv.CV_GAUSSIAN,9,9)  
        cv.Smooth(hsv_image,blurred_image,cv.CV_GAUSSIAN,5,5)                 # --three parameters are
                                                                              # (aperture_width,aperture_height,gaussian std_dev)
                                                                              # must  be an odd number, std_dev of 0 means that it is
                                                                              # calculated from the kernel size 
                                                                            
 
        cv.InRange(blurred_image,RED_MIN,RED_MAX,red_threshold_image)         #--threshold color based on HSV range
       
        cv.Split(hsv_image,h_channel,s_channel,v_channel,None)                #split HSV image into three seperate images
        
        cv.Not(s_channel,s_inv)
        cv.Not(h_channel,h_inv)
        cv.Mul(h_channel,s_inv,h_s_not)
        cv.Mul(h_inv,s_channel,h_not_s)
        cv.Mul(s_channel,v_channel,s_v)

        cv.AdaptiveThreshold(h_s_not,red_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY_INV,53,5) 
        cv.AdaptiveThreshold(s_v,yellow_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,303,-15)
        cv.AdaptiveThreshold(s_channel,green_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,303,-60)
       
        #cv.ShowImage("adaptive",green_adaptive)
        
        cv.Erode(red_adaptive,red_eroded_image,None,15)                        #erode and dilate the thresholded images
        cv.Erode(green_adaptive,green_eroded_image,None,9)
        cv.Erode(yellow_adaptive,yellow_eroded_image,None,5)
        cv.Dilate(red_eroded_image,red_dilated_image,None,5)
        cv.Dilate(green_eroded_image,green_dilated_image,None,17)
        cv.Dilate(yellow_eroded_image,yellow_dilated_image,None,7) 
      
        red_contours,_ = cv2.findContours(image=np.asarray(red_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
        green_contours,_ = cv2.findContours(image=np.asarray(green_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
        yellow_contours,_ = cv2.findContours(image=np.asarray(yellow_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
        #cv2.drawContours(np.asarray(cv_image[:,:]),red_contours,-1,(0,0,255),3)   
     
      
     
       
        if (green_contours):
                for i in green_contours:
                        #ellipse = cv2.fitEllipse(i)
                        #cv2.ellipse(np.asarray(cv_image[:,:]),ellipse,(255,0,0),2)

                        moments = cv.Moments(cv.fromarray(i), binary = 1)             
                        area = cv.GetCentralMoment(moments, 0, 0)

                        if area > OBJECT_AREA:
                                x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                                y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                                radius = int(math.sqrt(area/math.pi))
                                cv.Circle(cv_image,(x,y),radius,(0,255,0),3)

                                point = check_lidar((x,y))
                                if (point[0] != 1000):
                                        print point,"!"
                                        append_marker(point,(0,1.0,0))    
           
        if (len(cloudx) > 0):
                for i,j in zip(cloudx,cloudy):
                        if (i < 700 and j < 700 and i > 0 and j > 0):
                                cv.Circle(cv_image,(int(i),int(j)),1,(0,255,0),3)
        
        ''' 
        if (red_contours):
                for i in red_contours:
                        moments = cv.Moments(cv.fromarray(i), binary = 1)              
                        area = cv.GetCentralMoment(moments, 0, 0)
                        if area > OBJECT_AREA:
                                x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                                y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                                radius = int(math.sqrt(area/math.pi))
                                cv.Circle(cv_image,(x,y),radius,(0,0,255),3)
                                #append_marker((x,y),(1.0,0,0))     
        if (yellow_contours):
                for i in yellow_contours:
                        moments = cv.Moments(cv.fromarray(i), binary = 1)             
                        area = cv.GetCentralMoment(moments, 0, 0)
                        if area > OBJECT_AREA:
                                x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                                y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                                radius = int(math.sqrt(area/math.pi))
                                cv.Circle(cv_image,(x,y),radius,(0,255,255),3)
                                #append_marker((x,y),(0,1.0,0))
        '''
         
                 
        cv.SetMouseCallback("camera feed",mouse_callback,hsv_image)   
        #cv.ShowImage("H channel",h_channel)
        #cv.ShowImage("S channel",s_channel)
        #cv.ShowImage("V channel",v_channel)
        cv.ShowImage("camera feed",cv_image)
       
        cv.WaitKey(3)

#-----------------------------------------------------------------------------------

def bouy_callback(event):
    global bouy_array
    bouy_publisher.publish(bouy_array)

#-----------------------------------------------------------------------------------

def pointcloud_callback(msg):
        global cloudx,cloudy,cloud
        cloud = pointcloud2_to_xyz_array(msg)
        cloud_mat = cv.CreateMat(len(cloud),1,cv.CV_32FC3)
        projection = cv.CreateMat(len(cloud),1,cv.CV_32FC2)
        cloud_mat = cloud
     
        cv.ProjectPoints2(cv.fromarray(cloud_mat),rotation_vector,translation_vector,intrinsic_mat,distortion_coeffs,projection)
        (cloudx,cloudy) = cv2.split(np.asarray(projection))
        
#-----------------------------------------------------------------------------------

rospy.Timer(rospy.Duration(.1), bouy_callback)
rospy.Subscriber("/cloud_3d",PointCloud2,pointcloud_callback)
rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)
rospy.spin()






