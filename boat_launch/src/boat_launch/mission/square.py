from __future__ import division

from txros import util

import boat_scripting

@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)
    
    for i in xrange(1):
        print 'Side', i
        yield boat.move.turn_left_deg(45).go()
