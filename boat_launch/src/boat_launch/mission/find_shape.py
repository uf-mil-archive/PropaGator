#! /usr/bin/env python

from __future__ import division
from txros import action, util, tf
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy
import sensor_msgs.point_cloud2 as pc2
import math
from uf_common.orientation_helpers import xyz_array, xyzw_array, quat_to_rotvec
import numpy
from geometry_msgs.msg import Vector3

FIVE_DEG = .0425 #maybe? # FIVE_DEG - .0425
TEN_DEG = .1 #maybe? # TEN_DEG - .085

LEFT_TURN = .0425
RIGHT_TURN = -.0425
PIXEL_TOLERANCE = 40
ANGLE_OFFSET = 5
DISPLAY_HEIGHT = 640
DEFAULT_SHAPE = 'circle'
DEFAULT_COLOR = 'black'
BOAT_LENGTH_OFFSET = 2
MOVE_OFFSET = 5


REORIENT = False

def to_vector(x, y):
    return Vector3(x ,y , 0) 


def calc_angle(opp_input):
    adjacent = DISPLAY_HEIGHT / 2
    opposite = opp_input
    hypotenuse = math.sqrt(adjacent*adjacent + opposite*opposite)
    arcsin = math.asin(opposite/hypotenuse)
    return arcsin


@util.cancellableInlineCallbacks
def orient(boat):

    angle_to_move = 0
    final_cloud = []
    x_mean = 0
    y_mean = 0
    numerator = 0
    denom = 0
    pointcloud_base = []

    # loops until lidar gets a good filtered lidar reading
    while len(final_cloud) <= 0:
        pointcloud = yield boat.get_pointcloud()
        yield util.sleep(.1) # sleep to avoid tooPast errors
        pointcloud_base = yield boat.to_baselink(pointcloud)
        yield util.sleep(.1) # sleep to avoid tooPast errors

        pointcloud_enu = []

        # Append pointcloud data to enu 
        for p in pc2.read_points(pointcloud, field_names=("x", "y", "z"), skip_nans=False, uvs=[]):
            pointcloud_enu.append((p[0], p[1], p[2]))
        yield util.sleep(.1) # sleep to avoid tooPast errors

        final_cloud = []

        # Filter lidar data to only data right in front of the boat
        # Since both pointcloud have the sane index for the same points, 
        # we then use the baselink pointcloud data to fiter enu points in front of the boat

        print "Filtering lidar data"
        for i in range(0, len(pointcloud_base)):
            temp = pointcloud_base[i]
            if abs(temp[1]) < .3:
                final_cloud.append(pointcloud_enu[i])

    # Use final point cloud to get x and y mean to be used in line generation
    for i in final_cloud:
        x_mean += i[0]
        y_mean += i[1]

    # Find x-mean and y-mean
    y_mean = y_mean/len(final_cloud)
    x_mean = x_mean/len(final_cloud)

    for i in final_cloud:
        numerator += (i[0] - x_mean)*(i[1] - y_mean)
        denom += (i[0] - x_mean)*(i[0] - x_mean)

    if denom == 0: denom = 1
    m = numerator/denom
    b = y_mean - (m * x_mean)

    x1 = 1
    x2 = 100

    # Vector we want to be parallel to 
    parallel_vector = m*1 + b

    # Gets heading of the boat as unit vector
    position = boat.odom.position[0:2]
    yaw = quat_to_rotvec(boat.odom.orientation)[2]
    heading = numpy.array([numpy.cos(yaw), numpy.sin(yaw)])
    
    # Vector to be parrallel to 
    vec1 = numpy.array([x1, parallel_vector, 0])
    # Vector of the boat
    vec2 = numpy.array([(heading[0]), (heading[1]), 0])

    angle_to_move = 2
    move = 0
    angle_to_move = 1

    # Finds angle between boat unit vector and dock
    numerator = numpy.dot(vec1, vec2)
    one = numpy.linalg.norm(vec1)
    two =  numpy.linalg.norm(vec2)
    cosine = numerator / (one * two)
    # Angle betwen the boat and the shore
    angle_to_move = math.acos(cosine)

    # Scan lidar to get the distances from the shore to the boat for use in creating triangle
    distances = yield boat.get_distance_from_object(.05)

    # Caluculate distance to move to be directly in front of point
    theda_one = .9 - angle_to_move
    L1 = distances[0]
    distance_to_move = math.sin(theda_one) * L1

    if vec2[0] < 0:
        #print "Yawing left ", angle_to_move
        print "Moving right ", distance_to_move 
        #boat.move.turn_left(angle_to_move).go()
        left = boat.move.left(-distance_to_move-2).go()
        forward = boat.move.forward(distance_to_move -3).go()
        yield left, forward

    if vec2[0] > 0:
        #print "Yawing right ", angle_to_move
        print "Moving left ", distance_to_move 
        #boat.move.turn_left(angle_to_move).go()
        left = boat.move.left(distance_to_move-2).go()
        forward = boat.move.forward(distance_to_move -3).go()
        yield left, forward

    #defer.returnValue(True)

