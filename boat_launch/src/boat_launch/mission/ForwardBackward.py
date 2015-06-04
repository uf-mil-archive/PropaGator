from __future__ import division

import math

from txros import util

import boat_scripting


@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)
    
    print 'Next'
    yield boat.move.forward(-2).go()
    print 'Next'
    yield boat.move.forward(-2).go()
    print 'Next0'
    yield boat.move.forward(-2).go()
    print 'Next'
    yield boat.move.forward(-2).go()
    print 'Next'
    yield boat.move.forward(-2).go()


    print 'Next'
    yield boat.move.forward(-2).go()
    print 'Next'
    yield boat.move.forward(-2).go()
    print 'Next0'
    yield boat.move.forward(-2).go()
    print 'Next'
    yield boat.move.forward(-2).go()
    print 'Next'
    yield boat.move.forward(-2).go()