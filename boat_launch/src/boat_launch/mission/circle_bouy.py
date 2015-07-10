#! /usr/bin/env python

from __future__ import division
from std_msgs.msg import Float64, Header
from txros import util
import rospy, numpy
import boat_scripting
from uf_common.orientation_helpers import xyz_array, xyzw_array, quat_to_rotvec
import sensor_msgs.point_cloud2 as pc2
import math
from visualization_msgs.msg import MarkerArray, Marker
from geometry_msgs.msg import Vector3
from uf_common.orientation_helpers import *

BOAT_LENGTH_OFFSET = 2
FIVE_DEG = .0425 #maybe? # FIVE_DEG - .0425
PIXEL_TOLERANCE = 15


@util.cancellableInlineCallbacks
def center_buoy(boat):
    # While the boat is not locked on to the yield boat.move.left(-distance_to_move)target's orientation
    while True:
        # Get new target location - 
        # 0 is dead center
        # positive values mean right
        # negative values mean left

        bouys = yield boat.get_bouys()
        color = bouys[0].color
        msg_x = bouys[0].x_coord

        # Throws out bad readings

        if msg_x == 0: pass
            #yield boat.move.forward(.5).go()
            #print "No shape found, moving forward"
        if msg_x != 0:

            # If the pixel location is within our range of error
            if msg_x < 500 + PIXEL_TOLERANCE or msg_x < 500 - PIXEL_TOLERANCE:
                print color + shape + " in Center at location: " + str(msg_x) + " --- Locking Target"
                break
                # Break the loop and continue

            angle_move = FIVE_DEG

            # If the target is right of the center 
            if msg_x > 500 + PIXEL_TOLERANCE :
                # turn the number of degrees right between the center and the target, minus an offset
                boat.move.turn_left(-angle_move).go()
                print "Turning right", abs(angle_move)

            # If the target is left of the center
            if msg_x < 500 - PIXEL_TOLERANCE:
                # turn the number of degrees left between the center and the target, minus an offset
                boat.move.turn_left(angle_move).go()
                print "Turning left", abs(angle_move)

            print color + " bouy at pixel location: ", abs(msg_x)


@util.cancellableInlineCallbacks
def main(nh):

    boat = yield boat_scripting.get_boat(nh, False, False)
    distance = 100
    distances = []

    #yield center_buoy(boat)

    print "Bouy is now in center, moving towards it"

    while distance > 3:

        # get distance from boat
        distances = yield boat.get_distance_from_object(.05)

        # Average distance between boat and bouy
        distance = distances[0] - BOAT_LENGTH_OFFSET
        print "Distance from target", distance
        # move forward uuntil we are less than three meters from bouy
        boat.move.forward(1).go()

    print "Turning right"
    yield boat.move.turn_right_deg(45).go()
    yield util.sleep(3)

    print "Starting square"

    for i in xrange(4):
        print 'Side', i
        yield boat.move.forward(5).go()
        yield boat.move.turn_left_deg(90).go()













    

    


