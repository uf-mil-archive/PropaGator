 #! /usr/bin/env python

from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy
import math


def get_declination(x, y, z):
    return math.atan2(-z, (math.sqrt(x**2 + y**2 + z**2)))

def get_heading(x, y):
    return math.atan2(y, x)

 #SPP allign the craft based on what pings the hydrophones hear for a given freq
@util.cancellableInlineCallbacks
def main(nh, min_freq, max_freq):
    boat = yield boat_scripting.get_boat(nh)
    while True:
        ping_return = None
        while ping_return is None:
            try:
                print 'Float'
                boat.float_on()
                yield util.sleep(.5)
                print 'Listen'
                ping_return = yield util.wrap_timeout(boat.get_processed_ping((min_freq, max_freq)), 20)
            except Exception:
                # Timeout rotate 30 deg
                print 'Timeout no ping rotating 30 degrees left'
                boat.float_off()
                #yield boat.move.set_position(boat.odom.position).set_orientation(boat.odom.orientation)
                yield boat.move.yaw_left_deg(30).go()
                print 'Rotated 30 degrees'

	    boat.float_off()
        
        # Changed message type to handle this. Conversion is carried out in the hydrophones script in 
        # software_common/hydrophones/scripts
        print 'Ping is: ', ping_return
        print 'I say heading is: ', ping_return.heading
        print 'I say declination is: ', ping_return.declination

        if ping_return.declination > 1.2:
            good += 1
            if good > 4:
                print 'Success!'
                return
        elif abs(ping_return.heading) > math.radians(30):
            good = 0
            print 'Turn to ping'
            yield boat.move.yaw_left(ping_return.heading).go()
        else:
            good = 0
            print 'Move towards ping'
            yield boat.move.forward(2).go()
