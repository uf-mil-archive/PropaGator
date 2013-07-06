#!/usr/bin/env python

import roslib
roslib.load_manifest('spock')
import rospy
import numpy
import cv,cv2,math,sys
import actionlib
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
from visualization_msgs.msg import Marker,MarkerArray
from ioboard.msg import IOBoardAction,IOBoardActionResult,IOBoardGoal

rospy.init_node('hand_detection')
bridge = CvBridge()

#waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
temp = actionlib.SimpleActionClient('ioboard_command', IOBoardAction)
print 'connecting to action client'
#temp.wait_for_server()
print 'connecting to shooter client'

#-----------------------------------------------------------------------------------
#memory allocation
IMAGE_SIZE = (640,480)
blurred_gray = cv.CreateImage(IMAGE_SIZE,8,1)
gray = cv.CreateImage(IMAGE_SIZE,8,1)
cv_image = cv.CreateImage(IMAGE_SIZE,8,3)
hsv = cv.CreateImage(IMAGE_SIZE,8,3)
hsv_h = cv.CreateImage(IMAGE_SIZE,8,1)
hsv_s = cv.CreateImage(IMAGE_SIZE,8,1)  
hsv_v = cv.CreateImage(IMAGE_SIZE,8,1)

lab = cv.CreateImage(IMAGE_SIZE,8,3)
lab_l = cv.CreateImage(IMAGE_SIZE,8,1)
lab_a = cv.CreateImage(IMAGE_SIZE,8,1)  
lab_b = cv.CreateImage(IMAGE_SIZE,8,1)

rgb_r = cv.CreateImage(IMAGE_SIZE,8,1)
rgb_g = cv.CreateImage(IMAGE_SIZE,8,1)  
rgb_b = cv.CreateImage(IMAGE_SIZE,8,1)

luv = cv.CreateImage(IMAGE_SIZE,8,3)
luv_l = cv.CreateImage(IMAGE_SIZE,8,1)
luv_u = cv.CreateImage(IMAGE_SIZE,8,1)  
luv_v = cv.CreateImage(IMAGE_SIZE,8,1)

xyz = cv.CreateImage(IMAGE_SIZE,8,3)
xyz_x = cv.CreateImage(IMAGE_SIZE,8,1)
xyz_y = cv.CreateImage(IMAGE_SIZE,8,1)  
xyz_z = cv.CreateImage(IMAGE_SIZE,8,1)

hls = cv.CreateImage(IMAGE_SIZE,8,3)
hls_h = cv.CreateImage(IMAGE_SIZE,8,1)
hls_l = cv.CreateImage(IMAGE_SIZE,8,1)  
hls_s = cv.CreateImage(IMAGE_SIZE,8,1)

xyz = cv.CreateImage(IMAGE_SIZE,8,3)
xyz_x = cv.CreateImage(IMAGE_SIZE,8,1)
xyz_y = cv.CreateImage(IMAGE_SIZE,8,1)  
xyz_z = cv.CreateImage(IMAGE_SIZE,8,1)

ycrcb = cv.CreateImage(IMAGE_SIZE,8,3)
ycrcb_y = cv.CreateImage(IMAGE_SIZE,8,1)
ycrcb_cr = cv.CreateImage(IMAGE_SIZE,8,1)  
ycrcb_cb = cv.CreateImage(IMAGE_SIZE,8,1)

sa = cv.CreateImage(IMAGE_SIZE,8,1)
a_not = cv.CreateImage(IMAGE_SIZE,8,1)
test = cv.CreateImage(IMAGE_SIZE,8,1)
final = cv.CreateImage(IMAGE_SIZE,8,1)

scaled_r = cv.CreateImage(IMAGE_SIZE,8,1)
scaled_g = cv.CreateImage(IMAGE_SIZE,8,1)  
scaled_b = cv.CreateImage(IMAGE_SIZE,8,1)

red_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
purple_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
red_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
red_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
purple_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
purple_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

def threshold_scissor(image):
        cv.AdaptiveThreshold(image,scissor_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY_INV,81,55)
        cv.Erode(scissor_adaptive,scissor_eroded,None,2)
        cv.Dilate(scissor_eroded,scissor_dilated,None,5)

#----------------------------------------------------------------------------------- 
def threshold_purple(image):
        cv.AdaptiveThreshold(image,purple_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY_INV,53,15)
        cv.Erode(purple_adaptive,purple_eroded_image,None,1)
        cv.Dilate(purple_adaptive,purple_dilated_image,None,8)

def threshold_red(image):
        #bright cv.AdaptiveThreshold(image,red_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,17,-30)
        cv.AdaptiveThreshold(image,red_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,17,-15)
        cv.Erode(red_adaptive,red_eroded_image,None,1)
        cv.Dilate(red_eroded_image,red_dilated_image,None,5)    

#-----------------------------------------------------------------------------------

