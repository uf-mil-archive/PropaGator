

#! /usr/bin/env python

from __future__ import division
from std_msgs.msg import Float64
from txros import util
import rospy
import boat_scripting

@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)

    yield boat.hold_at_current_pos()


