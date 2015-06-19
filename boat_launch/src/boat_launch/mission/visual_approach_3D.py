#! /usr/bin/env python

from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy


@util.cancellableInlineCallbacks
def main(self, camera, distance, targetdesc, loiter_time=0):
    goal_mgr = self._camera_3d_action_clients[camera].send_goal(object_finder_msg.FindGoal(
        header=Header(
            frame_id='/enu',
        ),
        targetdescs=[targetdesc],
    ))
    start_pose = self.pose
    
    try:
        last_good_pos = None
        loiter_start = None
        
        while True:
            feedback = yield goal_mgr.get_feedback()
            targ = feedback.targetreses[0]
            
            if targ.P > 0.55:
                last_good_pos = orientation_helpers.xyz_array(targ.pose.position)
            
            print targ.P
            
            if last_good_pos is not None:
                desired_pos = start_pose.set_position(last_good_pos).backward(distance).position
                desired_pos[2] = start_pose.position[2]
                
                print ' '*20, numpy.linalg.norm(desired_pos - self.pose.position), desired_pos, self.pose.position
                
                if numpy.linalg.norm(desired_pos - self.pose.position) < 0.5:
                    if loiter_start is None:
                        loiter_start = time.time()
                    
                    if time.time() > loiter_start + loiter_time:
                        yield self._moveto_action_client.send_goal(
                            start_pose.set_position(desired_pos).as_MoveToGoal()).get_result()
                        return
                
                self._moveto_action_client.send_goal(
                    start_pose.set_position(desired_pos).as_MoveToGoal(speed=.5)).forget()
    finally:
        goal_mgr.cancel()
        yield self.move.go() # stop moving
