#! /usr/bin/env python

from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy


@util.cancellableInlineCallbacks
def main(self, camera, object_name, distance_estimate, selector=lambda items, body_tf: items[0], turn=True):
    goal_mgr = self._camera_2d_action_clients[camera].send_goal(legacy_vision_msg.FindGoal(
        object_names=[object_name],
    ))
    start_pose = self.pose
    start_map_transform = tf.Transform(
        start_pose.position, start_pose.orientation)
    try:
        while True:
            feedback = yield goal_mgr.get_feedback()
            res = map(json.loads, feedback.targetreses[0].object_results)
            
            try:
                transform = yield self._tf_listener.get_transform('/base_link',
                    feedback.header.frame_id, feedback.header.stamp)
                map_transform = yield self._tf_listener.get_transform('/enu',
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
            
            # project ray onto plane defined by distance_estimate
            # from start_pose.position along axis_world
            
            plane_point = distance_estimate * axis_world + start_pose.position
            plane_vector = axis_world
            
            x = plane_vector.dot(ray_start_world - plane_point) / plane_vector.dot(ray_dir_world)
            object_pos = ray_start_world - ray_dir_world * x
            
            desired_pos = object_pos - start_map_transform.transform_vector(transform.transform_point(ray_start_camera))
            desired_pos = desired_pos - axis_world * axis_world.dot(desired_pos - start_pose.position)
            
            error_pos = desired_pos - map_transform.transform_point([0, 0, 0])
            error_pos = error_pos - axis_world * axis_world.dot(error_pos)
            error_pos[2] = 0
            
            print desired_pos, numpy.linalg.norm(error_pos)/3e-2
            
            if numpy.linalg.norm(error_pos) < 3e-2: # 3 cm
                if turn:
                    direction_symmetry = int(obj['direction_symmetry'])
                    dangle = 2*math.pi/direction_symmetry
                    
                    def rotate(x, angle):
                        return transformations.rotation_matrix(angle, axis_world)[:3, :3].dot(x)
                    for sign in [-1, +1]:
                        while rotate(obj_dir_world, sign*dangle).dot(start_pose.forward_vector) > obj_dir_world.dot(start_pose.forward_vector):
                            obj_dir_world = rotate(obj_dir_world, sign*dangle)
                    
                    yield (self.move
                        .set_position(desired_pos)
                        .look_at_rel_without_pitching(obj_dir_world)
                        .go())
                else:
                    yield (self.move
                        .set_position(desired_pos)
                        .go())
                
                return
            
            # go towards desired position
            self._moveto_action_client.send_goal(
                start_pose.set_position(desired_pos).as_MoveToGoal(speed=0.1)).forget()
    finally:
        goal_mgr.cancel()
        yield self.move.go() # stop moving