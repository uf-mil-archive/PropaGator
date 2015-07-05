#! /usr/bin/env python

from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy
import sensor_msgs.point_cloud2 as pc2
import math

FIVE_DEG - .0425
TEN_DEG - .085

LEFT_TURN = .0425
RIGHT_TURN = -.0425
PIXEL_TOLERANCE = 35
ANGLE_OFFSET = 5
DISPLAY_HEIGHT = 640
DEFAULT_SHAPE = 'circle'
BOAT_LENGTH_OFFSET = 2
MOVE_OFFSET = 5

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
    count = 1

    # While the boat is still distant from the target
    while True and not rospy.is_shutdown() and count < 3:

        temp_distance = 0
        avg_distance = 0
        shortest_distance = 100
        farthest_distance = 0

        # While the boat is not locked on to the target's orientation
        while True and shortest_distance > 3:
            # Get new target location - 
            # 0 is dead center
            # positive values mean right
            # negative values mean left
            msg =  yield boat.get_shape_location(shape)

            # Throws out bad readings

            if msg == 0: pass
                #yield boat.move.forward(.5).go()
                #print "No shape found, moving forward"
            if msg != 0:

                # If the pixel location is within our range of error
                if abs(msg) < PIXEL_TOLERANCE:
                    print "circle in Center at location: " + str(msg) + " --- Locking Target"
                    break
                    # Break the loop and continue

                angle_move = FIVE_DEG 

                # If the target is right of the center 
                if msg > 0:
                    # turn the number of degrees right between the center and the target, minus an offset
                    boat.move.turn_left(-angle_move).go()
                    print "Turning right", abs(angle_move)

                # If the target is left of the center
                if msg < 0:
                    # turn the number of degrees left between the center and the target, minus an offset
                    boat.move.turn_left(angle_move).go()
                    print "Turning left", abs(angle_move)

                print shape + " at pixel location: ", abs(msg)

        print "Holding at current position"
        print "Scanning lidar"

        hold = []

        # loops until lidar gets a good filtered lidar reading
        while len(hold) <= 0:
            # get pointcloud
            pointcloud = yield boat.get_pointcloud()
            yield util.sleep(.1) # sleep to avoid tooPast errors
            pointcloud_base = yield boat.to_baselink(pointcloud)
            yield util.sleep(.1) # sleep to avoid tooPast errors

            # Filter lidar data to only data right in front of the boat
            print "Filtering lidar data"
            hold = filter(lambda x: abs(x[1]) < .05, pointcloud_base)

        # Calculate several distances between target and boat
        for x in range(len(hold)):
            dist = hold[x]
            temp_distance += dist[0]
            print dist
            # Check and assign the closest object to the boat
            if dist[0] < shortest_distance: shortest_distance = dist[0]
            if dist[0] > farthest_distance: farthest_distance = dist[0]

        avg_distance = temp_distance/len(hold) - BOAT_LENGTH_OFFSET
        shortest_distance = shortest_distance- BOAT_LENGTH_OFFSET
        farthest_distance = farthest_distance - BOAT_LENGTH_OFFSET

        print "Average distance from target:", avg_distance 
        print "Shortest distance between boat and object:", shortest_distance 
        print "Farther distance between boat and object:", farthest_distance 

        final_move = shortest_distance/MOVE_OFFSET

        if farthest_distance > 3.5: 
            #raw_input("Press enter to move forward" + str(final_move) + " meters")
            # Print only if a move is commanded
            print "Moving forward " + str(final_move) + " meters"
            yield boat.move.forward(final_move).go()
        if farthest_distance <= 2: 
            #raw_input("Press enter to move forward one meter")
            # Print only if a move is commanded
            print "Moving forward one meter"
            yield boat.move.forward(1.5).go()
            yield util.sleep(3)
            yield boat.move.forward(-5).go()
            print 'Find shape success'
            count +=1

        # delete variables to avoid any threading problems
        del avg_distance, temp_distance, farthest_distance, shortest_distance