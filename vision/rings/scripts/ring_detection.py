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
from c3_trajectory_generator.msg import MoveToAction, MoveToGoal
from uf_common.orientation_helpers import lookat, get_perpendicular
from nav_msgs.msg import Odometry
from rings.msg import ShootRingsAction,ShootRingsActionFeedback

rospy.init_node('ring_detection')
bridge = CvBridge()

global running,current_position,shots,orientation,colors,color_index
running = False
current_position = []
orientation = []
shots = 0
colors = ['red','green','purple']
color_index = 0

#-----------------------------------------------------------------------------------

waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
print 'connecting to action client'
#waypoint.wait_for_server()

def send_waypoint(point,orientation):
	waypoint.send_goal(MoveToGoal(
			header=Header(
				frame_id='/simmap',
			),
			pose=Pose(
				position=Point(
					x=point[0],
					y=point[1],
					z=0,
				),
				orientation=Quaternion(*orientation),
			),
	))

def distance (p1,p2):
	return (math.sqrt((p2[1]-p1[1])**2 + (p2[0]-p1[0])**2))
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

#--print HSV of pixel clicked on in image (used to find threshold values)
def mouse_callback(event,x,y,flags,image):

        if event==cv.CV_EVENT_LBUTTONDOWN:
                print cv.Get2D(image,y,x)                                     

#-----------------------------------------------------------------------------------
def adjust_carrot(x,y):
        err = distance((x,y),current_position)
        offset = err*.5
        if (offset > .1):
                send_waypoint((offset + current_position[0],offset - current_position[1]),orientation)
        else:
                print "shooting!"
                color_index = color_index + 1
                shots = shots + 1      

def image_callback(data):
        if (running):
                print "running"
                cv_image = bridge.imgmsg_to_cv(data,"bgr8")
                cv.CvtColor(cv_image,hsv_image,cv.CV_BGR2HSV)                         # --convert from BGR to HSV

                cv.Smooth(hsv_image,blurred_image,cv.CV_GAUSSIAN,5,5)                 # --three parameters are (aperture_width,aperture_height,gaussian std_dev)
                                                                                      # must  be an odd number, std_dev of 0 means that it is calculated from 
                                                                                      # the kernel size
               
                cv.Split(hsv_image,h_channel,s_channel,v_channel,None)                #split HSV image into three seperate images
                
                
                cv.AdaptiveThreshold(s_channel,red_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,33,-26)      #use hue channel to filter for red
                cv.AdaptiveThreshold(s_channel,purple_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY_INV,53,26)
               
                cv.Erode(red_adaptive,red_eroded_image,None,1)                        #erode and dilate the thresholded images
                cv.Erode(purple_threshold_image,purple_eroded_image,None,15)
                cv.Dilate(red_eroded_image,red_dilated_image,None,10)
                cv.Dilate(purple_eroded_image,purple_dilated_image,None,9)

                #cv.ShowImage("red threshold",red_dilated_image)                       #show image here because findContours effects memory location
                
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
                                        if (color[color_index] == 'red'):
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
                                        if (color[color_index] == 'purple'):
                                                purple_ring_pos = (x,y)

                              
                                
                 


                         
                cv.SetMouseCallback("camera feed",mouse_callback,hsv_image)   
                #cv.ShowImage("test",dist_image)             
                #cv.ShowImage("H channel",h_channel)
                #cv.ShowImage("S channel",s_channel)
                #cv.ShowImage("V channel",v_channel)
                cv.ShowImage("camera feed",cv_image)
               
                cv.WaitKey(3)

#-----------------------------------------------------------------------------------

def pose_callback(msg):
	global current_position,orientation
        orientation = msg.pose.pose.orientation
	current_position = (msg.pose.pose.position.x,msg.pose.pose.position.y)
        
rospy.Subscriber('/odom', Odometry, pose_callback)
#-----------------------------------------------------------------------------------

class ShootRingsServer:

 def __init__(self):
        self.server = actionlib.SimpleActionServer('shoot_rings', ShootRingsAction, self.execute, False)
        rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)
        self.server.start()
        self._feedback = ShootRingsActionFeedback

 def execute(self,goal):
        global shots
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






