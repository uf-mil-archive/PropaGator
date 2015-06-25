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
import sensor_msgs.point_cloud2 as pc2
from sensor_msgs.msg import PointCloud2
import tf

from heapq import heappush, heappop # for priority queue
import math
import time

"""
    This class implements a simple reactive path planner
        The generation is as follows:
            

"""

class a_star_rpp:
    # Initilization function
    def __init__(self):
        # Desired pose
        self.desired_position = self.current_position = np.zeros(3)
        self.desired_orientation = self.current_orientation = np.zeros(3)

        # Region deffinitions
        self.orientation_radius = rospy.get_param('orientation_radius', 1)
        self.raw_pc_sub = rospy.Subscriber("/lidar/raw_pc", PointCloud2, self.pointcloud_callback)


    # Accept a new goal in the form of a posetwist
    def new_goal(self, goal):
        self.desired_position = tools.position_from_posetwist(goal)
        self.desired_orientation = tools.orientation_from_posetwist(goal)

        # Twist not supported
        if (xyz_array(goal.twist.linear).any() or 
            xyz_array(goal.twist.angular).any() ):
            rospy.logwarn('None zero twist are not handled by the azi a_star_rpp trajectory generator. Setting twist to 0')

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
        #vector_to_goal = self.desired_position - self.current_position
        #self.distance_to_goal = np.linalg.norm(vector_to_goal)

    # Update the trajactory and return a posetwist
    def update(self):
        '''
        # Check if in the orientation radius for the first time
        if self.redraw_line and self.distance_to_goal < self.orientation_radius:
            self.redraw_line = False
            rospy.loginfo('Redrawing trajectory line')
            self.line = line(self.desired_position, tools.normal_vector_from_rotvec(self.desired_orientation) + self.desired_position)
        '''
        # Output a posetwist (carrot)
        # Project current position onto trajectory line
        #Bproj = self.line.proj_pt(self.current_position)
        distance = np.linalg.norm(self.desired_position - self.current_position)


        # Move carrot along line
        #tracking_step = self.get_tracking_distance()
        print "self.map_builder()" 
        print self.map_builder()
        c_pos = self.current_position # + [1,0,0]
        print 'c_pos1',c_pos
        velocity = 1
        # If bproj is in threashold just set the carrot to the final position
        if distance < 0.5:
            c_pos = self.desired_position
            velocity = 0
        print 'c_pos2',c_pos

        # Fill up PoseTwist
        carrot = PoseTwist(
                pose = Pose(
                    position = tools.vector3_from_xyz_array(c_pos),
                    orientation = tools.quaternion_from_rotvec([0, 0, self.line.angle])),

                twist = Twist(
                    linear = Vector3(velocity, 0, 0),        # Wrench Generator handles the sine of the velocity
                    angular = Vector3())
                )
        print 'carrot', carrot
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

    def pointcloud_callback(self, msg):
        pointcloud = pc2.read_points(msg, field_names=("x", "y", "z"), skip_nans=False, uvs=[])
        # While there are points in the cloud to read...
        while True:
            try:
                # new point
                xyz_point = next(pointcloud) 
                print xyz_point
            # When the last point has been processed
            except StopIteration:
                break

    def map_builder(self):
        # MAIN

        dirs = 8 # number of possible directions to move on the map
        if dirs == 4:
            dx = [1, 0, -1, 0]
            dy = [0, 1, 0, -1]
        elif dirs == 8:
            dx = [1, 1, 0, -1, -1, -1, 0, 1]
            dy = [0, 1, 1, 1, 0, -1, -1, -1]

        n = 20# horizontal size of the map
        m = 20# vertical size of the map
        the_map = []
        row = [0] * n
        for i in range(m): # create empty map
            the_map.append(list(row))

        # fillout the map with a '+' pattern
        for x in range(n / 8, n * 7 / 8):
            the_map[m / 2][x] = 1
        for y in range(m/8, m * 7 / 8):
            the_map[y][n / 2] = 1


        (xA, yA, xB, yB) = [0, int(round(n/2)), abs(int(round(self.desired_position[0]-self.current_position[0]))), int(round(self.desired_position[1]-self.current_position[1] + n/2))]
        '''
        print 'Map size (X,Y): ', n, m
        print 'Start: ', xA, yA
        print 'Finish: ', xB, yB
        t = time.time()
        '''
        route = self.pathFind(the_map, n, m, dirs, dx, dy, xA, yA, xB, yB)
        print 'Route: ', '(', route, ')'
        return route
        
        
        # mark the route on the map
        if len(route) > 0:
            x = xA
            y = yA
            the_map[y][x] = 2
            for i in range(len(route)):
                j = int(route[i])
                x += dx[j]
                y += dy[j]
                the_map[y][x] = 3
            the_map[y][x] = 4

        # display the map with the route added
        print 'Map:'
        for y in range(m):
            for x in range(n):
                xy = the_map[y][x]
                if xy == 0:
                    print '.', # space
                elif xy == 1:
                    print 'O', # obstacle
                elif xy == 2:
                    print 'S', # start
                elif xy == 3:
                    print 'R', # route
                elif xy == 4:
                    print 'F', # finish
            print

        raw_input('Press Enter...')
        
    # A-star algorithm.
    # The path returned will be a string of digits of directions.
    def pathFind(self, the_map, n, m, dirs, dx, dy, xA, yA, xB, yB):
        print "xA", xA
        print "xB", xB
        print "yA", yA
        print "yB", yB

        closed_nodes_map = [] # map of closed (tried-out) nodes
        open_nodes_map = [] # map of open (not-yet-tried) nodes
        dir_map = [] # map of dirs
        row = [0] * n
        #print 'row,' ,row
        for i in range(m): # create 2d arrays
            closed_nodes_map.append(list(row))
            open_nodes_map.append(list(row))
            dir_map.append(list(row))
        #print dir_map
        pq = [[], []] # priority queues of open (not-yet-tried) nodes
        pqi = 0 # priority queue index
        # create the start node and push into list of open nodes
        n0 = node(xA, yA, 0, 0)
        #print 'n0', n0
        n0.updatePriority(xB, yB)
        #print n0.updatePriority(xB, yB)
        heappush(pq[pqi], n0)
        open_nodes_map[yA][xA] = n0.priority # mark it on the open nodes map

        # A* search
        while len(pq[pqi]) > 0:
            # get the current node w/ the highest priority
            # from the list of open nodes
            n1 = pq[pqi][0] # top node
            n0 = node(n1.xPos, n1.yPos, n1.distance, n1.priority)
            x = n0.xPos
            y = n0.yPos
            heappop(pq[pqi]) # remove the node from the open list
            open_nodes_map[y][x] = 0
            closed_nodes_map[y][x] = 1 # mark it on the closed nodes map

            # quit searching when the goal is reached
            # if n0.estimate(xB, yB) == 0:
            if x == xB and y == yB:
                # generate the path from finish to start
                # by following the dirs
                path = ''
                while not (x == xA and y == yA):
                    j = dir_map[y][x]
                    c = str((j + dirs / 2) % dirs)
                    path = c + path
                    x += dx[j]
                    y += dy[j]
                return path

            # generate moves (child nodes) in all possible dirs
            for i in range(dirs):
                xdx = x + dx[i]
                ydy = y + dy[i]
                if not (xdx < 0 or xdx > n-1 or ydy < 0 or ydy > m - 1
                        or the_map[ydy][xdx] == 1 or closed_nodes_map[ydy][xdx] == 1):
                    # generate a child node
                    m0 = node(xdx, ydy, n0.distance, n0.priority)
                    m0.nextMove(dirs, i)
                    m0.updatePriority(xB, yB)
                    # if it is not in the open list then add into that
                    if open_nodes_map[ydy][xdx] == 0:
                        open_nodes_map[ydy][xdx] = m0.priority
                        heappush(pq[pqi], m0)
                        # mark its parent node direction
                        dir_map[ydy][xdx] = (i + dirs / 2) % dirs
                    elif open_nodes_map[ydy][xdx] > m0.priority:
                        # update the priority
                        open_nodes_map[ydy][xdx] = m0.priority
                        # update the parent direction
                        dir_map[ydy][xdx] = (i + dirs / 2) % dirs
                        # replace the node
                        # by emptying one pq to the other one
                        # except the node to be replaced will be ignored
                        # and the new node will be pushed in instead
                        while not (pq[pqi][0].xPos == xdx and pq[pqi][0].yPos == ydy):
                            heappush(pq[1 - pqi], pq[pqi][0])
                            heappop(pq[pqi])
                        heappop(pq[pqi]) # remove the target node
                        # empty the larger size priority queue to the smaller one
                        if len(pq[pqi]) > len(pq[1 - pqi]):
                            pqi = 1 - pqi
                        while len(pq[pqi]) > 0:
                            heappush(pq[1-pqi], pq[pqi][0])
                            heappop(pq[pqi])       
                        pqi = 1 - pqi
                        heappush(pq[pqi], m0) # add the better node instead
        return '' # if no route found



class node:
    def __init__(self, xPos, yPos, distance, priority):
        self.xPos = xPos
        self.yPos = yPos
        self.distance = distance
        self.priority = priority
    def __lt__(self, other): # comparison method for priority queue
        return self.priority < other.priority
    def updatePriority(self, xDest, yDest):
        self.priority = self.distance + self.estimate(xDest, yDest) * 10 # A*
    # give higher priority to going straight instead of diagonally
    def nextMove(self, dirs, d): # d: direction to move
        if dirs == 8 and d % 2 != 0:
            self.distance += 14
        else:
            self.distance += 10
    # Estimation function for the remaining distance to the goal.
    def estimate(self, xDest, yDest):
        xd = xDest - self.xPos
        yd = yDest - self.yPos
        # Euclidian Distance
        d = math.sqrt(xd * xd + yd * yd)
        # Manhattan distance
        # d = abs(xd) + abs(yd)
        # Chebyshev distance
        # d = max(abs(xd), abs(yd))
        return(d)