def image_callback(data):
       
        image = bridge.imgmsg_to_cv(data,"bgr8")
              
        #normalize image
        cv.Split(image,rgb_r,rgb_g,rgb_b,None)
        red_mean = cv2.mean(numpy.asarray(rgb_r[:,:]))
        cv.Div(src2 = cv.fromarray(numpy.ones((480,640))),src1 = rgb_r,dst = scaled_r, scale = 128/red_mean[0])
        green_mean = cv2.mean(numpy.asarray(rgb_g[:,:]))
        cv.Div(src2 = cv.fromarray(numpy.ones((480,640))),src1 = rgb_g,dst = scaled_g, scale = 128/green_mean[0])
        blue_mean = cv2.mean(numpy.asarray(rgb_b[:,:]))
        cv.Div(src2 = cv.fromarray(numpy.ones((480,640))),src1 = rgb_b,dst = scaled_b, scale = 128/blue_mean[0])
        cv.Merge(scaled_r,scaled_g,scaled_b,None,cv_image)

        
        #try all the color spaces
        cv.CvtColor(cv_image,hsv,cv.CV_BGR2HSV)
        cv.CvtColor(cv_image,lab,cv.CV_BGR2Lab)
        cv.CvtColor(cv_image,luv,cv.CV_BGR2Luv)
        cv.CvtColor(cv_image,hls,cv.CV_BGR2HLS)
        cv.CvtColor(cv_image,xyz,cv.CV_BGR2XYZ)
        cv.CvtColor(cv_image,ycrcb,cv.CV_BGR2YCrCb)                       
       
        cv.Split(hsv,hsv_h,hsv_s,hsv_v,None)
        cv.Split(cv_image,rgb_r,rgb_g,rgb_b,None)
        cv.Split(lab,lab_l,lab_a,lab_b,None)
        cv.Split(luv,luv_l,luv_u,luv_v,None)
        cv.Split(hls,hls_h,hls_l,hls_s,None)
        cv.Split(xyz,xyz_x,xyz_y,xyz_x,None)
        cv.Split(ycrcb,ycrcb_y,ycrcb_cr,ycrcb_cb,None)

        cv.Not(lab_a,a_not)
        cv.Sub(hsv_s,a_not,sa)
        cv.Sub(luv_u,hls_h,test)

        '''
        cv.CvtColor(cv_image,gray,cv.CV_BGR2GRAY) 
        cv.Smooth(gray,blurred_gray,cv.CV_GAUSSIAN,3,3) 
        small = cv2.resize(numpy.asarray(ycrcb_cr[:,:]),(320,240)
        circles=cv2.HoughCircles(image=numpy.asarray(small[:,:]),method=cv.CV_HOUGH_GRADIENT,dp=1,minDist=1,param1=100,param2=60, minRadius=1,maxRadius=600)

        if not(circles is None):
                for i in circles:
                        if (i[0][1] < 200):
                                print "found circles",i,len(i)
                                center = (i[0][0],i[0][1])
                                radius = i[0][2]
                                cv.Circle(cv_image,center,radius,(1,1,0),2) 
        '''
                    
        cv.Mul(test,red_adaptive,final)      
        
        threshold_red(sa)
        #threshold_red(ycrcb_cr)     
        red_contours,_ = cv2.findContours(image=numpy.asarray(red_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE) 
        '''
        circles = extract_circles(red_contours,[1,0,0])
        for x,y,radius in circles:  
                cv.Circle(cv_image,(x,y),radius,[0,0,255],3)
        '''
        threshold_purple(hsv_s)
        purple_contours,_ = cv2.findContours(image=numpy.asarray(purple_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
        '''
        circles = extract_circles(purple_contours,[1,0,1])
        for x,y,radius in circles:  
                cv.Circle(cv_image,(x,y),radius,[255,0,255],3)
        '''
      
        '''
        cv.ShowImage("TEST",test)
        cv.ShowImage("HSV_H",hsv_h)
        cv.ShowImage("HSV_S",hsv_s)
        cv.ShowImage("HSV_V",hsv_v)
        cv.ShowImage("LAB_L",lab_l)
        cv.ShowImage("LAB_A",lab_a)
        cv.ShowImage("LAB_B",lab_b)
        cv.ShowImage("RGB_R",rgb_r)
        cv.ShowImage("RGB_G",rgb_g)
        cv.ShowImage("RGB_B",rgb_b)
        cv.ShowImage("LUV_L",luv_l)
        cv.ShowImage("LUV_U",luv_u)
        cv.ShowImage("LUV_V",luv_v)
        cv.ShowImage("HLS_H",hls_h)
        cv.ShowImage("HLS_L",hls_l)
        cv.ShowImage("HLS_S",hls_s)
        cv.ShowImage("YCrCb_Y",ycrcb_y)
        cv.ShowImage("YCrCb_Cr",ycrcb_cr)
        cv.ShowImage("YCrCb_Cb",ycrcb_cb)
        '''
     
        cv.ShowImage("normalized",cv_image)
        cv.ShowImage("original",image)
        cv.WaitKey(3)
rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)


def temp_callback(event):
        goal = IOBoardGoal(command = 'Temp')
        #temp.send_goal_wait(goal)
        #temperature = temp.get_result()
        #print temperature
rospy.Timer(rospy.Duration(.5),temp_callback)


rospy.spin()


