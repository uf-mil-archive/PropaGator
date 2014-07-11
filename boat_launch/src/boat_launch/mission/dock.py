from __future__ import division

from txros import util

import boat_scripting


@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)
    
    for TARGET in ['triangle', 'circle', 'cruciform'][::-1]:
        yield boat.visual_approach('forward', 'boat_dock/' + TARGET, 24*.0254, 3)
        yield boat.move.forward(1.4).go(speed=.15)
        yield boat.move.backward(4).go()
