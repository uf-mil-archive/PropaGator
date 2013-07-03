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

OBJECT_AREA = 1000
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

sminh = cv.CreateImage(IMAGE_SIZE,8,1)
blurred_bgr_image = cv.CreateImage(IMAGE_SIZE,8,3)

red_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
red_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
red_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)


lock = threading.Lock()

global running,new_buoy,max_distance,master_cloud,rammed
rammed = False
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

desired = [600,1000] #x position,area
def adjust_carrot(x,y,area):
        global rammed
        err = (desired[0]-x)
        if (err > 20):
                print 'pos',(x,y)
                adjust = err
                print "y"
                print adjust
                #waypoint.send_goal(current.as_MoveToGoal(linear=[0,adjust,0]))  
        elif (area < desired[1]):
                adjust = desired[1] - area
                print 'x'
                print adjust
                #waypoint.send_goal(current.as_MoveToGoal(linear=[adjust,0,0]))  
        else:
                print "ramming"
                #waypoint.cancel_goal()
                #waypoint.send_goal_and_wait(current_pose_editor.forward(3))
                #waypoint.send_goal_and_wait(current_pose_editor.backward(3))
                rammed = True

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
                        circles.append((x,y,int(radius*1.5))) 
                        adjust_carrot(x,y,area)                             #use just visual servo
                        '''
                        point = check_lidar((x,y),radius)               #use if you want to use lidar to confirm waypoint
                        if (point[0]):
                                print 'going to:',point[1]
                                waypoint.send_goal_and_wait(current_pose_editor.relative(np.array([point[1][0], point[1][1] + .5, 0])).as_MoveToGoal(speed = .5))
                                circles.append((x,y,int(radius*1.5))) 
                        '''  
        return circles                

#-----------------------------------------------------------------------------------
def threshold_red(image):
    
        cv.AdaptiveThreshold(image,red_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,27,-25)
        cv.Erode(red_adaptive,red_eroded_image,None,2)
        cv.Dilate(red_eroded_image,red_dilated_image,None,5)  


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
                cv_image = bridge.imgmsg_to_cv(data,"bgr8")
                cv.CvtColor(cv_image,hsv,cv.CV_BGR2HSV)                         # --convert from BGR to HSV
                cv.CvtColor(cv_image,lab,cv.CV_BGR2Lab)

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
                cv.Sub(hls_s,hls_h,sminh)
        
                threshold_red(sa)
             
                cv.ShowImage("red",red_dilated_image)

                red_contours,_ = cv2.findContours(image=np.asarray(red_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
            
                print_lidar_projections(cv_image)

                circles = extract_circles(red_contours,[1,0,0])
                for x,y,radius in circles:                    
                        cv.Circle(cv_image,(x,y),radius,[0,0,1],3)

                cv.SetMouseCallback("camera feed",mouse_callback,hsv_image)   
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
        #rospy.Subscriber("/cloud_3d",PointCloud2,pointcloud_callback)
        rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)
        self.server.start()
        print "button server started"

 def execute(self,goal):
        global running,rammed
        while (not(self.server.is_preempt_requested()) and not(rammed)):
             running = True
        running = False
        rammed = False
        self.server.set_preempted() 

server = FindButtonServer()
rospy.spin()

