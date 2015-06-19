#! /usr/bin/env python

from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy


@util.cancellableInlineCallbacks
def main(nh, boat):

    while True:
        msg =  yield boat.get_shape_location('triangle')
        if abs(msg.data) < 10:
            print "Triangle in Center at location: " + str(msg.data) + " --- Locking Target"
            break

        print "Triangle at pixel location: ", abs(msg.data) 


    yield boat.hold_at_current_pos()
    lidar_scan = yield boat.get_scan()



