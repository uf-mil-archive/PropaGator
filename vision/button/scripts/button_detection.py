#!/usr/bin/env python

import roslib
roslib.load_manifest('button')
import rospy
import numpy as np
import cv,cv2,math,threading
from nav_msgs.msg import Odometry
from pointcloud2xyz import *
from std_msgs.msg import String
from sensor_msgs.msg import Image,PointCloud2,PointField
from cv_bridge import CvBridge, CvBridgeError
from visualization_msgs.msg import Marker,MarkerArray
import matplotlib.pyplot as plt
import actionlib
from button.msg import FindButtonAction
from uf_common.msg import MoveToAction, MoveToGoal
from uf_common.orientation_helpers import lookat, get_perpendicular,PoseEditor

rospy.init_node('button_detection')
bridge = CvBridge()

waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
print 'connecting to action client'

OBJECT_AREA = 10
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

distortion_coeffs[0,0] = -0.216569744368694
distortion_coeffs[0,1] =  0.150298190916304 
distortion_coeffs[0,2] = 0.000482115806222 
distortion_coeffs[0,3] = -0.000462572155571 

rotation_vector[0,0] =   1.1647
rotation_vector[0,1] =  -1.2071
rotation_vector[0,2] =   1.2281

translation_vector[0,0] =  -0.135459  #-0.25#
translation_vector[0,1] =  0.545470#0.145470
translation_vector[0,2] =  0.818359   # .5#

#-----------------------------------------------------------------------------------

#memory allocation
hsv_image = cv.CreateImage(IMAGE_SIZE,8,3)
blurred_image = cv.CreateImage(IMAGE_SIZE,8,3)

h_channel = cv.CreateImage(IMAGE_SIZE,8,1)
s_channel = cv.CreateImage(IMAGE_SIZE,8,1)  
v_channel = cv.CreateImage(IMAGE_SIZE,8,1)

lab_image_ = cv.CreateImage(IMAGE_SIZE,8,3)
l_channel = cv.CreateImage(IMAGE_SIZE,8,1)
a_channel = cv.CreateImage(IMAGE_SIZE,8,1)  
b_channel = cv.CreateImage(IMAGE_SIZE,8,1)

sminh = cv.CreateImage(IMAGE_SIZE,8,1)
blurred_bgr_image = cv.CreateImage(IMAGE_SIZE,8,3)

red_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
red_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
red_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)


lock = threading.Lock()

global running,new_buoy,max_distance,master_cloud

master_cloud = []
max_distance = 7
running = False

#-----------------------------------------------------------------------------------
#--print HSV of pixel clicked on in image (used to find threshold values)
def mouse_callback(event,x,y,flags,image):

        if event==cv.CV_EVENT_LBUTTONDOWN:
                print cv.Get2D(image,y,x)  
#----------------------------------------------------------------------------------

def distance(p0, p1):
    return math.sqrt((p0[0] - p1[0])**2 + (p0[1] - p1[1])**2)

def check_lidar((x,y),radius):
        min_dist = 200
        object_found = False
        
        lock.acquire()
        for i in master_cloud:
                dist = distance((i[0],i[1]),(x,y))
                if (dist < radius*3.0 and dist < min_dist ):          
                        object_center = i[2]
                        object_found = True 
                        min_dist = dist

        lock.release()
        if (object_found):         
                return (True,object_center)
        else:
                return (False,[0,0,0])

def extract_circles(contours,rgb):
        global current_pose_editor        
        circles = []
        for i in contours:
                moments = cv.Moments(cv.fromarray(i), binary = 1)             
                area = cv.GetCentralMoment(moments, 0, 0)

                if area > OBJECT_AREA:
                        x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                        y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                        radius = int(math.sqrt(area/math.pi))

                        point = check_lidar((x,y),radius)
                        if (point[0]):
                                #waypoint.send_goal_and_wait(current_pose_editor.relative(numpy.array([point[1][0] + .1, point[1][1] + .1, 0])).as_MoveToGoal(speed = .2))
                                circles.append((x,y,int(radius*1.5)))   
        return circles                

