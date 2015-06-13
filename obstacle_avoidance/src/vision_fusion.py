#!/usr/bin/env python

import numpy 
import cv2
import math
from sensor_msgs.msg import PointCloud2, PointField
import sensor_msgs.point_cloud2 as pc2
import rospy
from roslib import message
import pygame

DISPLAY_SIZE = (960, int(960*.56))

white = (255,255,255)
black = (0,0,0)


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

		# FOR TESTING ONLY RIGHT NOW
		# height of display to be printed to 
		# height and width will contain ueye camera dimensions eventually
		self.height  = 960
		self.width  = self.height * .56
		self.image  = numpy.zeros((self.width,self.height))
		self.screen = pygame.display.set_mode(DISPLAY_SIZE)


		# find center of camera image
		self.width_center = self.width / 2
		self.height_center = self.height / 2

		# Angles between vector and x,y,z axis
		self.x_theta = 0
		self.y_theta = 0
		self.z_theta = 0

		# starts main loop that constantly feeds from lidar
		# RIGHT NOW IT PRINTS TO PYGAME SCREEN
		rospy.Subscriber("/lidar/raw_pc", PointCloud2, self.pointcloud_callback)


	def pointcloud_callback(self, data):

		# Read from pointcloud
		pointcloud = pc2.read_points(data, field_names=("x", "y", "z"), skip_nans=False, uvs=[])
		self.screen.fill(black)

		# While there are points in the cloud to read...
		while True:
			try:
				# new point
				xyz_point = next(pointcloud) 
				# RIGHT NOW EVERYTHING IS IN RIGHT QUADRANT
				x, y, z = abs(xyz_point[1]), xyz_point[2], xyz_point[0]
				print x, y, z
				# send points into comparison vector
				self.focus_vector = numpy.array([x, y, z]) 
				# solve for all angles in vector
				self.solve_angles(1)
				self.solve_angles(2)
				self.solve_angles(3)
				# map new vector to appropriate pixel on camera image
				self.create_triangle() 
			# When the last point has been procesed
			except StopIteration:
				pygame.display.flip()
				break


	def solve_angles(self, obj_vect):
		# buffer non-generic funtion used to decide whih angle to solve for
		if obj_vect == 1: self.x_theta = self.find_angle(self.x_unit_vect)
		if obj_vect == 2: self.y_theta = self.find_angle(self.y_unit_vect)
		if obj_vect == 3: self.z_theta =self.find_angle(self.x_unit_vect)

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
		self.x_theta = math.tan(self.x_theta) * self.height_center # x axis location
		self.y_theta = math.sin(self.y_theta) * self.width_center  # y axis location
		# plot those points to the screen
		pygame.draw.circle(self.screen, 255, (int(self.x_theta), int(self.y_theta)), 2, 1)
		
	def find_magnitude(self, vector):
		magnitude = math.sqrt(vector[0]*vector[0] +  vector[1]*vector[1] + vector[2]*vector[2])
		return magnitude

if __name__ == '__main__':

	pygame.init()
	rospy.init_node("vision_fusion")
	lidar = lidar_theta()
	rospy.spin()










