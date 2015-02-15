from __future__ import division

from txros import util

import boat_scripting


@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)
    
    for i in xrange(4):
        print 'Step Number #', i
        yield boat.move.forward(2).go()
        yield boat.move.turn_right_deg(90).go()
        yield boat.move.forward(2).go()
        yield boat.move.turn_left_deg(90).go()