#!/usr/bin/env python

import numpy 
import cv2
import math

desired_x = 960
desired_y = 540

'''

Vector notation: [x y z]

'''

class lidar_theta(object):
	def __init__(self):
		self.center_vector = numpy.array([0,0,1])
		self.focus_vector = numpy.array([1,1,1])
		self.angle_in_radians = 0
		self.angle_in_degrees = 0

	def find_angle(self):
		numerator = numpy.dot(self.center_vector, self.focus_vector)
		center_magnitude = self.find_magnitude(self.center_vector)
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

	lidar = lidar_theta()
	lidar.find_angle()

	'''
	 theta = theta_return()
	 theta.solve_x_theta()
	 theta.solve_y_theta()
	'''










