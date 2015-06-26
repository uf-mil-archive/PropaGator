#! /usr/bin/env python

from __future__ import division
from txros import util
import boat_scripting
from uf_common import orientation_helpers
from rawgps_common import gps
import numpy

@util.cancellableInlineCallbacks
def main(nh, pos, speed=0, turn=True):
    print 'go_to_ecef_pos: ', pos, "\n\tAt: ", speed, 'm/s\n\t', 'Turn first: ', turn

    boat = yield boat_scripting.get_boat(nh)

    success = False
    try:
        first = True
        last_enu_pos = numpy.zeros(3)
        while True:
            odom_df = boat.get_gps_odom_rel()
            abs_msg = yield boat.get_gps_odom()
            msg = yield odom_df
            
            error_ecef = pos - orientation_helpers.xyz_array(abs_msg.pose.pose.position)
            error_enu = gps.enu_from_ecef(ecef_v=error_ecef, ecef_pos=orientation_helpers.xyz_array(abs_msg.pose.pose.position))
            error_enu[2] = 0
            print error_enu, '=>', numpy.linalg.norm(error_enu), '->', 1
            
            enu_pos = orientation_helpers.xyz_array(msg.pose.pose.position) + error_enu
            enu_pos[2] = boat.pose.position[2]
            
            if numpy.linalg.norm(error_enu) < 1:
                yield boat.move.set_position(enu_pos).go()
                print 'go_to_ecef_pos', 'succeeded'
                success = True
                return
            
            # Turn and send initial move incase enu_pos is within half a metter of [0,0,0]
            if first and turn:
                if turn:
                    yield boat.move.look_at_without_pitching(enu_pos).go(speed=speed)
                boat._moveto_action_client.send_goal(
                    boat.pose.set_position(enu_pos).as_MoveToGoal(speed=speed)).forget()
                first = False
            
            # Check if the position drifted more than half a meter radius
            #   if so send a new goal
            if numpy.linalg.norm(last_enu_pos - enu_pos) > 0.5:
                print 'Set new position'
                boat._moveto_action_client.send_goal(
                    boat.pose.set_position(enu_pos).as_MoveToGoal(speed=speed)).forget()

            last_enu_pos = enu_pos

        success = True
    finally:
        if not success:
            print 'go_to_ecef_pos failed to attain desired position, holding current position'
            yield boat.move.set_position(boat.odom.position).go() # stop moving
            #yield boat.move.backward(3).go() # I have no idea why this needs to happen
