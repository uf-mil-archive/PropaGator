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
s_not = cv.CreateImage(IMAGE_SIZE,8,1)

final = cv.CreateImage(IMAGE_SIZE,8,1)
test = cv.CreateImage(IMAGE_SIZE,8,1)

scaled_r = cv.CreateImage(IMAGE_SIZE,8,1)
scaled_g = cv.CreateImage(IMAGE_SIZE,8,1)  
scaled_b = cv.CreateImage(IMAGE_SIZE,8,1)

scissors = cv.CreateImage(IMAGE_SIZE,8,1)
lizard = cv.CreateImage(IMAGE_SIZE,8,1)
rock = cv.CreateImage(IMAGE_SIZE,8,1)
spock = cv.CreateImage(IMAGE_SIZE,8,1)
paper = cv.CreateImage(IMAGE_SIZE,8,1)

scissors_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
scissors_eroded = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
scissors_dilated = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

spock_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
spock_eroded = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
spock_dilated = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

rock_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
rock_eroded = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
rock_dilated = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

paper_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
paper_eroded = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
paper_dilated = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

lizard_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
lizard_eroded = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
lizard_dilated = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

purple_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
purple_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
purple_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)



#----------------------------------------------------------------------------------- 
def threshold_paper(image):
        cv.AdaptiveThreshold(image,paper_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,53,-6)
        cv.Mul(paper_adaptive,hls_s,paper)
        cv.Erode(paper_adaptive,paper_eroded,None,1)
        cv.Dilate(paper_eroded,paper_dilated,None,4)

def threshold_lizard(image):
        cv.AdaptiveThreshold(image,lizard_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,53,-20)#16
        cv.Erode(lizard_adaptive,lizard_eroded,None,1)
        cv.Dilate(lizard_eroded,lizard_dilated,None,4)

def threshold_spock(image):
        cv.AdaptiveThreshold(image,spock_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY_INV,103,25)
        cv.Mul(spock_adaptive,hls_h,spock)
        cv.Erode(spock,spock_eroded,None,2)
        cv.Dilate(spock_eroded,spock_dilated,None,6)

def threshold_scissors(image):
        cv.AdaptiveThreshold(image,scissors_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,203,-116)
        cv.Erode(scissors_adaptive,scissors_eroded,None,3)
        cv.Dilate(scissors_eroded,scissors_dilated,None,6)

def threshold_rock(image):
        cv.AdaptiveThreshold(image,rock_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,203,-116)
        cv.Erode(rock_adaptive,rock_eroded,None,3)
        cv.Dilate(rock_eroded,rock_dilated,None,6)
 
 
def find_squares(contours,sign):
        font = cv.InitFont(cv.CV_FONT_HERSHEY_SIMPLEX,1,1,0,1,1)
        max_area = 1000
        for i in contours:    
                x,y,w,h = cv2.boundingRect(i)
                if (w*h > max_area and math.fabs(w-h) < 5 and y < 240):
                        best = [x,y,w,h]
        try:
                cv.PutText(cv_image,sign,(best[0],best[1]),font,[0,0,255])
                cv.Rectangle(cv_image,(best[0],best[1]),(best[0]+best[2],best[1]+best[3]),[0,0,255])
        except UnboundLocalError:
                print sign," not found"

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

        #cv.Not(lab_a,a_not)
        #cv.Sub(hsv_s,a_not,sa)
        cv.Not(hsv_s,s_not)
        
        #cv.Sub(hsv_s,hsv_h,rock)
        #cv.Sub(rock,hls_h,test)        #MAYBE LIZARD

        cv.Sub(ycrcb_cr,hsv_h,rock)

        cv.Sub(luv_u,hls_h,scissors)
        cv.Sub(luv_v,luv_u,lizard)
                     
       
        threshold_scissors(hls_h)
        threshold_lizard(lizard)
        threshold_spock(hls_s)
        threshold_paper(hls_s)
        threshold_rock(hls_s)
        
        '''
        cv.ShowImage("paper",paper_dilated)
        cv.ShowImage("lizard",lizard_dilated)
        cv.ShowImage("scissors",scissors_dilated)      
        '''
        cv.ShowImage("spock",spock_dilated)
        cv.ShowImage("rock",rock_dilated)

        #threshold_red(ycrcb_cr)     
        scissors_contours,_ = cv2.findContours(image=numpy.asarray(scissors_dilated[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE) 
        lizard_contours,_ = cv2.findContours(image=numpy.asarray(lizard_dilated[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
        spock_contours,_ = cv2.findContours(image=numpy.asarray(spock_dilated[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
        rock_contours,_ = cv2.findContours(image=numpy.asarray(rock_dilated[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE) 
        paper_contours,_ = cv2.findContours(image=numpy.asarray(paper_dilated[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE) 
       
        
        find_squares(scissors_contours,"scissors")
        find_squares(lizard_contours,"lizard")
        find_squares(spock_contours,"spock")
  
        cv.ShowImage("HSV_H",hsv_h)
        cv.ShowImage("HSV_S",hsv_s)
        cv.ShowImage("HSV_V",hsv_v)
        cv.ShowImage("LAB_L",lab_l)
        cv.ShowImage("LAB_A",lab_a)
        cv.ShowImage("LAB_B",lab_b)
        cv.ShowImage("RGB_R",scaled_r)
        cv.ShowImage("RGB_G",scaled_g)
        cv.ShowImage("RGB_B",scaled_b)
        cv.ShowImage("LUV_L",luv_l)
        cv.ShowImage("LUV_U",luv_u)
        cv.ShowImage("LUV_V",luv_v)
        cv.ShowImage("HLS_H",hls_h)
        cv.ShowImage("HLS_L",hls_l)
        cv.ShowImage("HLS_S",hls_s)
        cv.ShowImage("YCrCb_Y",ycrcb_y)
        cv.ShowImage("YCrCb_Cr",ycrcb_cr)
        cv.ShowImage("YCrCb_Cb",ycrcb_cb)
       
        cv.ShowImage("normalized",cv_image)
        cv.WaitKey(3)
rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)


def temp_callback(event):
        goal = IOBoardGoal(command = 'Temp')
        #temp.send_goal_wait(goal)
        #temperature = temp.get_result()
        #print temperature
rospy.Timer(rospy.Duration(.5),temp_callback)


rospy.spin()


