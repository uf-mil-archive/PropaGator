#!/usr/bin/env python
'''THIS IS ZACH GOINS'S FILE GO TO HIM WITH QUESTIONS
'''
import numpy 
import cv2
from cv_bridge import CvBridge, CvBridgeError
import math
from sensor_msgs.msg import PointCloud2, PointField, CompressedImage, Image
import sensor_msgs.point_cloud2 as pc2
import rospy
from roslib import message

DISPLAY_SIZE = (960, 480) # Display to be printed out to 

white = (255,255,255)
black = (0,0,0)

X_OFFSET = 80 
Y_OFFSET = 300


'''

Vector notation: [x y z]

Method:

1. Take [x y z] vector from pointcloud
2. Find angle between each vector and the axis unit vector 

	cos = (focus_vector)dot(unit_vector) / ||focus_vector||
	angle in radians = arccos(cos)

3. Map x and y angle values by creating a right triangle from known center points
   and using hypotenuse = tan(theda found above) * ajacent. Where the hypotenuse for the x and y 
   collide is the mapped point

Still much work to do, to be continued

'''

class lidar_theta(object):

	def __init__(self):

		# unit vectors for angle comparison
		self.x_unit_vect = numpy.array([1,0,0])
		self.y_unit_vect = numpy.array([0,1,0])
		self.z_unit_vect = numpy.array([0,0,1])

		# vector that will be filled to be compared to
		self.focus_vector = numpy.array([1,1,1])
		self.quad_assign = numpy.array([1,1,1])

		# FOR TESTING ONLY RIGHT NOW
		# height of display to be printed to 
		# height and width will contain ueye camera dimensions eventually

		self.height  = 480
		self.width  = 640
		self.lidar_height = 0
		self.lidar_width = 0
		self.image = numpy.zeros((DISPLAY_SIZE[0]/2,DISPLAY_SIZE[1]/2))
		self.blank_image = numpy.zeros((DISPLAY_SIZE[0]/2,DISPLAY_SIZE[1]/2))

		self.x_co_final = 0
		self.y_co_final = 0

		# find center of camera image
		self.width_center = self.width / 2
		self.height_center = self.height / 2

		# Angles between vector and x,y,z axis
		self.x_theta = 0
		self.y_theta = 0
		self.z_theta = 0

		self.bridge = CvBridge()

		self.quadrant = 0

		self.rate = rospy.Rate(1)

		# starts main loop that constantly feeds from lidar
		rospy.Subscriber("/lidar/raw_pc", PointCloud2, self.pointcloud_callback)
		#rospy.Subscriber("/forward_camera/image_rect_color", Image, self.image_cb)
		self.vison_fusion = rospy.Publisher("camera/lidar_camera_blend", Image, queue_size = 10)

		'''
	def image_cb(self, data):
		try:
			self.cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
		except CvBridgeError, e:
			print e

		(rows,cols,channels) = self.cv_image.shape

		self.image = numpy.zeros((rows,cols))
		self.blank_image = numpy.zeros((rows,cols))
		self.image = self.cv_image		
		'''

	def pointcloud_callback(self, data):

		# Read from pointcloud
		pointcloud = pc2.read_points(data, field_names=("x", "y", "z"), skip_nans=False, uvs=[])
		self.blank_image = numpy.zeros((DISPLAY_SIZE[0]/2,DISPLAY_SIZE[1]/2))
		self.lidar_height = data.height
		self.lidar_width = data.width

		# While there are points in the cloud to read...
		while True:
			try:
				# new point
				xyz_point = next(pointcloud) 
				x, y, z = xyz_point[0], xyz_point[1], xyz_point[2]
				self.quad_assign = numpy.array([x, y, z]) 
				x, y, z = abs(xyz_point[0]), abs(xyz_point[1]), abs(xyz_point[2])
				self.focus_vector = numpy.array([x, y, z]) 
				# solve for all angles in vector
				self.solve_angles(1)
				self.solve_angles(2)
				self.solve_angles(3)
				self.quadrant_assign()
				# map new vector to appropriate pixel on camera image
				self.create_triangle() 
			# When the last point has been processed
			except StopIteration:
				# Publish Data in 2D
				self.vison_fusion.publish(self.blank_image)
				self.rate.sleep()
				break

	def quadrant_assign(self):
		if (self.quad_assign[0] > 0) & (self.quad_assign[1] > 0): self.quadrant = 1
		if (self.quad_assign[0] < 0) & (self.quad_assign[1] > 0): self.quadrant = 2
		if (self.quad_assign[0] < 0) & (self.quad_assign[1] < 0): self.quadrant = 3
		if (self.quad_assign[0] > 0) & (self.quad_assign[1] < 0): self.quadrant = 4
		rospy.logdebug("Quadrant: {}".format(self.quadrant))

	def solve_angles(self, obj_vect):
		# buffer non-generic funtion used to decide which angle to solve for
		if obj_vect == 1: self.x_theta = self.find_angle(self.x_unit_vect); #print self.x_theta 
		if obj_vect == 2: self.y_theta = self.find_angle(self.y_unit_vect); #print self.y_theta 
		if obj_vect == 3: self.z_theta = self.find_angle(self.x_unit_vect); #print self.z_theta 

		rospy.logdebug("Theta-")
		rospy.logdebug("	x: {}".format(self.x_theta))
		rospy.logdebug("	y: {}".format(self.y_theta))
		rospy.logdebug("	z: {}".format(self.z_theta))

	def find_angle(self, obj_vect):
		# refer to notes above for complete steps
		# generic funntion to find angle between vector and axis unit vector
		# returns angle in radians
		numerator = numpy.dot(obj_vect, self.focus_vector)
		focus_magnitude = self.find_magnitude(self.focus_vector)
		cosine = numerator / focus_magnitude
		return math.acos(cosine)

	def create_triangle(self):
		# generic function to create a triangle from each axis to map 3D -> 2D
		if self.quadrant == 1:
			self.x_theta = int(math.tan(self.x_theta) * self.height_center) #- self.height_center  # x axis location
			self.y_theta = int(math.sin(self.y_theta) * self.width_center) #- self.width_center# y axis location
		if self.quadrant == 2:
			self.x_theta = int(math.tan(self.x_theta) * -self.height_center)# + self.height_center # x axis location
			self.y_theta = int(math.sin(self.y_theta) * self.width_center) #- self.width_center# y axis location
		if self.quadrant == 3:
			self.x_theta = int(math.tan(self.x_theta) * -self.height_center)# + self.height_center # x axis location
			self.y_theta = int(math.sin(self.y_theta) * -self.width_center) #+ self.width_center
		if self.quadrant == 4:
			self.x_theta = int(math.tan(self.x_theta) * self.height_center) #- self.height_center  # x axis location
			self.y_theta = int(math.sin(self.y_theta) * -self.width_center) #+ self.width_center

		x_final = self.x_theta - X_OFFSET
		y_final = self.y_theta + Y_OFFSET


		rospy.logdebug("Coordinates: ({}, {})".format(round(x_final, 2), round(y_final, 2)))
		# plot those points to the screen

		self.image[y_final][x_final] = 255
		self.blank_image[y_final][x_final] = 255

	def find_magnitude(self, vector):
		magnitude = math.sqrt(vector[0]*vector[0] +  vector[1]*vector[1] + vector[2]*vector[2])
		return magnitude

if __name__ == '__main__':

	rospy.init_node("vision_fusion", log_level=rospy.DEBUG)
	lidar = lidar_theta()
	rospy.spin()


