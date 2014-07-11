from __future__ import division

import json
import math
import traceback
import time

import numpy
from twisted.internet import defer

from txros import action, util, tf

#SPP having access to the gps methods will allow us to easily switch from ECEF to lat/long
import rawgps_common

from std_msgs.msg import Header, Float64
from uf_common.msg import MoveToAction, MoveToGoal, PoseTwistStamped, Float64Stamped
from legacy_vision.msg import FindAction, FindGoal
from uf_common import orientation_helpers
from tf import transformations
from legacy_vision import msg as legacy_vision_msg
from object_finder import msg as object_finder_msg
from nav_msgs.msg import Odometry
from rawgps_common import gps
#SPP for hydropones
from hydrophones.msg import ProcessedPing
#SPP for getting gps fix lat/long
from geometry_msgs.msg import PointStamped
#for dynamixel configs
from dynamixel_servo.msg import DynamixelFullConfig 


class _PoseProxy(object):
    def __init__(self, boat, pose):
        self._boat = boat
        self._pose = pose
    
    def __getattr__(self, name):
        def _(*args, **kwargs):
            return _PoseProxy(self._boat, getattr(self._pose, name)(*args, **kwargs))
        return _
    
    def go(self, *args, **kwargs):
        return self._boat._moveto_action_client.send_goal(
            self._pose.as_MoveToGoal(*args, **kwargs)).get_result()

