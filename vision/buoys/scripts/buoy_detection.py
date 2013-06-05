#!/usr/bin/env python

import roslib
roslib.load_manifest('buoys')
import rospy
import numpy as np
import cv,cv2,math
from std_msgs.msg import String
from sensor_msgs.msg import Image,PointCloud2,PointField
from cv_bridge import CvBridge, CvBridgeError
from visualization_msgs.msg import Marker,MarkerArray
import matplotlib.pyplot as plt

rospy.init_node('buoy_detection')
bridge = CvBridge()

#-----------------------------------------------------------------------------------

#cv.NamedWindow("camera feed",1)
#cv.NamedWindow("H channel",1)
#cv.NamedWindow("S channel",1)
#cv.NamedWindow("V channel",1)
#cv.NamedWindow("red threshold",1)
#cv.NamedWindow("green threshold",1)

#-----------------------------------------------------------------------------------

#parameters to set
RED_MIN = cv.fromarray(np.array([0, 125, 0],np.uint8),allowND = True)
RED_MAX = cv.fromarray(np.array([15, 255, 60],np.uint8),allowND = True)

GREEN_MIN = cv.fromarray(np.array([30, 160, 50],np.uint8),allowND = True)
GREEN_MAX = cv.fromarray(np.array([75, 210, 130],np.uint8),allowND = True)

YELLOW_MIN = cv.fromarray(np.array([10, 190, 220],np.uint8),allowND = True)
YELLOW_MAX = cv.fromarray(np.array([35, 210, 255],np.uint8),allowND = True)

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
#cloud1 = cv.CreateImage(IMAGE_SIZE,8,1)
#cloud2 = cv.CreateImage(IMAGE_SIZE,8,1)
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

global cloud1,cloud2
cloud1 = []
cloud2 = []


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
	marker.lifetime = rospy.Duration(.5)
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
	marker.pose.position.z = 0
	bouy_array.markers.append(marker)

#-----------------------------------------------------------------------------------

#--print HSV of pixel clicked on in image (used to find threshold values)
def mouse_callback(event,x,y,flags,image):

        if event==cv.CV_EVENT_LBUTTONDOWN:
                print cv.Get2D(image,y,x)  
        
#-----------------------------------------------------------------------------------
DUMMY_FIELD_PREFIX = '__'

# mappings between PointField types and np types
type_mappings = [(PointField.INT8, np.dtype('int8')), (PointField.UINT8, np.dtype('uint8')), (PointField.INT16, np.dtype('int16')),
                 (PointField.UINT16, np.dtype('uint16')), (PointField.INT32, np.dtype('int32')), (PointField.UINT32, np.dtype('uint32')),
                 (PointField.FLOAT32, np.dtype('float32')), (PointField.FLOAT64, np.dtype('float64'))]
pftype_to_nptype = dict(type_mappings)
nptype_to_pftype = dict((nptype, pftype) for pftype, nptype in type_mappings)

# sizes (in bytes) of PointField types
pftype_sizes = {PointField.INT8: 1, PointField.UINT8: 1, PointField.INT16: 2, PointField.UINT16: 2,
                PointField.INT32: 4, PointField.UINT32: 4, PointField.FLOAT32: 4, PointField.FLOAT64: 8}

def pointcloud2_to_dtype(cloud_msg):
    '''Convert a list of PointFields to a np record datatype.
    '''
    offset = 0
    np_dtype_list = []
    for f in cloud_msg.fields:
        while offset < f.offset:
            # might be extra padding between fields
            np_dtype_list.append(('%s%d' % (DUMMY_FIELD_PREFIX, offset), np.uint8))
            offset += 1
        np_dtype_list.append((f.name, pftype_to_nptype[f.datatype]))
        offset += pftype_sizes[f.datatype]

    # might be extra padding between points
    while offset < cloud_msg.point_step:
        np_dtype_list.append(('%s%d' % (DUMMY_FIELD_PREFIX, offset), np.uint8))
        offset += 1
        
    return np_dtype_list

def arr_to_fields(cloud_arr):
    '''Convert a np record datatype into a list of PointFields.
    '''
    fields = []
    for field_name in cloud_arr.dtype.names:
        np_field_type, field_offset = cloud_arr.dtype.fields[field_name]
        pf = PointField()
        pf.name = field_name
        pf.datatype = nptype_to_pftype[np_field_type]
        pf.offset = field_offset
        pf.count = 1 # is this ever more than one?
        fields.append(pf)
    return fields

def pointcloud2_to_array(cloud_msg, split_rgb=False):
    ''' Converts a rospy PointCloud2 message to a np recordarray 
    
    Reshapes the returned array to have shape (height, width), even if the height is 1.

    The reason for using np.fromstring rather than struct.unpack is speed... especially
    for large point clouds, this will be <much> faster.
    '''
    # construct a np record type equivalent to the point type of this cloud
    dtype_list = pointcloud2_to_dtype(cloud_msg)

    # parse the cloud into an array
    cloud_arr = np.fromstring(cloud_msg.data, dtype_list)

    # remove the dummy fields that were added
    cloud_arr = cloud_arr[
        [fname for fname, _type in dtype_list if not (fname[:len(DUMMY_FIELD_PREFIX)] == DUMMY_FIELD_PREFIX)]]

    if split_rgb:
        cloud_arr = split_rgb_field(cloud_arr)
    
    return np.reshape(cloud_arr, (cloud_msg.height, cloud_msg.width))