#-----------------------------------------------------------------------------------
def threshold_red(image):
        cv.AdaptiveThreshold(image,red_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,51,-50)  
        cv.Erode(red_adaptive,red_eroded_image,None,5)                                                                
        cv.Dilate(red_eroded_image,red_dilated_image,None,9)    

def print_lidar_projections(image):
        if (len(master_cloud) > 0):
                lock.acquire()
                for i in master_cloud:
                        cv.Circle(image,(int(i[0]),int(i[1])),1,(0,255,0),3)
                lock.release()
#-----------------------------------------------------------------------------------

def image_callback(data):
        
        global running       
        if (running):
                print "running"
                cv_image = bridge.imgmsg_to_cv(data,"bgr8")
                cv.CvtColor(cv_image,hsv_image,cv.CV_BGR2HSV)                         # --convert from BGR to HSV
                cv.CvtColor(cv_image,lab_image_,cv.CV_BGR2Lab)

                cv.Smooth(cv_image,blurred_bgr_image,cv.CV_GAUSSIAN,9,9)  
                cv.Smooth(hsv_image,blurred_image,cv.CV_GAUSSIAN,5,5)                
                cv.Split(blurred_image,h_channel,s_channel,v_channel,None) 
                cv.Split(lab_image_,l_channel,a_channel,b_channel,None)   

                cv.Sub(s_channel,h_channel,sminh)
                #v.Sub(v_channel,h_channel,vminh)
                #cv.Sub(s_channel,v_channel,sminv) #maybe use for blue            
                
                threshold_red(sminh)
       
                #cv.ShowImage("test",sminv)
                cv.ShowImage("red",red_adaptive)

                red_contours,_ = cv2.findContours(image=np.asarray(red_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
            
                print_lidar_projections(cv_image)

                circles = extract_circles(red_contours,[1,0,0])
                for x,y,radius in circles:                    
                        cv.Circle(cv_image,(x,y),radius,[0,0,1],3)

                cv.SetMouseCallback("camera feed",mouse_callback,hsv_image)   
                #cv.ShowImage("l channel",l_channel)
                #cv.ShowImage("a channel",a_channel)
                #cv.ShowImage("b channel",b_channel)
                cv.ShowImage("camera feed",cv_image)
                
                cv.WaitKey(3)
               
#-----------------------------------------------------------------------------------
def in_frame(x):
        if (x[0] < 600 and x[0] > 0 and x[1] < 600 and x[1] > 0 and all(math.fabs(i) < max_distance for i in x[2])):
                return True
        else:
                return False

def pointcloud_callback(msg):
        if (running):
                lock.acquire()
                global master_cloud
                cloud = pointcloud2_to_xyz_array(msg)
                #print len(cloud)
                if (len(cloud) > 0):
                        cloud_mat = cv.CreateMat(len(cloud),1,cv.CV_32FC3)
                        projection = cv.CreateMat(len(cloud),1,cv.CV_32FC2)
                        cloud_mat = cloud
                     
                        cv.ProjectPoints2(cv.fromarray(cloud_mat),rotation_vector,translation_vector,intrinsic_mat,distortion_coeffs,projection)
                        (x,y) = cv2.split(np.asarray(projection))
                        index = 0
                        master_cloud = []
                        for i,j in zip(x,y):
                                master_cloud.append([i[0],j[0],cloud[index]])
                                index = index + 1
                        master_cloud = filter(in_frame,master_cloud)
                lock.release()
                
#-----------------------------------------------------------------------------------
def pose_callback(msg):
	global current_position,current_pose_editor
	current_pose_editor = PoseEditor.from_Odometry(msg)
	current_position = (msg.pose.pose.position.x,msg.pose.pose.position.y)
rospy.Subscriber('/odom', Odometry, pose_callback)

#-----------------------------------------------------------------------------------
class FindButtonServer:

 def __init__(self):
        self.server = actionlib.SimpleActionServer('find_button', FindButtonAction, self.execute, False)
        rospy.Subscriber("/cloud_3d",PointCloud2,pointcloud_callback)
        rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)
        self.server.start()
        print "button server started"

 def execute(self,goal):
        global running
        while (not(self.server.is_preempt_requested())):
             running = True
        running = False
        self.server.set_preempted() 

server = FindButtonServer()
rospy.spin()

