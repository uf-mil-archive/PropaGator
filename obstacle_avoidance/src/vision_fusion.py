#!/usr/bin/env python

import numpy 
import cv2
import math
from sensor_msgs.msg import PointCloud2, PointField
import sensor_msgs.point_cloud2 as pc2
import rospy
from roslib import message


desired_x = 960
desired_y = 540

x_compare_vect = numpy.array([1,0,0])
y_compare_vect = numpy.array([0,1,0])
z_compare_vect = numpy.array([0,0,1])


'''

Vector notation: [x y z]

'''

class lidar_theta(object):

	def __init__(self):
		self.x_center_vector = x_compare_vect
		self.y_center_vector = y_compare_vect
		self.z_center_vector = z_compare_vect

		self.focus_vector = numpy.array([1,1,1])

		self.x = 0
		self.y = 0
		self.z = 0

		self.angle_in_radians = 0
		self.angle_in_degrees = 0

		rospy.Subscriber("/lidar/raw_pc", PointCloud2, self.pointcloud_callback)

	def solve_angles(self, obj_vect):
		if obj_vect == 1: self.find_angle(self.x_center_vector)
		if obj_vect == 2: self.find_angle(self.y_center_vector)
		if obj_vect == 3: self.find_angle(self.z_center_vector)

	def find_angle(self, obj_vect):

		numerator = numpy.dot(obj_vect, self.focus_vector)
		center_magnitude = self.find_magnitude(obj_vect)
		focus_magnitude = self.find_magnitude(self.focus_vector)
		cosine = numerator / (center_magnitude * focus_magnitude)

		self.angle_in_radians = math.acos(cosine)
		self.angle_in_degrees = self.angle_in_radians * 180 / numpy.pi

		print self.angle_in_degrees


	def find_magnitude(self, vector):
		magnitude = math.sqrt(vector[0]*vector[0] + 
							  vector[1]*vector[1] + 
							  vector[2]*vector[2]
							  )

		return magnitude

	def pointcloud_callback(self, data) :

	    data_out = pc2.read_points(data, field_names=("x", "y", "z"), skip_nans=False, uvs=[])
	    int_data = next(data_out)
	    self.x = int_data[0]
	    self.y =  int_data[2]
	    self.z = int_data[1]
	    self.focus_vector = numpy.array([self.x, self.y, self.z])
	    # y = 0
	    # 
	    #
	    self.solve_angles(1)
	    self.solve_angles(2)
	    self.solve_angles(3)

	    print "x: ", self.x
	    print "y: ", self.y
	    print "z: ", self.z



	def callback(self, data):
		gen = pc2.read_points(data, skip_nans=True, field_names=("x", "y", "z"))
		self.y =  int (data.height / 2)
		self.x = int (data.width / 2)
		self.y = read_depth (middle_x, height, data)
		# do stuff with middle


		

class theta_return(object):
	def __init__(self):
		self.img = cv2.imread('/home/zachgoins/Desktop/test.jpg', 0)

		self.height  = self.img.shape[0]
		self.width  = self.img.shape[1]

		self.width_center = self.width / 2
		self.height_center = self.height / 2

		self.desired_x_test_pixel = desired_x
		self.desired_y_test_pixel = desired_y

		self.x = 0
		self.y = 0
		self.h = 0

		self.theta = 0
		self.theta_in_degrees = 0

	def create_triangle(self, variable):
		if variable == 'x': self.x = abs(self.width_center - self.desired_x_test_pixel)
		if variable == 'y': self.x = abs(self.height_center - self.desired_y_test_pixel)
		self.y = self.height 
		self.h = math.sqrt(self.x*self.x + self.y*self.y)

	def solve_x_theta(self):
		self.create_triangle('x')
		self.theta = math.acos(self.y / self.h)
		self.theta_in_degrees = self.theta * 180/ numpy.pi
		print "X theta in Degress: ", self.theta_in_degrees
		print "X theta in Radians: ", self.theta

	def solve_y_theta(self):
		self.create_triangle('y')
		self.theta = math.acos(self.y / self.h)
		self.theta_in_degrees = self.theta * 180/ numpy.pi
		print "Y theta in Degress: ", self.theta_in_degrees
		print "Y theta in Radians: ", self.theta



if __name__ == '__main__':

	rospy.init_node("test")
	lidar = lidar_theta()
	lidar.solve_angles(1)

	lidar.solve_angles(2)

	lidar.solve_angles(3)

	laser_projector = PointCloud2()

	rospy.spin()
	'''
	 theta = theta_return()
	 theta.solve_x_theta()
	 theta.solve_y_theta()
	'''










