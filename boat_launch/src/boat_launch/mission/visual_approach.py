#! /usr/bin/env python

from __future__ import division
from txros import action, util, tf
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy
import numpy
from legacy_vision import msg as legacy_vision_msg
from uf_common.msg import MoveToAction, MoveToGoal, PoseTwistStamped, Float64Stamped
from tf import transformations
import traceback


@util.cancellableInlineCallbacks
def main(nh, camera, object_name, size_estimate, desired_distance, selector=lambda items, body_tf: items[0]):

    boat = yield boat_scripting.get_boat(nh)

    goal_mgr = boat._camera_2d_action_clients[camera].send_goal(legacy_vision_msg.FindGoal(
        object_names=[object_name],
    ))

    start_pose = boat.pose
    start_map_transform = tf.Transform(
        start_pose.position, start_pose.orientation)
    try:
        while True:
            feedback = yield goal_mgr.get_feedback()
            res = map(json.loads, feedback.targetreses[0].object_results)
            
            try:
                transform = yield boat._tf_listener.get_transform('/base_link',
                    feedback.header.frame_id, feedback.header.stamp)
                map_transform = yield boat._tf_listener.get_transform('/enu',
                    '/base_link', feedback.header.stamp)
            except Exception:
                traceback.print_exc()
                continue
            
            if not res: continue
            obj = selector(res, transform)
            if obj is None: continue
            
            ray_start_camera = numpy.array([0, 0, 0])
            ray_dir_camera = numpy.array(map(float, obj['center']))
            obj_dir_camera = numpy.array(map(float, obj['direction']))
            
            ray_start_world = map_transform.transform_point(
                transform.transform_point(ray_start_camera))
            ray_dir_world = map_transform.transform_vector(
                transform.transform_vector(ray_dir_camera))
            obj_dir_world = map_transform.transform_vector(
                transform.transform_vector(obj_dir_camera))
            
            axis_camera = [0, 0, 1]
            axis_body = transform.transform_vector(axis_camera)
            axis_world = start_map_transform.transform_vector(axis_body)
            
            distance_estimate = size_estimate / (2 * numpy.linalg.norm(obj_dir_camera))
            print distance_estimate
            
            object_pos = ray_start_world + ray_dir_world * distance_estimate
            print object_pos
            
            desired_pos = object_pos - axis_world * desired_distance
            
            error_pos = desired_pos - map_transform.transform_point([0, 0, 0])
            error_pos[2] = 0
            
            print desired_pos, numpy.linalg.norm(error_pos)/6e-2
            
            if numpy.linalg.norm(error_pos) < 6e-2: # 6 cm
                yield (boat.move
                    .set_position(desired_pos)
                    .go())
                return
            
            # go towards desired position
            boat._moveto_action_client.send_goal(
                start_pose.set_position(desired_pos).as_MoveToGoal(speed=0.1)).forget()
    finally:
        goal_mgr.util.cancel()
        yield boat.move.go() # stop moving