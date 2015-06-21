#! /usr/bin/env python

import tools

"""
	This class implements a simple non-reactive path planner
		The generation is as follows:
			* Draw strait line to goal
			* Project current position onto goal line
			* Add a step towards the goal to the projection onto the goal line 
				* The step is a picewise function of distance to goal
			* If close enough to goal the line is redrawn such that the orientation 
				matches the final desired orientation
"""

class point_shoot_pp:
	# Initilization function
	def __init__(self):
		pass

	# Accept a new goal in the form of a pose twist
	def new_goal(self, goal):
		pass

	# Preempt a goal
	def preempt_goal(self):
		pass

	# Recieve feedback in the form of a posetwist(pt)
	def feedback(self, pt):
		pass

	# Update the trajactory and return a posetwist
	def update(self):
		pass

	# Stop this path planner
	def stop(self):
		pass

	def start(self):
		pass