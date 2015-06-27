 #! /usr/bin/env python

from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy


 #SPP allign the craft based on what pings the hydrophones hear for a given freq
@util.cancellableInlineCallbacks
def main(nh, freq):

    boat = yield boat_scripting.get_boat(nh, False)
    while True:
        boat.float_on()
        try:
            yield util.sleep(.5)
            ping_return = yield boat.get_processed_ping(freq)
        finally: 
            pass
            # Figure out what you want to use here, used to be --> yield boat.cancel()
            # Not sure you really need to use anything really, test that out though
	    boat.float_off()
        print ping_return

        if ping_return.declination > 1.2:
            good += 1
            if good > 4: return
        elif abs(ping_return.heading) > math.radians(30):
            good = 0
            yield boat.move.yaw_left(ping_return.heading).go()
        else:
            good = 0
            yield boat.move.forward(2).go()
