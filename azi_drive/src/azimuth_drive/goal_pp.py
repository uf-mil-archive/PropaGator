#! /usr/bin/env python

import tools
import rospy
import numpy as np
from uf_common.msg import PoseTwist
from geometry_msgs.msg import Pose, Twist
from geometry_msgs.msg import Vector3
from uf_common.orientation_helpers import xyz_array

"""
    This class implements a simple non-reactive path planner
        it just outputs the current goal position there are no intermidate steps
"""

class goal_pp:
    # Initilization function
    def __init__(self):
        # Desired pose
        self.desired_position = self.current_position = np.zeros(3)
        self.desired_orientation = self.current_orientation = np.zeros(3)

    # Accept a new goal in the form of a posetwist
    def new_goal(self, goal):
        self.desired_position = tools.position_from_posetwist(goal)
        self.desired_orientation = tools.orientation_from_posetwist(goal)

        # Twist not supported
        if (xyz_array(goal.twist.linear).any() or 
            xyz_array(goal.twist.angular).any() ):
            rospy.logwarn('None zero twist are not handled by the azi goal_pp trajectory generator. Setting twist to 0')

    # Preempt a goal
    def preempt_goal(self):
        self.desired_position = self.current_position
        self.desired_orientation = self.current_orientation

    # Recieve feedback in the form of a posetwist(pt)
    def feedback(self, pt):
        pass

    # Update the trajactory and return a posetwist
    def update(self):
        # Fill up PoseTwist
        carrot = PoseTwist(
                pose = Pose(
                    position = tools.vector3_from_xyz_array(self.desired_position),
                    orientation = tools.quaternion_from_rotvec(self.desired_orientation)),

                twist = Twist(
                    linear = Vector3(),
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
