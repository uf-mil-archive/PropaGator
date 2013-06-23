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
from uf_common.msg import MoveToAction, MoveToGoal
from uf_common.orientation_helpers import lookat, get_perpendicular,PoseEditor


rospy.init_node('ring_detection')
bridge = CvBridge()

waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
print 'connecting to action client'

global running,shots,colors,color_index
running = False
shots = 0
colors = ['red','green','purple']
color_index = 0

#-----------------------------------------------------------------------------------

purple_MIN = cv.fromarray(numpy.array([30, 160, 50],numpy.uint8),allowND = True)
purple_MAX = cv.fromarray(numpy.array([75, 210, 130],numpy.uint8),allowND = True)
OBJECT_AREA = 15000
IMAGE_SIZE = (640,480)

#-----------------------------------------------------------------------------------
hsv_image = cv.CreateImage(IMAGE_SIZE,8,3)
blurred_image = cv.CreateImage(IMAGE_SIZE,8,3)
h_channel = cv.CreateImage(IMAGE_SIZE,8,1)
s_channel = cv.CreateImage(IMAGE_SIZE,8,1)  
v_channel = cv.CreateImage(IMAGE_SIZE,8,1)
h_s = cv.CreateImage(IMAGE_SIZE,8,1)
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
gain = .1
red_shoot = (320,240)
def distance (p1,p2):
	return (math.sqrt((p2[1]-p1[1])**2 + (p2[0]-p1[0])**2))
def adjust_carrot(x,y):
        err = distance((x,y),red_shoot)
        adjust = (x-red_shoot[0],y-red_shoot[1])
        if (err > 10):
                waypoint.send_goal(current_pose_editor.relative(numpy.array([x, y, 0])).as_MoveToGoal(speed = .1))
        else:
                print "shooting!"
                color_index = color_index + 1
                shots = shots + 1  

#----------------------------------------------------------------------------------- 
def threshold_purple(image):
        cv.AdaptiveThreshold(image,purple_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY_INV,53,20)
        cv.Erode(purple_adaptive,purple_eroded_image,None,1)
        cv.Dilate(purple_adaptive,purple_dilated_image,None,8)

def threshold_red(image):
        cv.AdaptiveThreshold(image,red_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,201,-40)
        cv.Erode(red_adaptive,red_eroded_image,None,1)
        cv.Dilate(red_eroded_image,red_dilated_image,None,10)    

#-----------------------------------------------------------------------------------   

def extract_circles(contours,rgb):
        circles = []
        for i in contours:
                moments = cv.Moments(cv.fromarray(i), binary = 1)             
                area = cv.GetCentralMoment(moments, 0, 0)

                if area > OBJECT_AREA:
                        x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                        y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                        radius = int(math.sqrt(area/math.pi))
                       
                        if (colors[color_index] == 'red'):
                                pass
                                #adjust_carrot(x,y)          
        return circles 

#-----------------------------------------------------------------------------------    

def image_callback(data):
        global running,colors
        if (running):
                #print "running"
                cv_image = bridge.imgmsg_to_cv(data,"bgr8")
                cv.CvtColor(cv_image,hsv_image,cv.CV_BGR2HSV)                        

                cv.Smooth(hsv_image,blurred_image,cv.CV_GAUSSIAN,5,5)                
                cv.Split(hsv_image,h_channel,s_channel,v_channel,None)

                threshold_purple(s_channel)
                threshold_red(s_channel)               

                red_contours,_ = cv2.findContours(image=numpy.asarray(red_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
                purple_contours,_ = cv2.findContours(image=numpy.asarray(purple_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
                #cv2.drawContours(numpy.asarray(cv_image[:,:]),red_contours,-1,(0,0,255),3)   

                for i in [ (red_contours,[1,0,0]) , (purple_contours,[1,0,1]) ]:
                        circles = extract_circles(i[0],i[1])
                        rgb = i[1]
                        bgr = (255*numpy.array(rgb[::-1])).tolist()        #invert list and multiply by 255 for cv.Circle color argument
                        for x,y,radius in circles:                    
                                cv.Circle(cv_image,(x,y),radius,bgr,3)  

                cv.ShowImage("red",red_adaptive)
                cv.ShowImage("purple",purple_adaptive)

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
        while(shots < goal.attempts and not(self.server.is_preempt_requested())):
                running = True
                self._feedback.darts_shot = shots
                self.server.publish_feedback(self._feedback)
                #shots = shots + 1
                rospy.sleep(1)
             
        running = False
        if (shots == goal.attempts):
                self.server.set_succeeded()
                shots = 0
        else:
                self.server.set_preempted()
                shots = 0

def pose_callback(msg):
	global current_pose_editor
	current_pose_editor = PoseEditor.from_Odometry(msg)
	current_position = (msg.pose.pose.position.x,msg.pose.pose.position.y)
rospy.Subscriber('/odom', Odometry, pose_callback)

server = ShootRingsServer()
rospy.spin()






