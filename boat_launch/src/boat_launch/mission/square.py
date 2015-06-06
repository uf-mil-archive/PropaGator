#! /usr/bin/env python

from __future__ import division
from std_msgs.msg import Float64
from txros import util
import rospy
import boat_scripting
#import gain_control

@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)

    for i in xrange(4):
        print 'Side', i
        #gain_control.less_gain()
        yield boat.move.forward(-10).go()
        #gain_control.more_gain()
        yield boat.move.turn_left_deg(90).go()