@util.cancellableInlineCallbacks
def center_sign(boat, shape, color):

    print "Attempting to find", shape
    # While the boat is not locked on to the yield boat.move.left(-distance_to_move)target's orientation
    while True:
        # Get new target location - 
        # 0 is dead center
        # positive values mean right
        # negative values mean left

        msg = yield boat.get_shape_location(shape)
        msg_x = msg[0]
        msg_color = msg[1]

        # Throws out bad readings

        if msg_x == 0: pass
            #yield boat.move.forward(.5).go()
            #print "No shape found, moving forward"
        if msg_x != 0 and msg_color == color:

            # If the pixel location is within our range of error
            if abs(msg_x) < PIXEL_TOLERANCE:
                print color + shape + " in Center at location: " + str(msg_x) + " --- Locking Target"
                break
                # Break the loop and continue

            angle_move = TEN_DEG 

            # If the target is right of the center 
            if msg_x > 0:
                # turn the number of degrees right between the center and the target, minus an offset
                #boat.move.turn_left(-angle_move).go()
                print "Turning right", abs(angle_move)

            # If the target is left of the center
            if msg_x < 0:
                # turn the number of degrees left between the center and the target, minus an offset
                #boat.move.turn_left(angle_move).go()
                print "Turning left", abs(angle_move)

            print color + " " + shape + " at pixel location: ", abs(msg_x)


@util.cancellableInlineCallbacks
def main(nh, shape=None, color=None):

    if shape == None:
        shape = DEFAULT_SHAPE
    if color == None:
        color = DEFAULT_COLOR

    boat = yield boat_scripting.get_boat(nh, False, False)

    reorient = True

    # While the boat is still distant from the target
    while True and not rospy.is_shutdown():

        temp_distance = 0
        avg_distance = 0
        shortest_distance = 100
        farthest_distance = 0
        x_mean = 0
        y_mean = 0
        numerator = 0
        denom = 0

        try:
            yield util.wrap_timeout(center_sign(boat, shape, color), 20)
            print "Locked onto shape"
        except Exception:
            print "Could not dock find shape, moving on to dock"
        finally: pass

        # Only reorients one time
        '''
        if reorient == True and REORIENT == True:
            print "reorienting"
            orientation = yield orient(boat)
            sign_centered = yield center_sign(boat, shape, color)
            reorient = False
        '''

        print "holding at current position"
        print "Scanning lidar"

        distances = yield boat.get_distance_from_object(.05)

        avg_distance = distances[0] - BOAT_LENGTH_OFFSET
        shortest_distance = distances[1]- BOAT_LENGTH_OFFSET
        farthest_distance = distances[2] - BOAT_LENGTH_OFFSET

        print "Average distance from target:", avg_distance 
        print "Shortest distance between boat and object:", shortest_distance 
        print "Farther distance between boat and object:", farthest_distance 

        final_move = shortest_distance/MOVE_OFFSET

        if farthest_distance > 3.5: 
            #raw_input("Press enter to move forward" + str(final_move) + " meters")
            # Print only if a move is commanded
            print "Moving forward " + str(final_move) + " meters"
            yield boat.move.forward(final_move).go()
        if farthest_distance <= 1.5: 
            #raw_input("Press enter to move forward one meter")
            # Print only if a move is commanded
            print "Moving forward one meter"
            boat.move.forward(1.5).go()
            print "Moving back 5 meters"
            yield util.sleep(5)
            yield boat.move.forward(-5).go()
            print 'Find shape success'
            break

        # delete variables to avoid any threading problems
        del avg_distance, temp_distance, farthest_distance, shortest_distance