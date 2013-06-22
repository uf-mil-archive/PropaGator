#!/usr/bin/env python

import roslib
roslib.load_manifest('buoys')
import rospy
import numpy as np
import cv,cv2,math,threading
from pointcloud2xyz import *
from std_msgs.msg import String
from sensor_msgs.msg import Image,PointCloud2,PointField
from cv_bridge import CvBridge, CvBridgeError
from visualization_msgs.msg import Marker,MarkerArray
import matplotlib.pyplot as plt
import actionlib

rospy.init_node('buoy_detection')
bridge = CvBridge()

#-----------------------------------------------------------------------------------

#parameters to set
GREEN_MIN = cv.fromarray(np.array([50, 150, 200],np.uint8),allowND = True)
GREEN_MAX = cv.fromarray(np.array([90, 210, 255],np.uint8),allowND = True)

YELLOW_MIN = cv.fromarray(np.array([20, 130, 200],np.uint8),allowND = True)
YELLOW_MAX = cv.fromarray(np.array([40, 180, 255],np.uint8),allowND = True)

OBJECT_AREA = 20
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

#sminv = cv.CreateImage(IMAGE_SIZE,8,1)
#vminh = cv.CreateImage(IMAGE_SIZE,8,1)
sminh = cv.CreateImage(IMAGE_SIZE,8,1)
#yellow_threshold = cv.CreateImage(IMAGE_SIZE,8,1)
#red_threshold = cv.CreateImage(IMAGE_SIZE,8,1)
blurred_bgr_image = cv.CreateImage(IMAGE_SIZE,8,3)

green_threshold_image = cv.CreateImage(IMAGE_SIZE,8,1)
yellow_threshold_image = cv.CreateImage(IMAGE_SIZE,8,1)
blue_threshold_image = cv.CreateImage(IMAGE_SIZE,8,1)

blue_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
red_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
yellow_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
green_adaptive = cv.CreateImage(IMAGE_SIZE,8,1)
red_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
red_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
green_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
green_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
yellow_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
yellow_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
blue_eroded_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)
blue_dilated_image = cv.CreateMat(IMAGE_SIZE[1],IMAGE_SIZE[0],cv.CV_8U)

lock = threading.Lock()

global running,new_buoy,max_distance,master_cloud

master_cloud = []
max_distance = 7
new_buoy = False
running = True


#-----------------------------------------------------------------------------------

#publish marker array of objects found
global buoy_array,marker_id
buoy_publisher=rospy.Publisher('buoy_markers',MarkerArray)
buoy_array=MarkerArray()
marker_id = 0
def append_marker(pos,color):
        global marker_id     
        if (marker_id > 20):
                marker_id = 0
        else:
                marker_id = marker_id + 1
	marker = Marker()
	marker.header.frame_id = "/base_link"
	marker.type = marker.SPHERE
	marker.id = marker_id
	marker.lifetime = rospy.Duration.from_sec(2)
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
	marker.pose.position.z = pos[2]
	buoy_array.markers.append(marker)
        #bouy_publisher.publish(marker)

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
                if (dist < radius*2.0 and dist < min_dist ):          
                        object_center = i[2]
                        object_found = True 
                        min_dist = dist

        lock.release()
        if (object_found):         
                return (True,object_center)
        else:
                return (False,[0,0,0])


def extract_circles(contours,rgb):
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
                                circles.append((x,y,int(radius*1.5)))
                                append_marker(point[1],rgb)
                                global new_buoy
                                new_buoy = True     
        return circles
                        

#-----------------------------------------------------------------------------------
def threshold_red(image):
        cv.AdaptiveThreshold(image,red_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,51,-45)  
        cv.Erode(red_adaptive,red_eroded_image,None,8)                                                                
        cv.Dilate(red_eroded_image,red_dilated_image,None,9)    

def threshold_green(image):
        #cv.InRange(blurred_image,GREEN_MIN,GREEN_MAX,green_adaptive)
        cv.AdaptiveThreshold(image,green_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY_INV,101,25)
        cv.Erode(green_adaptive,green_eroded_image,None,2) #9                                                      
        cv.Dilate(green_eroded_image,green_dilated_image,None,6)#27

