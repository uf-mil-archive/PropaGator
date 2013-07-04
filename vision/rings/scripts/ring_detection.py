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
from ioboard.msg import IOBoardAction,IOBoardActionResult,IOBoardGoal

rospy.init_node('ring_detection')
bridge = CvBridge()

waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
shooter = actionlib.SimpleActionClient('ioboard_command', IOBoardAction)
print 'connecting to action client'
#shooter.wait_for_server()
print 'connecting to shooter client'


global running,shot,color
running = False
shot = False
color = []


gain = 1
red_shoot = (350,75)

OBJECT_AREA = 1500
IMAGE_SIZE = (640,480)

#-----------------------------------------------------------------------------------

purple_MIN = cv.fromarray(numpy.array([30, 160, 50],numpy.uint8),allowND = True)
purple_MAX = cv.fromarray(numpy.array([75, 210, 130],numpy.uint8),allowND = True)

#-----------------------------------------------------------------------------------
blurred_gray = cv.CreateImage(IMAGE_SIZE,8,1)
gray = cv.CreateImage(IMAGE_SIZE,8,1)

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

def distance (p1,p2):
	return (math.sqrt((p2[1]-p1[1])**2 + (p2[0]-p1[0])**2))

def adjust_carrot(x,y):
        global shot
        err = distance((x,y),red_shoot)
        adjust_sign = [(red_shoot[0]-x),(y-red_shoot[1])]
        #adjust_mag = [.3,.3]
        if (err > 30):
                print 'pos',(x,y)
                #adjust_mag[0] = math.copysign(adjust_mag[0],adjust_sign[0])
                #adjust_mag[1] = math.copysign(adjust_mag[1],adjust_sign[1])
                adjust_mag = [.001*adjust_sign[0],.001*adjust_sign[1]]
                if (math.fabs(adjust_sign[1]) > 20 and math.fabs(adjust_sign[0]) > 20):
                      print "both"
                      print 'x=',adjust_mag[1],'y=',adjust_mag[0]
                      #waypoint.send_goal(current_pose_editor.relative(numpy.array([adjust_mag[1], 0, 0])).as_MoveToGoal(speed = .3)) 
                      waypoint.send_goal(current_pose_editor.as_MoveToGoal(linear=[adjust_mag[1],adjust_mag[0],0]))           
                elif (math.fabs(adjust_sign[1]) > 20):
                      print "x"
                      print adjust_mag[1]
                      #waypoint.send_goal(current_pose_editor.relative(numpy.array([adjust_mag[1], 0, 0])).as_MoveToGoal(speed = .3)) 
                      waypoint.send_goal(current_pose_editor.as_MoveToGoal(linear=[adjust_mag[1],0,0]))                
                elif (math.fabs(adjust_sign[0]) > 20):
                      print "y"
                      print adjust_mag[0]
                      waypoint.send_goal(current_pose_editor.as_MoveToGoal(linear=[0,adjust_mag[0],0]))     
        else:
                goal = IOBoardGoal(command = 'Shoot2')
                print "shooting!"
                waypoint.cancel_goal()
                shooter.send_goal_and_wait(goal)
                shot = True

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

def extract_circles(contours,rgb):
        circles = []
        global shot
        for i in contours:
                moments = cv.Moments(cv.fromarray(i), binary = 1)             
                area = cv.GetCentralMoment(moments, 0, 0)

                if area > OBJECT_AREA:
                        x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                        y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                        radius = int(math.sqrt(area/math.pi))
                        if (shot == False):
                                circles.append((x,y,int(radius)))
                                adjust_carrot(x,y)          
        return circles 

#-----------------------------------------------------------------------------------    

def image_callback(data):
        global running,color
        if (running):
                #print "running"
                cv_image = bridge.imgmsg_to_cv(data,"bgr8")

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
                
                if (color == 'red'):
                        threshold_red(sa)
                        #threshold_red(ycrcb_cr)     
                        red_contours,_ = cv2.findContours(image=numpy.asarray(red_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE) 
                        circles = extract_circles(red_contours,[1,0,0])
                        for x,y,radius in circles:  
                                cv.Circle(cv_image,(x,y),radius,[0,0,255],3)
                elif(color == 'purple'):
                        threshold_purple(hsv_s)
                        purple_contours,_ = cv2.findContours(image=numpy.asarray(purple_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
                        circles = extract_circles(purple_contours,[1,0,1])
                        for x,y,radius in circles:  
                                cv.Circle(cv_image,(x,y),radius,[255,0,255],3)

                cv.ShowImage("red",red_adaptive)      
                cv.SetMouseCallback("camera feed",mouse_callback,hsv)   
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
                cv.ShowImage("XYZ_X",xyz_x)
                cv.ShowImage("XYZ_Y",xyz_y)
                cv.ShowImage("XYZ_Z",xyz_z)
                cv.ShowImage("YCrCb_Y",ycrcb_y)
                cv.ShowImage("YCrCb_Cr",ycrcb_cr)
                cv.ShowImage("YCrCb_Cb",ycrcb_cb)
                '''
                cv.ShowImage("camera feed",cv_image)
                
                cv.WaitKey(3)
rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)
#-----------------------------------------------------------------------------------

class ShootRingsServer:

 def __init__(self):
        global running
        self.server = actionlib.SimpleActionServer('shoot_rings', ShootRingsAction, self.execute, False)
        self.server.start()
        self._feedback = ShootRingsActionFeedback
        print "rings server started"

 def execute(self,goal):
        global shot,running,color
        shot = False
        color = goal.color
        while(not(shot) and not(self.server.is_preempt_requested())):
                running = True
                rospy.sleep(1)
        
        running = False
        if (shot):
                shot = False
                self.server.set_succeeded()       
        else:
                shot = False
                self.server.set_preempted()
              

def pose_callback(msg):
	global current_pose_editor
	current_pose_editor = PoseEditor.from_Odometry(msg)
	current_position = (msg.pose.pose.position.x,msg.pose.pose.position.y)
rospy.Subscriber('/odom', Odometry, pose_callback)

server = ShootRingsServer()
rospy.spin()