def array_to_pointcloud2(cloud_arr, stamp=None, frame_id=None, merge_rgb=False):
    '''Converts a np record array to a sensor_msgs.msg.PointCloud2.
    '''
    if merge_rgb:
        cloud_arr = merge_rgb_fields(cloud_arr)

    # make it 2d (even if height will be 1)
    cloud_arr = np.atleast_2d(cloud_arr)

    cloud_msg = PointCloud2()

    if stamp is not None:
        cloud_msg.header.stamp = stamp
    if frame_id is not None:
        cloud_msg.header.frame_id = frame_id
    cloud_msg.height = cloud_arr.shape[0]
    cloud_msg.width = cloud_arr.shape[1]
    cloud_msg.fields = arr_to_fields(cloud_arr)
    cloud_msg.is_bigendian = False # assumption
    cloud_msg.point_step = cloud_arr.dtype.itemsize
    cloud_msg.row_step = cloud_msg.point_step*cloud_arr.shape[1]
    cloud_msg.is_dense = all([np.isfinite(cloud_arr[fname]).all() for fname in cloud_arr.dtype.names])
    cloud_msg.data = cloud_arr.tostring()
    return cloud_msg

def merge_rgb_fields(cloud_arr):
    '''Takes an array with named np.uint8 fields 'r', 'g', and 'b', and returns an array in
    which they have been merged into a single np.float32 'rgb' field. The first byte of this
    field is the 'r' uint8, the second is the 'g', uint8, and the third is the 'b' uint8.

    This is the way that pcl likes to handle RGB colors for some reason.
    '''
    r = np.asarray(cloud_arr['r'], dtype=np.uint32)
    g = np.asarray(cloud_arr['g'], dtype=np.uint32)
    b = np.asarray(cloud_arr['b'], dtype=np.uint32)    
    rgb_arr = np.array((r << 16) | (g << 8) | (b << 0), dtype=np.uint32)

    # not sure if there is a better way to do this. i'm changing the type of the array
    # from uint32 to float32, but i don't want any conversion to take place -jdb
    rgb_arr.dtype = np.float32

    # create a new array, without r, g, and b, but with rgb float32 field
    new_dtype = []
    for field_name in cloud_arr.dtype.names:
        field_type, field_offset = cloud_arr.dtype.fields[field_name]
        if field_name not in ('r', 'g', 'b'):
            new_dtype.append((field_name, field_type))
    new_dtype.append(('rgb', np.float32))
    new_cloud_arr = np.zeros(cloud_arr.shape, new_dtype)    

    # fill in the new array
    for field_name in new_cloud_arr.dtype.names:
        if field_name == 'rgb':
            new_cloud_arr[field_name] = rgb_arr
        else:
            new_cloud_arr[field_name] = cloud_arr[field_name]
        
    return new_cloud_arr

def split_rgb_field(cloud_arr):
    '''Takes an array with a named 'rgb' float32 field, and returns an array in which
    this has been split into 3 uint 8 fields: 'r', 'g', and 'b'.

    (pcl stores rgb in packed 32 bit floats)
    '''
    rgb_arr = cloud_arr['rgb'].copy()
    rgb_arr.dtype = np.uint32
    r = np.asarray((rgb_arr >> 16) & 255, dtype=np.uint8)
    g = np.asarray((rgb_arr >> 8) & 255, dtype=np.uint8)
    b = np.asarray(rgb_arr & 255, dtype=np.uint8)
    
    # create a new array, without rgb, but with r, g, and b fields
    new_dtype = []
    for field_name in cloud_arr.dtype.names:
        field_type, field_offset = cloud_arr.dtype.fields[field_name]
        if not field_name == 'rgb':
            new_dtype.append((field_name, field_type))
    new_dtype.append(('r', np.uint8))
    new_dtype.append(('g', np.uint8))
    new_dtype.append(('b', np.uint8))    
    new_cloud_arr = np.zeros(cloud_arr.shape, new_dtype)
    
    # fill in the new array
    for field_name in new_cloud_arr.dtype.names:
        if field_name == 'r':
            new_cloud_arr[field_name] = r
        elif field_name == 'g':
            new_cloud_arr[field_name] = g
        elif field_name == 'b':
            new_cloud_arr[field_name] = b
        else:
            new_cloud_arr[field_name] = cloud_arr[field_name]
    return new_cloud_arr

