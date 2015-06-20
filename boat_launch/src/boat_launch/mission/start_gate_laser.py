import numpy
import math
from txros import util
import boat_scripting


@util.cancellableInlineCallbacks
def main(nh, left, up, right, down):
    # Print mission start msg
    print 'Finding start gate with laser'
    boat = yield boat_scripting.get_boat(nh)
    buoys = yield boat.get_objects()
    #boat_pose = yield boat.pose()
    '''Sort buoys
    '''
    #filter
    print buoys
    buoys = buoys_quick_sort(buoys)
    #find midpoint

    desired_position_x = (buoys.x + buoys.x) / 2
    desired_position_y = (buoys[1] + buoys[1]) / 2
    #find angle
    delta_x = (buoys.x[0] - buoys.x[1])
    delta_y = (buoys[0].y - buoys[1].y)
    #calculate perpendicular angle
    buoy_orientation = math.atan(delta_y/delta_x)
    #print buoy_orientation, "buoy_orientation"
    if buoy_orientation > numpy.pi/2:
        desired_orientation = buoy_orientation - numpy.pi
    elif buoy_orientation < numpy.pi/2:
        desired_orientation = buoy_orientation + numpy.pi
    linear = [desired_position_x, desired_position_y, 0]
    angular = [0, 0, desired_orientation]
    '''May need to convert to numpy array
    '''
    yield boat.move.as_MoveToGoal(linear,angular).go()

@util.cancellableInlineCallbacks
def buoys_quick_sort(l):
    boat = yield boat_scripting.get_boat(nh)
    boat_pose = yield boat.pose()
    length = len(l)
    if length <=1:
        yield l
    else:
        pivot = l.pop(int(length/2))
        less, more = [], []
        for x in l:
            if abs((x).y-boat.pose().y) <= abs((pivot).y-boat.pose().y):
                less.append(x)
            else:
                more.append(x)
        yield buoys_quick_sort(less) + [pivot] + buoys_quick_sort(more)
