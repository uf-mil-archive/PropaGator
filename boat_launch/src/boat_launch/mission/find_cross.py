#! /usr/bin/env python

from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy
import sensor_msgs.point_cloud2 as pc2
import threading

@util.cancellableInlineCallbacks
def main(nh):

    boat = yield boat_scripting.get_boat(nh, False)

   

    while True:


        temp_distance = 0
        final_dstance = 100
        average_distance = 0

        while True:
            msg =  yield boat.get_shape_location('cross')


            if abs(msg.data) < 10:
                print "cross in Center at location: " + str(msg.data) + " --- Locking Target"
                break

            if msg.data > 0:
                # turn 10 degrees
                yield boat.move.turn_left(.085).go()
                print "Moving left 10 degrees"

            if msg.data < 0:
                # turn 10 degrees
                yield boat.move.turn_left(-.085).go()

                print "Moving right 10 degrees"

            print "cross at pixel location: ", msg.data 

        print "Station holding at current position"
        yield boat.hold_at_current_pos()
        print "Scanning lidar"
        lidar_scan = yield boat.get_pointcloud()
        pointcloud = pc2.read_points(lidar_scan, field_names=("x", "y", "z"), skip_nans=False, uvs=[])

        print "Filtering lidar data"
        hold = filter(lambda x: abs(x[1]) < .02, pointcloud)

        if len(hold) == 0: hold.append((None,None, None)) 

        final_distance = 100
        print len(hold)
        for x in xrange(1,len(hold)):
            dist = hold[x]
            temp_distance = temp_distance + dist[0]
            if dist[0] < final_distance:
                final_distance = dist[0]

        average_distance = temp_distance/len(hold)+1
        if final_distance == 100: final_distance = 0

        print "Average distance from target:", average_distance
        print "Closest point to target:", final_distance
        if average_distance > 1: yield boat.move.forward(average_distance).go()
        if average_distance < 1: break










