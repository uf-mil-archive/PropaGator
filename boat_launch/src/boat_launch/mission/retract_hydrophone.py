#!/usr/bin/python

from __future__ import division

from txros import util

import boat_scripting


@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh, False, False)
    
    yield boat.retract_hydrophone()