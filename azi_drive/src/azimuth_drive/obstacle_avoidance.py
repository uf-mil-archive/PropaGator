#! /usr/bin/env python

# A* Shortest Path Algorithm
# http://en.wikipedia.org/wiki/A*
# FB - 201012256

import rospy

import actionlib
from uf_common.msg import PoseTwistStamped, MoveToAction, PoseTwist
from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster
from nav_msgs.msg import Odometry
from std_msgs.msg import Header
from geometry_msgs.msg import Pose, Twist, PoseStamped
from geometry_msgs.msg import Quaternion, Vector3
from sensor_msgs.msg import PointCloud2
from uf_common.orientation_helpers import xyz_array, xyzw_array, quat_to_rotvec
from uf_common.orientation_helpers import rotvec_to_quat
import numpy as np
import tools
from tools import line
import math
import threading

from heapq import heappush, heappop # for priority queue
import math
import time
import random

class hub:
    xPos = 0 # x position
    yPos = 0 # y position
    distance = 0 # total distance already travelled to reach the hub
    priority = 0 # priority = distance + remaining distance estimate
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

# A-star algorithm.
# The path returned will be a string of digits of directions.
def pathFind(the_map, n, m, dirs, dx, dy, xA, yA, xB, yB):
    closed_hubs_map = [] # map of closed (tried-out) hubs
    open_hubs_map = [] # map of open (not-yet-tried) hubs
    dir_map = [] # map of dirs
    row = [0] * n
    for i in range(m): # create 2d arrays
        closed_hubs_map.append(list(row))
        open_hubs_map.append(list(row))
        dir_map.append(list(row))

    pq = [[], []] # priority queues of open (not-yet-tried) hubs
    pqi = 0 # priority queue index
    # create the start hub and push into list of open hubs
    n0 = hub(xA, yA, 0, 0)
    n0.updatePriority(xB, yB)
    heappush(pq[pqi], n0)
    open_hubs_map[yA][xA] = n0.priority # mark it on the open hubs map

    # A* search
    while len(pq[pqi]) > 0:
        # get the current hub w/ the highest priority
        # from the list of open hubs
        n1 = pq[pqi][0] # top hub
        n0 = hub(n1.xPos, n1.yPos, n1.distance, n1.priority)
        x = n0.xPos
        y = n0.yPos
        heappop(pq[pqi]) # remove the hub from the open list
        open_hubs_map[y][x] = 0
        closed_hubs_map[y][x] = 1 # mark it on the closed hubs map

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

        # generate moves (child hubs) in all possible dirs
        for i in range(dirs):
            xdx = x + dx[i]
            ydy = y + dy[i]
            if not (xdx < 0 or xdx > n-1 or ydy < 0 or ydy > m - 1
                    or the_map[ydy][xdx] == 1 or closed_hubs_map[ydy][xdx] == 1):
                # generate a child hub
                m0 = hub(xdx, ydy, n0.distance, n0.priority)
                m0.nextMove(dirs, i)
                m0.updatePriority(xB, yB)
                # if it is not in the open list then add into that
                if open_hubs_map[ydy][xdx] == 0:
                    open_hubs_map[ydy][xdx] = m0.priority
                    heappush(pq[pqi], m0)
                    # mark its parent hub direction
                    dir_map[ydy][xdx] = (i + dirs / 2) % dirs
                elif open_hubs_map[ydy][xdx] > m0.priority:
                    # update the priority
                    open_hubs_map[ydy][xdx] = m0.priority
                    # update the parent direction
                    dir_map[ydy][xdx] = (i + dirs / 2) % dirs
                    # replace the hub
                    # by emptying one pq to the other one
                    # except the hub to be replaced will be ignored
                    # and the new hub will be pushed in instead
                    while not (pq[pqi][0].xPos == xdx and pq[pqi][0].yPos == ydy):
                        heappush(pq[1 - pqi], pq[pqi][0])
                        heappop(pq[pqi])
                    heappop(pq[pqi]) # remove the target hub
                    # empty the larger size priority queue to the smaller one
                    if len(pq[pqi]) > len(pq[1 - pqi]):
                        pqi = 1 - pqi
                    while len(pq[pqi]) > 0:
                        heappush(pq[1-pqi], pq[pqi][0])
                        heappop(pq[pqi])       
                    pqi = 1 - pqi
                    heappush(pq[pqi], m0) # add the better hub instead
    return '' # if no route found
