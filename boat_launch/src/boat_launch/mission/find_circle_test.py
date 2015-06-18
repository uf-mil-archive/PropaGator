#! /usr/bin/env python


from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy


@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)

    while True:
       msg =  yield boat.get_shape('circle')
       if abs(msg) < 100:
           break


    yield boat.hold_at_current_pos()
    lidar_scan = yield boat.get_scan()

    # Found square and scanner lidar

    while True:
       msg =  yield boat.get_shape('square')
       if abs(msg) < 100:
           break


    yield boat.hold_at_current_pos()
    lidar_scan = yield boat.get_scan()

    # Found triangle and scanner lidar

    while True:
       msg =  yield boat.get_shape('triangle')
       if abs(msg) < 100:
           break


    yield boat.hold_at_current_pos()
    lidar_scan = yield boat.get_scan()






