#!/usr/bin/env python
import boat_scripting
import numpy as np
from txros import util
from uf_common.orientation_helpers import rotvec_to_quat

# This mission will move the boat to a point on the line defined by
#   point and direction
# If offset is 0 the boat will move to point and look in direction
# If offset is not zero the boat will move to point + (offset meters along the direction)
# If flip is true the boat will align to direction + 180 degrees
# 
#   point: numpy.array(), [x, y, z]
#   direction: numpy.array(), [roll, pitch, yaw]    Note roll and pitch are ignored
#   offset: float meters
#   flip: bool

@util.cancellableInlineCallbacks
def main(nh, point, direction, offset = 0.0, flip = False):
    boat = yield boat_scripting.get_boat(nh)

    print 'Direction:', direction * 180 / np.pi
    yaw = direction[2]
    print 'Yaw:', yaw * 180 / np.pi
    print 'dx:', np.cos(yaw) * offset
    print 'dy:', np.sin(yaw) * offset
    print 'Point:', point
    point = point + [np.cos(yaw) * offset, np.sin(yaw) * offset, 0]
    if flip:
        direction[2] = (direction[2] + np.pi) % (2*np.pi)

    yield boat.move.set_position(point).set_orientation(rotvec_to_quat(direction)).go()