class a_star_path_planner:
    def __init__(self, name):

        # Thread Locking
        self.lock = threading.Lock()
        #self.lock.acquire()
        #self.lock.release()

        # Desired pose
        self.desired_position = self.current_position = np.zeros(3)
        self.desired_orientation = self.current_orientation = np.zeros(3)
        #self.desired_twist = self.current_twist = Twist()

        # Goal tolerances before seting succeded
        self.linear_tolerance = rospy.get_param('linear_tolerance', 0.5)
        self.angular_tolerance = rospy.get_param('angular_tolerance', 20 * np.pi / 180)
        self.orientation_radius = rospy.get_param('orientation_radius', 1)
        self.slow_down_radius = rospy.get_param('slow_down_radius', 3.0)

        # Speed parameters
        self.max_tracking_distance = rospy.get_param('max_tracking_distance', 2.0)
        self.min_tracking_distance = rospy.get_param('min_tracking_distance', 0.0)
        self.tracking_to_speed_conv = rospy.get_param('tracking_to_speed_conv', 1.0)
        self.tracking_slope = (self.max_tracking_distance - self.min_tracking_distance) / (self.slow_down_radius - self.orientation_radius)
        self.tracking_intercept = self.min_tracking_distance - self.tracking_slope * self.orientation_radius

        # Publishers
        self.traj_pub = rospy.Publisher('/trajectory', PoseTwistStamped, queue_size = 10)
        #self.traj_debug_pub = rospy.Publisher('/trajectory_debug', PoseStamped, queue_size = 10)
        self.pc_sub = rospy.Subscriber('/lidar/raw_pc', PointCloud2, self.odom_cb, queue_size = 10)
        # Set desired twist to 0
        #self.desired_twist.linear.x = self.desired_twist.linear.y = self.desired_twist.linear.z = 0
        #self.desired_twist.angular.x = self.desired_twist.angular.y = self.desired_twist.angular.z = 0

        # Initilize a line
        self.line = line(np.zeros(3), np.ones(3))

        # Wait for current position and set as desired position
        rospy.loginfo('Waiting for /odom')
        self.odom_sub = rospy.Subscriber('/odom', Odometry, self.odom_cb, queue_size = 10)
        while not self.current_position.any():  # Will be 0 until an odom publishes (if its still 0 it will drift very very soon)
            pass

        # Initlize Trajectory generator with current position as goal
        #   Set the line to be along our current orientation
        self.desired_position = self.current_position
        self.desired_orientation = self.current_orientation
        #   Make a line along the orientation
        self.line = line(self.current_position, tools.normal_vector_from_rotvec(self.current_orientation) + self.current_position)
        self.redraw_line = False
        rospy.loginfo('Got current pose from /odom')

        # Kill handling
        self.killed = False
        self.kill_listener = KillListener(self.set_kill, self.clear_kill)
        self.kill_broadcaster = KillBroadcaster(id=name, description='Tank steer trajectory_generator shutdown')
        try:
            self.kill_broadcaster.clear()           # In case previously killed
        except rospy.service.ServiceException, e:
            rospy.logwarn(str(e))
        rospy.on_shutdown(self.on_shutdown)

        # RC handling

        # Start the main update loop
        rospy.Timer(rospy.Duration(0.1), self.update)

        # Implement the moveto action server
        self.moveto_as = actionlib.SimpleActionServer('moveto', MoveToAction, None, False)
        self.moveto_as.register_goal_callback(self.new_goal)
        self.moveto_as.register_preempt_callback(self.goal_preempt)
        self.moveto_as.start()    
    
    # Update pose and twist
    def odom_cb(self, msg):
        # Lock on odom cb
        self.lock.acquire()

        self.current_position = tools.position_from_pose(msg.pose.pose)
        # Zero the Z
        self.current_position[2] = 0
        self.current_orientation = tools.orientation_from_pose(msg.pose.pose)
        # Get distance to the goal
        vector_to_goal = self.desired_position - self.current_position
        self.distance_to_goal = np.linalg.norm(vector_to_goal)
        self.angle_to_goal_orientation = map(smallest_coterminal_angle, self.desired_orientation - self.current_orientation)
        # overshoot is 1 if behind line drawn perpendicular to the goal line and through the desired position, -1 if on the other
        #       Side of said line
        #print 'Distance to goal: ', self.distance_to_goal
        #print 'line.hat: ', self.line.hat
        #print 'Vector to goal: ', vector_to_goal
        
        overshoot = np.dot(vector_to_goal / self.distance_to_goal, self.line.hat)

        self.overshoot = overshoot / abs(overshoot) 

        if math.isnan(self.overshoot):
            self.overshoot = 1

        #print 'Overshoot: ', self.overshoot

        # Release lock
        self.lock.release()

    def main():#current_x_y_enu, goal_x_y_enu, lidar_points_x_y_enu
        # MAIN
        dirs = 8 # number of possible directions to move on the map
        if dirs == 4:
            dx = [1, 0, -1, 0]
            dy = [0, 1, 0, -1]
        elif dirs == 8:
            dx = [1, 1, 0, -1, -1, -1, 0, 1]
            dy = [0, 1, 1, 1, 0, -1, -1, -1]
        
        odom_sub = rospy.Subscriber('/odom', Odometry, self.odom_cb, queue_size = 10)
        (xA, yA, zA, xB, yB, zB) = (math.trunc(self.current_position), math.trunc(self.desired_position))

        n = (100, xA) # horizontal size of the map
        m = (100, yA) # vertical size of the map
        the_map = []
        row = ([0] * n[0],[n[1]] * n[0])
        for len(row):
            row
        print row
        for i in range(m[0]): # create empty map
            the_map.append(list(row))


        '''
        # randomly select start and finish locations from a list
        sf = []
        sf.append((0, 0, n - 1, m - 1))
        sf.append((0, m - 1, n - 1, 0))
        sf.append((n / 2 - 1, m / 2 - 1, n / 2 + 1, m / 2 + 1))
        sf.append((n / 2 - 1, m / 2 + 1, n / 2 + 1, m / 2 - 1))
        sf.append((n / 2 - 1, 0, n / 2 + 1, m - 1))
        sf.append((n / 2 + 1, m - 1, n / 2 - 1, 0))
        sf.append((0, m / 2 - 1, n - 1, m / 2 + 1))
        sf.append((n - 1, m / 2 + 1, 0, m / 2 - 1))
        (xA, yA, xB, yB) = random.choice(sf)
        '''



        # fillout the map with a '+' pattern
        # mark hubs as occupied using lidar points
        
        '''
        for x in range(n / 8, n * 7 / 8):
            the_map[m / 2][x] = 1
        for y in range(m/8, m * 7 / 8):
            the_map[y][n / 2] = 1

        '''


        print 'Map size (X,Y): ', n, m
        print 'Start: ', xA, yA
        print 'Finish: ', xB, yB
        t = time.time()
        route = pathFind(the_map, n, m, dirs, dx, dy, xA, yA, xB, yB)
        print 'Time to generate the route (seconds): ', time.time() - t
        print 'Route:'
        print route

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
if __name__ == '__main__':
    rospy.init_node('a_star_path_planner')
    node = a_star_path_planner(rospy.get_name())
    rospy.spin()