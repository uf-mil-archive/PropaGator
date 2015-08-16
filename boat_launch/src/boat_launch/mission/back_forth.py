#! /usr/bin/env python

from __future__ import division
from std_msgs.msg import Float64
from txros import util
import rospy
import boat_scripting

@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)

    while True:
        print 'Forward 10'
        yield boat.move.forward(10).go()
        print 'Turn around'
        yield boat.move.turn_left_deg(180).go()