def get_xyz_points(cloud_array, remove_nans=True, dtype=np.float):
    '''Pulls out x, y, and z columns from the cloud recordarray, and returns
        a 3xN matrix.
    '''
    # remove crap points
    if remove_nans:
        mask = np.isfinite(cloud_array['x']) & np.isfinite(cloud_array['y']) & np.isfinite(cloud_array['z'])
        cloud_array = cloud_array[mask]
    
    # pull out x, y, and z values
    points = np.zeros(list(cloud_array.shape) + [3], dtype=dtype)
    points[...,0] = cloud_array['x']
    points[...,1] = cloud_array['y']
    points[...,2] = cloud_array['z']

    return points

def pointcloud2_to_xyz_array(cloud_msg, remove_nans=True):
    return get_xyz_points(pointcloud2_to_array(cloud_msg), remove_nans=remove_nans)

#-----------------------------------------------------------------------------------

def image_callback(data):
      
        cv_image = bridge.imgmsg_to_cv(data,"bgr8")
        cv.CvtColor(cv_image,hsv_image,cv.CV_BGR2HSV)                         # --convert from BGR to HSV

        cv.Smooth(cv_image,blurred_bgr_image,cv.CV_GAUSSIAN,9,9)  
        cv.Smooth(hsv_image,blurred_image,cv.CV_GAUSSIAN,5,5)                 # --three parameters are (aperture_width,aperture_height,gaussian std_dev)
                                                                              # must  be an odd number, std_dev of 0 means that it is calculated from 
                                                                              # the kernel size
 
        cv.InRange(blurred_image,RED_MIN,RED_MAX,red_threshold_image)         #--threshold color based on HSV range
        cv.InRange(blurred_image,GREEN_MIN,GREEN_MAX,green_threshold_image) 
        cv.InRange(blurred_image,YELLOW_MIN,YELLOW_MAX,yellow_threshold_image)  
       
        cv.Split(hsv_image,h_channel,s_channel,v_channel,None)                #split HSV image into three seperate images
        
        cv.Not(s_channel,s_inv)
        cv.Not(h_channel,h_inv)
        cv.Mul(h_channel,s_inv,h_s_not)
        cv.Mul(h_inv,s_channel,h_not_s)
        cv.Mul(s_channel,v_channel,s_v)
        cv.AdaptiveThreshold(h_s_not,red_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY_INV,53,5)      #use hue channel to filter for red
        cv.AdaptiveThreshold(s_v,yellow_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,303,-15)
        cv.AdaptiveThreshold(s_channel,green_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,303,-60)
        #cv.AdaptiveThreshold(h_channel,green_adaptive,255,cv.CV_ADAPTIVE_THRESH_MEAN_C,cv.CV_THRESH_BINARY,303,-10)
        
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
        #cv2.drawContours(np.asarray(cv_image[:,:]),green_contours,-1,(0,255,0),3)     
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
        '''
       
       

        if (green_contours):
                for i in green_contours:
                        ellipse = cv2.fitEllipse(i)
                        cv2.ellipse(np.asarray(cv_image[:,:]),ellipse,(255,0,0),2)
                        print "ellipse = ",ellipse
                        moments = cv.Moments(cv.fromarray(i), binary = 1)             
                        area = cv.GetCentralMoment(moments, 0, 0)
                        if area > OBJECT_AREA:
                                x = int(cv.GetSpatialMoment(moments, 1, 0)/area)
                                y = int(cv.GetSpatialMoment(moments, 0, 1)/area)
                                radius = int(math.sqrt(area/math.pi))
                                cv.Circle(cv_image,(x,y),radius,(0,255,0),3)
                                #append_marker((x,y),(0,1.0,0))
                                #check_lidar((x,y))
        #cv.ShowImage("final",green_dilated_image)     
        global cloud1   
         
            
        if (len(cloud1) > 0):
                for i,j in zip(cloud1,cloud2):
                        if (i < 700 and j < 700 and i > 0 and j > 0):
                                print "i",i,"j",j
                                cv.Circle(cv_image,(int(i),int(j)),1,(0,255,0),3)
        '''      
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
        global cloud1,cloud2
        cloud = pointcloud2_to_xyz_array(msg)
        cloud_mat = cv.CreateMat(len(cloud),1,cv.CV_32FC3)
        projection = cv.CreateMat(len(cloud),1,cv.CV_32FC2)
        cloud_mat = cloud
     
        cv.ProjectPoints2(cv.fromarray(cloud_mat),rotation_vector,translation_vector,intrinsic_mat,distortion_coeffs,projection)
       
        (cloud1,cloud2) = cv2.split(np.asarray(projection))
        print "pointcloud",cloud1,cloud2
        #cv.ShowImage("lidar",cv.fromarray(cloud1))
        '''
        if  (len(np.asarray(projection)) > 10):
                plt.scatter(cloud1,cloud2)
                plt.show()
        '''
        
#-----------------------------------------------------------------------------------

rospy.Timer(rospy.Duration(.1), bouy_callback)
rospy.Subscriber("/cloud_3d",PointCloud2,pointcloud_callback)
rospy.Subscriber("/mv_bluefox_camera_node/image_raw",Image,image_callback)
rospy.spin()