def threshold_yellow(image):
        cv.InRange(image,YELLOW_MIN,YELLOW_MAX,yellow_adaptive)
        #cv.AdaptiveThreshold(v_channel,yellow_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,103,-15)
        cv.Erode(yellow_adaptive,yellow_eroded_image,None,5)                                                                            
        cv.Dilate(yellow_eroded_image,yellow_dilated_image,None,9)     

def threshold_blue(image):
        cv.AdaptiveThreshold(image,blue_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,71,-20)
        cv.Erode(blue_adaptive,blue_eroded_image,None,9)                                                        
        cv.Dilate(blue_eroded_image,blue_dilated_image,None,9) 

def print_lidar_projections(image):
        if (len(master_cloud) > 0):
                lock.acquire()
                for i in master_cloud:
                        cv.Circle(image,(int(i[0]),int(i[1])),1,(0,255,0),3)
                lock.release()
#-----------------------------------------------------------------------------------

def image_callback(data):
        
        if (running):
      
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
                threshold_green(a_channel)
                threshold_yellow(blurred_image)
                threshold_blue(h_channel) 
       
                #cv.ShowImage("test",sminv)
                cv.ShowImage("red",red_dilated_image)
                cv.ShowImage("yellow",yellow_adaptive)
                #cv.ShowImage("blue",blue_dilated_image)
                cv.ShowImage("green",green_dilated_image)

                red_contours,_ = cv2.findContours(image=np.asarray(red_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
                green_contours,_ = cv2.findContours(image=np.asarray(green_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
                yellow_contours,_ = cv2.findContours(image=np.asarray(yellow_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
                blue_contours,_ = cv2.findContours(image=np.asarray(blue_dilated_image[:,:]),mode=cv.CV_RETR_EXTERNAL,method=cv.CV_CHAIN_APPROX_SIMPLE)
                #cv2.drawContours(np.asarray(cv_image[:,:]),red_contours,-1,(0,0,255),3)   
            
                print_lidar_projections(cv_image)

                for i in [(green_contours,[0,1,0]) , (yellow_contours,[1,1,0]) , (red_contours,[1,0,0])]:# , (blue_contours,[0,0,1])]:
                        circles = extract_circles(i[0],i[1])
                        rgb = i[1]
                        bgr = (255*np.array(rgb[::-1])).tolist()        #invert list and multiply by 255 for cv.Circle color argument
                        for x,y,radius in circles:                    
                                cv.Circle(cv_image,(x,y),radius,bgr,3)
    

                cv.SetMouseCallback("camera feed",mouse_callback,hsv_image)   
                #cv.ShowImage("l channel",l_channel)
                #cv.ShowImage("a channel",a_channel)
                #cv.ShowImage("b channel",b_channel)
                cv.ShowImage("camera feed",cv_image)
                
                cv.WaitKey(3)
               

#-----------------------------------------------------------------------------------
def buoy_callback(event):
        global new_buoy
        if (running and new_buoy):
                global buoy_array
                buoy_publisher.publish(buoy_array)
                buoy_array=MarkerArray()
                new_buoy = False
          
#-----------------------------------------------------------------------------------

def action_callback(event):
        running = False
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
                print len(cloud)
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
'''
class FindBuoysServer:

 def __init__(self):
        self.server = actionlib.SimpleActionServer('find_buoys', FindBuoysAction, self.execute, False)
        rospy.Timer(rospy.Duration(.1), bouy_callback)
        rospy.Timer(rospy.Duration(1), action_callback)
        rospy.Subscriber("/cloud_3d",PointCloud2,pointcloud_callback)
        rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)
        self.server.start()

 def execute(self,goal):
        running = True

server = FindBuoysServer()
rospy.spin()
'''

rospy.Timer(rospy.Duration(.1), buoy_callback)
rospy.Subscriber("/cloud_3d",PointCloud2,pointcloud_callback)
rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)
rospy.spin()

