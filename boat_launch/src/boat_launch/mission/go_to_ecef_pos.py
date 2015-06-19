#! /usr/bin/env python

from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy

@util.cancellableInlineCallbacks
def main(self, pos, speed=0, turn=True):
    print 'go_to_ecef_pos', pos, speed, turn
    success = False
    try:
        first = True
        while True:
            odom_df = self._odom_sub.get_next_message()
            abs_msg = yield self._absodom_sub.get_next_message()
            msg = yield odom_df
            
            error_ecef = pos - orientation_helpers.xyz_array(abs_msg.pose.pose.position)
            error_enu = gps.enu_from_ecef(ecef_v=error_ecef, ecef_pos=orientation_helpers.xyz_array(abs_msg.pose.pose.position))
            error_enu[2] = 0
            print error_enu, '=>', numpy.linalg.norm(error_enu), '->', 1
            
            enu_pos = orientation_helpers.xyz_array(msg.pose.pose.position) + error_enu
            enu_pos[2] = self.pose.position[2]
            
            if numpy.linalg.norm(error_enu) < 1:
                yield self.move.set_position(enu_pos).go()
                print 'go_to_ecef_pos', 'succeeded'
                return
            
            if first and turn:
                yield self.move.look_at_without_pitching(enu_pos).go(speed=speed)
                first = False
            
            self._moveto_action_client.send_goal(
                self.pose.set_position(enu_pos).as_MoveToGoal(speed=speed)).forget()
        success = True
    finally:
        if not success:
            yield self.move.set_position(self.odom.position).go() # stop moving
            yield self.move.backward(3).go()
