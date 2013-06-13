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
import actionlib
from geometry_msgs.msg import Point
from nav_msgs.msg import Odometry
from rings.msg import ShootRingsAction,ShootRingsActionFeedback

rospy.init_node('ring_detection')
bridge = CvBridge()

carrot = rospy.Publisher("/carrot",Point)

global running,current_position,shots,orientation,colors,color_index
running = False
current_position = []
orientation = []
shots = 0
colors = ['red','green','purple']
color_index = 0

#-----------------------------------------------------------------------------------

purple_MIN = cv.fromarray(numpy.array([30, 160, 50],numpy.uint8),allowND = True)
purple_MAX = cv.fromarray(numpy.array([75, 210, 130],numpy.uint8),allowND = True)
OBJECT_AREA = 15000
IMAGE_SIZE = (640,480)

#-----------------------------------------------------------------------------------roscd
purple_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
red_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
red_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
purple_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
purple_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

#-----------------------------------------------------------------------------------

#--print HSV of pixel clicked on in image (used to find threshold values)
def mouse_callback(event,x,y,flags,image):

        if event==cv.CV_EVENT_LBUTTONDOWN:
                print cv.Get2D(image,y,x)                                     

#-----------------------------------------------------------------------------------
gain = .1
red_shoot = (320,240)
def distance (p1,p2):
	return (math.sqrt((p2[1]-p1[1])**2 + (p2[0]-p1[0])**2))
def adjust_carrot(x,y):
        err = distance((x,y),red_shoot)
        if (err > 10):
                carrot.publish(Point(x = gain*(x - red_shoot[0]),y = gain*(y - red_shoot[1]),z = 0))
        else:
                print "shooting!"
                color_index = color_index + 1
                shots = shots + 1      

def image_callback(data):
        global running,colors
        if (running):
                #print "running"
                cv_image = bridge.imgmsg_to_cv(data,"bgr8")
                cv.CvtColor(cv_image,hsv_image,cv.CV_BGR2HSV)                         # --convert from BGR to HSV

                cv.Smooth(hsv_image,blurred_image,cv.CV_GAUSSIAN,5,5)                 # --three parameters are (aperture_width,aperture_height,gaussian std_dev)
                                                                                      # must  be an odd number, std_dev of 0 means that it is calculated from 
                                                                                      # the kernel size
               
                cv.Split(hsv_image,h_channel,s_channel,v_channel,None)                #split HSV image into three seperate images
                
                #cv.Not(h_channel,h_not)
                 cv.AdaptiveThreshold(s_channel,red_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,53,-26)      #use hue channel to filter for red
        #cv.AdaptiveThreshold(s_channel,purple_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY_INV,53,20)
                cv.AdaptiveThreshold(h_s,purple_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,21,-2)
                cv.ShowImage("red threshold",red_adaptive)
               
                cv.Erode(red_adaptive,red_eroded_image,None,2)                        #erode and dilate the thresholded images
                cv.Erode(purple_adaptive,purple_eroded_image,None,1)
                cv.Dilate(red_eroded_image,red_dilated_image,None,10)
                cv.Dilate(purple_adaptive,purple_dilated_image,None,8)


                cv.ShowImage("red threshold",red_dilated_image)                       #show image here because findContours effects memory location
                
                red_contours,_ = cv2.findContours(image=numpy.asarray(red_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
                purple_contours,_ = cv2.findContours(image=numpy.asarray(purple_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
                #cv2.drawContours(numpy.asarray(cv_image[:,:]),red_contours,-1,(0,0,255),3) 
                #cv2.drawContours(numpy.asarray(cv_image[:,:]),purple_contours,-1,(0,255,0),3)     
                
                if (red_contours ):
                        for i in red_contours:
                                moments = cv.Moments(cv.fromarray(i), binary = 1)              
                                area = cv.GetCentralMoment(moments, 0, 0)
                                if area > OBJECT_AREA:
                                        x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                                        y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                                        radius = int(math.sqrt(area/math.pi))
                                        cv.Circle(cv_image,(x,y),radius,(0,0,255),3)
                                        if (colors[color_index] == 'red'):
                                                adjust_carrot(x,y)
                                        
                if (purple_contours):
                        for i in purple_contours:
                                moments = cv.Moments(cv.fromarray(i), binary = 1)             
                                area = cv.GetCentralMoment(moments, 0, 0)
                                if area > OBJECT_AREA:
                                        x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                                        y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                                        radius = int(math.sqrt(area/math.pi))
                                        cv.Circle(cv_image,(x,y),radius,(0,255,0),3)
                                        if (colors[color_index] == 'purple'):
                                                purple_ring_pos = (x,y)

                              
                cv.SetMouseCallback("camera feed",mouse_callback,hsv_image)   
                #cv.ShowImage("H channel",h_channel)
                #cv.ShowImage("S channel",s_channel)
                #cv.ShowImage("V channel",v_channel)
                cv.ShowImage("camera feed",cv_image)
               
                cv.WaitKey(3)

#-----------------------------------------------------------------------------------

class ShootRingsServer:

 def __init__(self):
        global running
        self.server = actionlib.SimpleActionServer('shoot_rings', ShootRingsAction, self.execute, False)
        rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)
        self.server.start()
        self._feedback = ShootRingsActionFeedback

 def execute(self,goal):
        global shots,running
        while(shots < goal.attempts):
                running = True
                self._feedback.darts_shot = shots
                self.server.publish_feedback(self._feedback)
                #shots = shots + 1
                #rospy.sleep(1)

        running = False
        shots = 0
        self.server.set_succeeded()

server = ShootRingsServer()
rospy.spin()






