#! /usr/bin/env python

from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy
import sensor_msgs.point_cloud2 as pc2
import math

# 5 degrees - .0425
# 10 degrees - .085

LEFT_TURN = .0425
RIGHT_TURN = -.0425
PIXEL_TOLERANCE = 20
ANGLE_OFFSET = 20
DISPLAY_HEIGHT = 640
BASE_OFFSET = 1.02

DEFAULT_SHAPE = 'cross'

def calc_angle(opp_input):
    adjacent = DISPLAY_HEIGHT / 2
    opposite = opp_input
    hypotenuse = math.sqrt(adjacent*adjacent + opposite*opposite)
    arcsin = math.asin(opposite/hypotenuse)
    return arcsin

@util.cancellableInlineCallbacks
def main(nh, shape=None):

    if shape == None:
        shape = DEFAULT_SHAPE

    boat = yield boat_scripting.get_boat(nh, False, False)

    # While the boat is still distant from the target
    while True and not rospy.is_shutdown():

        

        temp_distance = 0
        avg_distance = 0
        shortest_distance = 100
        farthest_distance = 0
        
        
        # While the boat is not locked on to the target's orientation
        while True:
            # Get new target location - 
            # 0 is dead center
            # positive values mean right
            # negative values mean left
            msg =  yield boat.get_shape_location(shape)

            # If the pixel location is within our range of error
            if msg != 0:
                if abs(msg) < PIXEL_TOLERANCE:
                    print shape + " in Center at location: " + str(msg) + " --- Locking Target"
                    break
                    # Break the loop and continue

                angle_move = -.02 #calc_angle(msg) - ANGLE_OFFSET

                # If the target is right of the center 
                if msg > 0:
                    # turn the number of degrees right between the center and the target, minus an offset
                    yield boat.move.turn_left(angle_move).go()

                # If the target is left of the center
                if msg < 0:
                    # turn the number of degrees left between the center and the target, minus an offset
                    yield boat.move.turn_left(-angle_move).go()

                print shape + " at pixel location: ", abs(msg) 

        

        # Target is now in center, moving onto to lidar distance sensing
        
        print "Holding at current position"
        print "Scanning lidar"

        ''

        hold = ()

        # loops until distance is greater than 0 -- throws out bad readings
        while len(hold) <= 0:
            # get 
            pointcloud = yield boat.get_pointcloud()
            yield util.sleep(.01) # sleep to avoid tooPast errors
            pointcloud_base = yield boat.to_baselink(pointcloud)
            # sleep to avoid tooPast errors

            # Filter lidar data to only data right in front of the boat
            print "Filtering lidar data"
            hold = filter(lambda x: abs(x[1]) < .05, pointcloud_base)
            hold = filter(lambda x: abs(x[2]) < 2, hold)
            yield util.sleep(.01)


            # Calculate several distances between target and boat

        for x in hold:
            dist = hold[0]
            temp_distance += dist[0]
            print dist.any(0)
            # Check and assign the closest object to the boat
            if dist[0] < shortest_distance: shortest_distance = dist[0]
            if dist[0] > farthest_distance: farthest_distance = dist[0]

        avg_distance = temp_distance/len(hold)

        print "Average distance from target:", avg_distance - BASE_OFFSET
        print "Shortest distance between boat and object:", shortest_distance -BASE_OFFSET
        print "Farther distance between boat and object:", farthest_distance - BASE_OFFSET


        # When we get a good reading

        

        # If the boat is 5 or more meters away, command larger move -- avg_distance
        # If the boat is less than 5 meters away, command smaller moves - smallest_distance
        # If boat is less than or equal to 1 meters away, end program

        
        if farthest_distance >= 5: yield boat.move.forward(avg_distance/2).go()
        if farthest_distance > 1 and farthest_distance < 5: yield boat.move.forward(shortest_distance).go()
        if farthest_distance <= 1: 
            yield boat.move.forward(-6).go()
            print 'Find shape success'
            break

        # Print only if a move is commanded
        print "Moving forward " + shortest_distance + " meters"
        # delete variables to avoid any threading problems
        del avg_distance, temp_distance, farthest_distance, shortest_distance
        
        yield util.sleep(.5)

        
        
        












