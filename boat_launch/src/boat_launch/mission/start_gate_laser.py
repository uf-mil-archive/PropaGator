import numpy
import math
from txros import util
import boat_scripting
from uf_common.orientation_helpers import xyz_array, xyzw_array, quat_to_rotvec
from uf_common.orientation_helpers import rotvec_to_quat
from uf_common.orientation_helpers import get_perpendicular
from itertools import *

"""
# This mission attempts to go through the start gate and speed gate using only lidar
#   Algorithim: 
#       * Set lidar angle to max so as to only see gates in front of the boat
#       * Ignore everything behind the boat
#       * Find the two clossest objects
#       * If they look like a start gate move towards there centroid
#       * repete one more time
"""
@util.cancellableInlineCallbacks
def main(nh):
    # Print mission start msg
    print 'Finding start gate with laser'
    boat = yield boat_scripting.get_boat(nh)

    print 'Still the lidar at 3.3 degrees'
    boat.still_lidar(nominal_angle=3.2)

    # How many gates we've gone through
    gates = 0

    while gates < 2:

        print 'Start getting odom'
        pose = boat.get_gps_odom_rel()

        print 'Get objects'
        buoys = yield boat.get_objects()
        #print 'Original buoys' + str(buoys)

        print 'Wait on odom to finish'
        pose = yield pose

        # Get an enu xyz array
        # Get a rotation matrix
        position = xyz_array(pose.pose.pose.position)[0:2]
        yaw = quat_to_rotvec(xyzw_array(pose.pose.pose.orientation))[2]
        heading = numpy.array([numpy.cos(yaw), numpy.sin(yaw)])

        # Translate to base link (no rotation)
        def translate(v):
            return xyz_array(v.position)[0:2] - position

        buoys = map(translate, buoys)
        #print 'buoy translated to base_link: ' + str(buoys)

        # Get a vector in the boats forward direction
        def get_angle(v):
            return numpy.arccos(v.dot(heading) / numpy.linalg.norm(v))

        buoy_angle = map(get_angle, buoys)
        #print 'buoy angles: ' + str(buoy_angle)

        buoys = zip(buoys, buoy_angle)
        #print 'Ziped buoys: ' + str(buoys)

        buoys = filter(lambda x: abs(x[1]) < 60 * numpy.pi / 180, buoys)
        #print 'Filter buoys behind the boat: ' + str(buoys)

        # Drop the angles
        buoys = [b[0] for b in buoys]

        # Get the two closses to the front buoys
        def distance(b1, b2):
            value =  numpy.linalg.norm(b1) - numpy.linalg.norm(b2)
            if value < 0:
                return -1
            elif value > 0:
                return 1
            else:
                return 0

        buoys = sorted(buoys, cmp=distance)

        # Check to make sure there are at least two buoys
        # If not try again
        if len(buoys) < 2:
            yield boat.move.forward(1).go()
            print '1 or less buoys found trying again'
            continue


        # Make sure the buoys are at least x meters apart and
        #   no more than y meters apart
        start_gate_pos = numpy.zeros(2)
        buoy_to_buoy = numpy.zeros(2)
        for b in combinations(buoys, 2):
            buoy_to_buoy = b[0] - b[1]
            dis = numpy.linalg.norm(buoy_to_buoy)
            if dis > 1 and dis < 5:
                # Found a gate
                start_gate_pos = (b[0] + b[1]) / 2
                break 
        
        if not numpy.any(start_gate_pos):
            print 'No two buoys form a start gate trying again'
            yield boat.move.forward(1).go()
            continue

        print 'Moving to gate ' + str(gates + 1)
        gates = gates + 1
        # get the xyz of the goal
        goal_pos = position + start_gate_pos
        goal_pos = numpy.insert(goal_pos, 2, 0)
        #print 'Goal pose: ' + str(goal_pos)

        # get the orientation vector
        point_to = get_perpendicular(numpy.insert(buoy_to_buoy, 2, 0), numpy.array([0, 0, 1]))[0:2]
        print 'Start gate pos: ', start_gate_pos
        print 'point to: ', point_to
        angle = get_angle(point_to)
        print 'Angle of goal: ', angle
        if get_angle(point_to) > numpy.pi / 2:
            # Flip the orientation
            point_to = -1 * point_to
        goal_orientation = rotvec_to_quat(numpy.array([0, 0, numpy.arctan2(point_to[1], point_to[0])])) 



        # Set pose and orientation
        yield boat.move.set_position(goal_pos).set_orientation(goal_orientation).go()
        yield boat.move.forward(3).go()

    print 'Completed start and speed gate!!'


    #yield boat.move.as_MoveToGoal(linear,angular).go()
