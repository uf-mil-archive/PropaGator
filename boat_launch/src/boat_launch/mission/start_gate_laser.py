import numpy
import math
from txros import util
import boat_scripting
from uf_common.orientation_helpers import xyz_array, xyzw_array, quat_to_rotvec
from uf_common.orientation_helpers import rotvec_to_quat
from uf_common.orientation_helpers import get_perpendicular
from itertools import *
from boat_launch.mission import move_on_line

"""
This mission attempts to go through the start gate and speed gate using only lidar data

Algorithim: 
* Set lidar angle to pan between max (calculated below) and slightly above horizontal, so as to only see start/speed gates which should be the tallest object
* Ignore everything behind the boat
* Find the two clossest objects
* If they look like a start gate move towards there centroid
* repete one more time

Math:
* Maximum angle of lidar, 3.529 deg at 50 ft and 7.031 deg at 25ft:
  * The competitoin start/speed gate buoys are 49" tall
  * The lidar sits above the water line about 1 ft
  * Some of the gate will be submerged
  * The max angle is the angle formed between the ray drawn from the lidar to the top of the buoy and the line drawn through the lidar parrallel to horizontal
  * The buoys could be anywhere between 25-50 ft away
  
References:
* [Rules] (https://s3.amazonaws.com/com.felixpageau.roboboat/RoboBoat_2015_final_rules_20150527.pdf)
"""

# TODO:
#       use start angle instead of constantly changing angle
#       

# The boat stops GATE_DISTANCE before the first gate and 
#   GATE_DISTANCE after gate two
GATE_DISTANCE = 1.5
GATE_LOOK_AHEAD = GATE_DISTANCE * 1.5 + 1.0

# Max + - angle that gates can be in
GATE_ANGLE = 20 * numpy.pi / 180

HALF_PI = numpy.pi / 2

# Find gate algorithim
def filter_gates(boat, gates, yaw, look_ahead):
    if gates is None:
        return (None, None)

    #print 'Original gates' + str(gates)

    # Get an enu xyz array
    # Get a rotation matrix
    position = boat.odom.position[0:2]
    
    heading = numpy.array([numpy.cos(yaw), numpy.sin(yaw)])

    # Translate to base link or base_link + GATE_DISTANCE (no rotation)
    def translate(v):
        return xyz_array(v.position)[0:2] - position

    gates_xyz = map(translate, gates)
    gates = zip(gates, gates_xyz)
    if look_ahead:
        # Remove gates that are to close
        gates = filter(lambda x: numpy.linalg.norm(x[1]) > GATE_LOOK_AHEAD, gates)
        

    # Get a vector in the boats forward direction
    def get_angle(v):
        return numpy.arccos(v[1].dot(heading) / numpy.linalg.norm(v[1]))

    gate_angles = map(get_angle, gates)
    #print 'gate angles: ' + str(gate_angles)

    gates = zip(gates, gate_angles)
    #print 'Ziped gates: ' + str(gates)

    gates = filter(lambda x: abs(x[1]) < GATE_ANGLE, gates)
    #print 'Filter gates behind the boat: ' + str(gates)

    if len(gates) == 0:
        print 'No gates in front of boat'
        return (None, None)

    # Drop the angles
    #gates = [g[0] for g in gates]

    # This function finds a score for how likely it is that the gate is correct
    #   It takes into account:
    #       gt = the angle OF the gate compared to input argument yaw
    #       d = the distance away the gate is from the boat
    #       gb = the angle TO the gate from the boat
    #   The formula is:
    #       0.5 * gb / GATE_ANGLE + 0.4 * d / 20 + 0.1 * gt / (pi/2) where:
    #       gt = abs( (pi / 2) - abs(angle of gate - yaw) % pi 
    #           gt = 0 when the boat and the gate are at 90 degrees to eachother
    #           gt is max (pi / 2) when the boat and the gate are parrallel
    #           gt E [0, pi / 2]
    #       d = distance to boat
    #           d E [0, 20] (20 is max lidar range)
    #       gb = angle between boat orientation and the gates centroid
    #           gb E [0, GATE_ANGLE]
    def cost(g):
        gt = abs( HALF_PI - (abs(g[0][0].yaw - yaw) % numpy.pi))
        gt = gt / HALF_PI
        d = numpy.linalg.norm(g[0][1])
        d = d / 20
        gb = g[1]
        gb = gb / GATE_ANGLE
        cost = 0.5 * gb + 0.4 * d + 0.2 * gt
        print 'cost:', cost, '\n\tgb:', gb, '\n\td:', d, '\n\tgt:', gt
        return cost

    # Clossest gate
    #gate = min(gates, key=lambda x: numpy.linalg.norm(x[1]))

    # Smallest angle
    #gate = min(gates, key=lambda x: abs(x[1]))[0]

    # Lowest cost gate
    gate = min(gates, key=cost)[0]

    # get the xyz of the goal in enu
    goal_pos = gate[1] + position
    goal_pos = numpy.insert(goal_pos, 2, 0)
    #print 'Goal pose: ' + str(goal_pos)

    # Get orientation angle and make sure we are facing forward and not backward
    angle = gate[0].yaw + numpy.pi / 2
    if abs(angle - yaw) > numpy.pi / 2:
        angle = angle + numpy.pi
    #print 'Angle of goal: ', angle
    
    goal_orientation = numpy.array([0, 0, angle])
    #goal_orientation = rotvec_to_quat(numpy.array([0, 0, angle])) 

    return (goal_pos, goal_orientation)


