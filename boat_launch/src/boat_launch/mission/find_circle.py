#! /usr/bin/env python

from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy
import sensor_msgs.point_cloud2 as pc2

@util.cancellableInlineCallbacks
def main(nh, boat):

    if boat == None:
        boat = yield boat_scripting.get_boat(nh, False)

    temp_distance = 0
    final_dstance = 0

    while True:
        
        while True:
            msg =  yield boat.get_shape_location('circle')

            if abs(msg.data) < 10:
                print "circle in Center at location: " + str(msg.data) + " --- Locking Target"
                break

            if msg.data > 0:
                # turn 10 degrees
                yield boat.move.turn_left(.085).go()

            if msg.data < 0:
                # turn 10 degrees
                yield boat.move.turn_left(-.085).go()

            print "circle at pixel location: ", abs(msg.data) 

        print "Station holding at current position"
        yield boat.hold_at_current_pos()
        print "Scanning lidar"
        lidar_scan = yield boat.get_pointcloud()
        pointcloud = pc2.read_points(lidar_scan, field_names=("x", "y", "z"), skip_nans=False, uvs=[])

        print "Filtering lidar data"
        x_value = x[1]
        hold = filter(lambda x: abs(x_value) < .02, pointcloud)


        for x in xrange(1,len(hold)):
            dist = hold[x]
            temp_distance = temp_distance + dist[0]

        final_distance = temp_distance/len(hold)

        print "temp_Distance from target:", final_distance

        if final_distance > 1: yield boat.move.forward(1).go()
        if final_distance < 1: break









