#! /usr/bin/env python

import tools
from tools import line
import rospy
import numpy as np
from uf_common.msg import PoseTwist
from geometry_msgs.msg import Pose, Twist
import math
from geometry_msgs.msg import Vector3
from uf_common.orientation_helpers import xyz_array

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
        # Desired pose
        self.desired_position = self.current_position = np.zeros(3)
        self.desired_orientation = self.current_orientation = np.zeros(3)

        # Region deffinitions
        self.orientation_radius = rospy.get_param('orientation_radius', 1)
        self.slow_down_radius = rospy.get_param('slow_down_radius', 3.0)

        # Speed parameters
        self.max_tracking_distance = rospy.get_param('max_tracking_distance', 2.0)
        self.min_tracking_distance = rospy.get_param('min_tracking_distance', 0.5)      # Half orientation radius
        self.tracking_to_speed_conv = rospy.get_param('tracking_to_speed_conv', 1.0)
        self.tracking_slope = (self.max_tracking_distance - self.min_tracking_distance) / (self.slow_down_radius - self.orientation_radius)
        self.tracking_intercept = self.min_tracking_distance - self.tracking_slope * self.orientation_radius

        # Goal paramiters
        self.distance_to_goal = 0.0

        # Initilize a line
        self.redraw_line = True
        self.line = line(np.zeros(3), np.ones(3))

    # Accept a new goal in the form of a posetwist
    def new_goal(self, goal):
        self.desired_position = tools.position_from_posetwist(goal)
        self.desired_orientation = tools.orientation_from_posetwist(goal)

        # Twist not supported
        if (xyz_array(goal.twist.linear).any() or 
            xyz_array(goal.twist.angular).any() ):
            rospy.logwarn('None zero twist are not handled by the azi point and shoot trajectory generator. Setting twist to 0')

        if np.linalg.norm(self.current_position - self.desired_position) > self.orientation_radius:
            #print 'Far out dude'
            self.line = line(self.current_position, self.desired_position)
            self.redraw_line = True
        else:
            #print 'Pretty close'
            self.line = line(self.desired_position, tools.normal_vector_from_rotvec(self.desired_orientation) + self.desired_position)
            self.redraw_line = False

    # Preempt a goal
    def preempt_goal(self):
        self.desired_position = self.current_position
        self.desired_orientation = self.current_orientation

    # Recieve feedback in the form of a posetwist(pt)
    def feedback(self, pt):
        # Update current pt
        self.current_position = tools.position_from_posetwist(pt)
        # Zero the Z
        self.current_position[2] = 0
        self.current_orientation = tools.orientation_from_posetwist(pt)

        # Get distance to the goal
        vector_to_goal = self.desired_position - self.current_position
        self.distance_to_goal = np.linalg.norm(vector_to_goal)

        # Get goal orientation
        self.angle_to_goal_orientation = map(tools.smallest_coterminal_angle, self.desired_orientation - self.current_orientation)

        # overshoot is 1 if behind line drawn perpendicular to the goal line and through the desired position, -1 if on the other
        #       Side of said line
        overshoot = np.dot(vector_to_goal / self.distance_to_goal, self.line.hat)
        self.overshoot = overshoot / abs(overshoot) 

        # In the case that overshoot is 0 or otherwise fails default to not overshoot
        if math.isnan(self.overshoot):
            self.overshoot = 1

    # Update the trajactory and return a posetwist
    def update(self):
        # Check if in the orientation radius for the first time
        if self.redraw_line and self.distance_to_goal < self.orientation_radius:
            self.redraw_line = False
            rospy.loginfo('Redrawing trajectory line')
            self.line = line(self.desired_position, tools.normal_vector_from_rotvec(self.desired_orientation) + self.desired_position)

        # Output a posetwist (carrot)
        # Project current position onto trajectory line
        Bproj = self.line.proj_pt(self.current_position)
        parallel_distance = np.linalg.norm(self.desired_position - Bproj)

        # Move carrot along line
        tracking_step = self.get_tracking_distance()
        c_pos = Bproj + self.overshoot * self.line.hat * tracking_step

        # If bproj is in threashold just set the carrot to the final position
        if parallel_distance < self.orientation_radius:
            c_pos = self.desired_position      

        # Fill up PoseTwist
        carrot = PoseTwist(
                pose = Pose(
                    position = tools.vector3_from_xyz_array(c_pos),
                    orientation = tools.quaternion_from_rotvec([0, 0, self.line.angle])),

                twist = Twist(
                    linear = Vector3(tracking_step * self.tracking_to_speed_conv * self.overshoot, 0, 0),        # Wrench Generator handles the sine of the velocity
                    angular = Vector3())
                )
        return carrot

    # Stop this path planner
    # Nothing required on stop
    def stop(self):
        pass

    # Start the trajectory generator with the current posetwist
    def start(self, current_pt):
        # Set current position and twist
        self.current_position = tools.position_from_posetwist(current_pt)
        # Zero the Z
        self.current_position[2] = 0
        self.current_orientation = tools.orientation_from_posetwist(current_pt)

        # Initlize Trajectory generator with current position as goal
        #   Set the line to be along our current orientation
        self.desired_position = self.current_position
        self.desired_orientation = self.current_orientation
        #   Make a line along the orientation
        self.line = line(self.current_position, tools.normal_vector_from_rotvec(self.current_orientation) + self.current_position)
        self.redraw_line = False

    # Get the speed setting of the trajectory
    #               Value                   :   Condition
    #               max                     :   d > slow_down
    #               min                     :   d < orientation
    #   tracking_slope * d + intercept      : orientation < d< slow_down
    def get_tracking_distance(self):
        if self.distance_to_goal < self.orientation_radius:
            return self.min_tracking_distance
        elif self.distance_to_goal > self.slow_down_radius:
            return self.max_tracking_distance
        else:
            return self.tracking_slope * self.distance_to_goal + self.tracking_intercept