class _Boat(object):
    def __init__(self, node_handle):
        self._node_handle = node_handle
    
    @util.cancellableInlineCallbacks
    def _init(self):
        self._trajectory_sub = self._node_handle.subscribe('trajectory', PoseTwistStamped)
        self._moveto_action_client = action.ActionClient(self._node_handle, 'moveto', MoveToAction)
        self._tf_listener = tf.TransformListener(self._node_handle)
        self._camera_2d_action_clients = dict(
            forward=action.ActionClient(self._node_handle, 'find2_forward_camera', legacy_vision_msg.FindAction),
        )
        self._camera_3d_action_clients = dict(
            forward=action.ActionClient(self._node_handle, 'find_forward', object_finder_msg.FindAction),
        )
        self._odom_sub = self._node_handle.subscribe('odom', Odometry)
        #The absodom topic has the lat long information ebeded in the nav_msgs/Odometry. the lat long is under pose.pose.position and this is what is reported back in the JSON messages for a chalenge.
        self._absodom_sub = self._node_handle.subscribe('absodom', Odometry)
        #SPP subscribe the boat to the processed pings, so the missions can utalize the processed acoustic pings
        self._hydrophone_ping_sub = self._node_handle.subscribe('hydrophones/processed', ProcessedPing)
        #SPP subscribe the boat to the GPS messages, so the missions can easly get the data for their JSON messages
        self._hydrophone_freq_sub= self._node_handle.subscribe('hydrophones/desired_freq', Float64)
        
        self.servo_full_config_pub = self._node_handle.advertise('dynamixel/dynamixel_full_config', DynamixelFullConfig)
        
        yield self._trajectory_sub.get_next_message()
        
        defer.returnValue(self)
    
    @property
    def pose(self):
        return orientation_helpers.PoseEditor.from_PoseTwistStamped(
            self._trajectory_sub.get_last_message())
    
    @property
    def move(self):
        return _PoseProxy(self, self.pose)
    
    @util.cancellableInlineCallbacks
    def deploy_hydrophone(self):
        deploy_msg=DynamixelFullConfig()
        deploy_msg.id=5
        deploy_msg.goal_position=4*2*math.pi
        deploy_msg.moving_speed=3, # near maximum, not actually achievable ...
        deploy_msg.torque_limit=800
        deploy_msg.goal_acceleration=20
        deploy_msg.control_mode=DynamixelFullConfig.CONTINUOUS_ANGLE
        deploy_msg.goal_velocity=3
        servo_full_config_pub(deploy_msg)
        return
    
    @util.cancellableInlineCallbacks
    def retract_hydrophone(self):
        deploy_msg=DynamixelFullConfig()
        deploy_msg.id=5
        deploy_msg.goal_position=0
        deploy_msg.moving_speed=3, # near maximum, not actually achievable ...
        deploy_msg.torque_limit=800
        deploy_msg.goal_acceleration=20
        deploy_msg.control_mode=DynamixelFullConfig.CONTINUOUS_ANGLE
        deploy_msg.goal_velocity=3
        servo_full_config_pub(deploy_msg)
        return
    
    
    @util.cancellableInlineCallbacks
    def get_hydrophone_freq(self):
        msg = yield self._hydrophone_freq_sub.get_next_message()
        defer.returnValue(msg)
        
    #SPP get the latest processed acoustic message that is at the specified frequency
    @util.cancellableInlineCallbacks
    def get_processed_ping(self, frequency):
        while True:
            # keep looking for a ping at the specified frequency
            msg = yield self._hydrophone_ping_sub.get_next_message()
            if msg.freq==frequency:
                # only if you receive one should you return. NOTE: mission_core run_missions will timeout and kill this task so it wont run forever()
                defer.returnValue(msg)        
    
    #SPP get the latest gps lat/long fix from the 
    @util.cancellableInlineCallbacks
    def get_gps_lat_long(self):
        msg = yield self._absodom_sub.get_next_message()
        #lat long is under msg.pose.pose.position of the nav_msgs/Odometry for the '/absodom' topic
        # Note: /absodom is Earth-Centered,Earth-Fixed (ECEF), so This means that ECEF rotates with the earth and a point fixed on the surface of the earth do not change.
        # See: http://en.wikipedia.org/wiki/ECEF
        temp=latlongheight_from_ecef(msg.pose.pose.position.x,msg.pose.pose.position.y,msg.pose.pose.position.z)
        ret_dict={'latitude' : temp[0],'longitude':temp[1]}
        defer.returnValue(ret_dict)
    
    @util.cancellableInlineCallbacks
    def get_gps_odom(self):
        msg = yield self._absodom_sub.get_next_message()
        defer.returnValue(msg)
    
    #SPP allign the craft based on what pings the hydrophones hear for a given freq
    @util.cancellableInlineCallbacks
    def hydrophone_align(self, frequency):
        i=1
        averaging_amount=5; #since it pings every 2 seconds remeber 5 declination_ammounts (averaging_amount*2 seconds)
        declination_ammounts = Queue() # this will be used to make sure that we are always approaching the pinger and not pointed 180deg in the oposite direction
        try:
            while True:
                ping_return=self.get_processed_ping(frequency)
                if(declination_ammounts.qsize()>averaging_amount):
                    declination_ammounts.get_nowait()#we only want up to averaging_amount of elements to compare
                declination_ammounts.put(ping_return.declination)
                print i,". ping_return freq: ", ping_return.freq, "  declination: ", ping_return.declination, " heading ", ping_return.heading, " amplitude: ", ping_return.amplitude, " valid: ", ping_return.valid
                #if we ger a high enough declination we should be getting closer at about 45 degrees off from it we should be able to quit
                if ping_return.declination>=.7 and ping_return.declination<=.9:
                    return
                # We would like to try to be straight on if possible so look at angles first
                elif ping_return.freq<-.15:
                    # rotate back left if the boat is more than .15 radians~8.5deg off, do it slowly as to not over shoot
                    yield self.move.turn_left_deg(degrees(fabs(ping_return.freq)/2))
                elif ping_return.freq>.15:
                    # rotate back right if the boat is more than .15 radians~8.5deg off, do it slowly as to not over shoot
                    yield self.move.turn_right_deg(degrees(fabs(ping_return.freq)/2))
                elif average(ediff1d(declination_ammounts))<0:
                    #ediff1d does a[0]=a[1]-a[0], a[1]=a[2]-a[1] ... a[N]=a[N]-a[N-1]
                    #so basicly make sure that we are still relativly approaching the beacon and not going in the oposite direction
                    yield self.move.turn_right_deg(91) # basicly start to turn arround since were going the wrong dir
                else:
                    #move forward some slowly
                    yield self.move.forward(2)
        finally:
            goal_mgr.cancel()
    
    
    @util.cancellableInlineCallbacks
    def get_ecef_pos(self):
        msg = yield self._absodom_sub.get_next_message()
        defer.returnValue(orientation_helpers.xyz_array(msg.pose.pose.position))
    
    @util.cancellableInlineCallbacks
    def go_to_ecef_pos(self, pos):
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
                    return
                
                if first:
                    yield self.move.look_at_without_pitching(enu_pos).go()
                    first = False
                
                self._moveto_action_client.send_goal(
                    self.pose.set_position(enu_pos).as_MoveToGoal()).forget()
        finally:
            yield self.move.go() # stop moving
    
    @util.cancellableInlineCallbacks
    def visual_align(self, camera, object_name, distance_estimate, selector=lambda items, body_tf: items[0], turn=True):
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
    
    @util.cancellableInlineCallbacks
    def visual_approach(self, camera, object_name, size_estimate, desired_distance, selector=lambda items, body_tf: items[0]):
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
                
                distance_estimate = size_estimate / (2 * numpy.linalg.norm(obj_dir_camera))
                print distance_estimate
                
                object_pos = ray_start_world + ray_dir_world * distance_estimate
                print object_pos
                
                desired_pos = object_pos - axis_world * desired_distance
                
                error_pos = desired_pos - map_transform.transform_point([0, 0, 0])
                error_pos[2] = 0
                
                print desired_pos, numpy.linalg.norm(error_pos)/6e-2
                
                if numpy.linalg.norm(error_pos) < 6e-2: # 6 cm
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
    
    @util.cancellableInlineCallbacks
    def visual_approach_3d(self, camera, distance, targetdesc, loiter_time=0):
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

_boats = {}
@util.cancellableInlineCallbacks
def get_boat(node_handle):
    if node_handle not in _boats:
        _boats[node_handle] = None # placeholder to prevent this from happening reentrantly
        _boats[node_handle] = yield _Boat(node_handle)._init()
        # XXX remove on nodehandle shutdown
    defer.returnValue(_boats[node_handle])
