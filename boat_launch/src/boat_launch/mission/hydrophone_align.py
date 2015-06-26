 #! /usr/bin/env python

from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy


 #SPP allign the craft based on what pings the hydrophones hear for a given freq
@util.cancellableInlineCallbacks
def main(nh):

    boat = yield boat_scripting.get_boat(nh, False)


    '''

    Ralph:

    All should be set up in this file, I am still working on the boat class

    Still need to set the frequency variable
    It used to given as a parameter so the code is set up for it, I just didn't know how it worked


    frequency = ???


    '''

    good = 0

    while True:

        yield boat.float()

        try:
            yield util.sleep(.5)
            ping_return = yield boat.get_processed_ping(frequency)
        finally: 
            pass
            # Figure out what you want to use here, used to be --> yield boat.cancel()
            # Not sure you really need to use anything really, test that out though
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