 #! /usr/bin/env python

from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy


 #SPP allign the craft based on what pings the hydrophones hear for a given freq
@util.cancellableInlineCallbacks
def main(self, frequency):
    good = 0
    while True:
        x = self.float()
        try:
            yield util.sleep(.5)
            ping_return = yield self.get_processed_ping(frequency)
        finally:
            x.cancel()
        print ping_return
        if ping_return.declination > 1.2:
            good += 1
            if good > 4: return
        elif abs(ping_return.heading) > math.radians(30):
            good = 0
            yield self.move.yaw_left(ping_return.heading).go()
        else:
            good = 0
            yield self.move.forward(2).go()