@util.cancellableInlineCallbacks
def main(nh):
    # #Print mission start msg
    #print 'Finding start gate with laser'
    boat = yield boat_scripting.get_boat(nh)

    try:
        yaw = quat_to_rotvec(boat.odom.orientation)[2]
        #print 'Pan the lidar between the maximum angle and slightly above horizontal'
        boat.pan_lidar(max_angle=3.264, min_angle=3.15, freq=0.5)
        
        # How many gates we've gone through
        gates_passed = 0

        have_gate = False
        last_gate_pos = None
        move = None

        while gates_passed < 2:
            #print 'Get gates'    
            gates = yield boat.get_gates()

            (gate_pos, gate_orientation) = (None, None) 
            if gates_passed == 0:
                (gate_pos, gate_orientation) = filter_gates(boat, gates, yaw, False)
            else:
                (gate_pos, gate_orientation) = filter_gates(boat, gates, yaw, True)

            # Check if valid gate found
            if gate_pos is not None:
                have_gate = True
                # Check if we previously found a gate
                if last_gate_pos is not None:
                    # Check if the gate center has drifted
                    # Don't go to a different gate (dis < 5)
                    dis = numpy.linalg.norm(last_gate_pos - gate_pos)
                    #print 'Distance: ', dis
                    if dis >= 0.1 and dis < 3:
                        print 'Gate drifted re-placing goal point'
                        #move = boat.move.set_position(gate_pos).set_orientation(gate_orientation).go()
                        if gates_passed == 0:
                            move = move_on_line.main(nh, gate_pos, gate_orientation, -GATE_DISTANCE)
                        else:
                            move = move_on_line.main(nh, gate_pos, gate_orientation, GATE_DISTANCE)
                    else:
                        #print 'Still happy with my current goal'
                        pass
                else:
                    # Just found a gate    
                    print 'Moving to gate ' + str(gates_passed + 1)
                    #move = boat.move.set_position(gate_pos).set_orientation(gate_orientation).go()
                    if gates_passed == 0:
                        #yield boat.move.yaw_left_deg(30).go()
                        move = move_on_line.main(nh, gate_pos, gate_orientation, -GATE_DISTANCE)
                        print 'zzz'
                        yield util.sleep(1.0)
                    else:
                        move = move_on_line.main(nh, gate_pos, gate_orientation, GATE_DISTANCE)

                # Set last gate pos
                last_gate_pos = gate_pos

            else:
                if have_gate is False:
                    print 'No gate found moving forward 1m'
                    yield boat.move.forward(1).go()
                else:
                    print 'Lost sight of gate; Continuing to last known position'

            # Check if task complete
            if have_gate and move.called:
                print 'Move complete'
                #yield boat.move.forward(3).go()
                yaw = quat_to_rotvec(boat.odom.orientation)[2]
                have_gate = False
                last_gate_pos = None
                gates_passed = gates_passed + 1
        
    finally:
        boat.default_state()
