from __future__ import division

import math

from txros import util

import boat_scripting
from rawgps_common import gps

def ll(lat, lon):
    return gps.ecef_from_latlongheight(math.radians(lat), math.radians(lon), 0)

@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)
    
    yield boat.go_to_ecef_pos(ll(29.641090, -82.361292))
    yield boat.go_to_ecef_pos(ll(29.641260, -82.361197))
    yield boat.go_to_ecef_pos(ll(29.641394, -82.361293))
    yield boat.go_to_ecef_pos(ll(29.641651, -82.361444))
    yield boat.go_to_ecef_pos(ll(29.641245, -82.361014))
    yield boat.go_to_ecef_pos(ll(29.641360, -82.361104))
    yield boat.go_to_ecef_pos(ll(29.641104, -82.361470))
    yield boat.go_to_ecef_pos(ll(29.641274, -82.361403))
