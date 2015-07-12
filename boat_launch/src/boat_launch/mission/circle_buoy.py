#! /usr/bin/env python

from __future__ import division
from std_msgs.msg import Float64
from txros import util
import rospy
import boat_scripting
import numpy
import math
from uf_common.orientation_helpers import xyz_array, xyzw_array, quat_to_rotvec

@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)

    boat_x = boat.odom.position[0]
    boat_y = boat.odom.position[1]

    closest = 0
    summation = 100

    closest_buoy = 0
    buoys = yield boat.get_bouys()

    while len(buoys.buoys) <= 0:
        buoys = yield boat.get_bouys()

    for i in buoys.buoys:

        x_dif = boat_x - i.position.x
        y_dif = boat_y - i.position.y

        temp_sum = abs(x_dif) + abs(y_dif)
        #print temp_sum

        if temp_sum < summation:
            summation = temp_sum
            closest = i

    x_move = boat_x - closest.position.x
    y_move = boat_y - closest.position.y


    point = numpy.array([closest.position.x, closest.position.y, 0])

    position = boat.odom.position[0:2]
    yaw = quat_to_rotvec(boat.odom.orientation)[2]
    heading = numpy.array([numpy.cos(yaw), numpy.sin(yaw)])

    vec2 = numpy.array([heading[0], heading[1]])
    vec1 = numpy.array([closest.position.x, closest.position.y])


    numerator = numpy.dot(vec1, vec2)
    one = numpy.linalg.norm(vec1)
    two =  numpy.linalg.norm(vec2)
    cosine = numerator / (one * two)
    # Angle betwen the boat and the shore
    angle_to_move = math.atan(cosine)
    print angle_to_move

    boat.move.yaw_left(-angle_to_move).go()
    
    yield boat.move.set_position(point).go()
    yield boat.move.turn_left_deg(-20).go()


    for i in xrange(4):
        yield boat.move.forward(3).go()
        yield boat.move.turn_left_deg(90).go()
        







