from __future__ import division

import json
import math
import traceback
import time
import Queue
import scipy

import numpy
from twisted.internet import defer

from txros import action, util, tf

#SPP having access to the gps methods will allow us to easily switch from ECEF to lat/long
import rawgps_common

import genpy
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
from geometry_msgs.msg import PointStamped, Wrench, WrenchStamped, Vector3
#for dynamixel configs
from dynamixel_servo.msg import DynamixelFullConfig 
from rise_6dof.srv import SendConstantWrench, SendConstantWrenchRequest
from sensor_msgs.msg import LaserScan
from sensor_msgs.msg import Image
from object_handling.msg import Buoys

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
    def _init(self, need_trajectory=True):
        print 1
        self._trajectory_sub = self._node_handle.subscribe('trajectory', PoseTwistStamped)
        print 2
        self._moveto_action_client = action.ActionClient(self._node_handle, 'moveto', MoveToAction)
        self._tf_listener = tf.TransformListener(self._node_handle)
        self._camera_2d_action_clients = dict(
            forward=action.ActionClient(self._node_handle, 'find2_forward_camera', legacy_vision_msg.FindAction),
        )
        self._camera_3d_action_clients = dict(
            forward=action.ActionClient(self._node_handle, 'find_forward', object_finder_msg.FindAction),
        )
        self._odom_sub = self._node_handle.subscribe('odom', Odometry)
        self._wrench_sub = self._node_handle.subscribe('wrench', WrenchStamped)
        #The absodom topic has the lat long information ebeded in the nav_msgs/Odometry. the lat long is under pose.pose.position and this is what is reported back in the JSON messages for a chalenge.
        self._absodom_sub = self._node_handle.subscribe('absodom', Odometry)
        #SPP subscribe the boat to the processed pings, so the missions can utalize the processed acoustic pings
        self._hydrophone_ping_sub = self._node_handle.subscribe('hydrophones/processed', ProcessedPing)
        #SPP subscribe the boat to the GPS messages, so the missions can easly get the data for their JSON messages
        self._hydrophone_freq_sub= self._node_handle.subscribe('hydrophones/desired_freq', Float64)
        
        self.servo_full_config_pub = self._node_handle.advertise('dynamixel/dynamixel_full_config', DynamixelFullConfig)
        
        self._send_constant_wrench_service = self._node_handle.get_service_client('send_constant_wrench', SendConstantWrench)

        self._lidar_sub = self._node_handle.subscribe('lidar/scan', LaserScan)
        
        self._object_sub = self._node_handle.subscribe('object', Buoys)

        self._current_image_sub = self._node_handle.subscribe('/camera/image_raw', Image)


        if(need_trajectory == True):
            yield self._trajectory_sub.get_next_message()
        
        defer.returnValue(self)
    
    @property
    def pose(self):
        return orientation_helpers.PoseEditor.from_PoseTwistStamped(
            self._trajectory_sub.get_last_message())
    @property
    def odom(self):
        return orientation_helpers.PoseEditor.from_Odometry(
            self._odom_sub.get_last_message())
    
    @property
    def move(self):
        return _PoseProxy(self, self.pose)
    
    @util.cancellableInlineCallbacks
    def float(self):
        while True:
            self._send_constant_wrench_service(SendConstantWrenchRequest(
                wrench=Wrench(
                    force=Vector3(0, 0, 0),
                    torque=Vector3(0, 0, 0),
                ),
                lifetime=genpy.Duration(1),
            ))
            yield util.sleep(.5)
    
    @util.cancellableInlineCallbacks
    def wait_for_bump(self):
        while True:
            wrench = yield self._wrench_sub.get_next_message()
            if wrench.wrench.force.x > 60:
                break
    
    @util.cancellableInlineCallbacks
    def deploy_hydrophone(self):
        #INITIALLY WHEN THE BOAT STARTS THE CABLE SHOULD BE FEED OVER THE TOP OF THE SERVO
        deploy_msg=DynamixelFullConfig()
        deploy_msg.id=4 #id 4 is the stern servo for the hydrophones
        deploy_msg.led=0
        deploy_msg.goal_position=-2*math.pi
        deploy_msg.moving_speed=1.4 # 1.4 rad/s~22 rpm
        deploy_msg.torque_limit=173 # 173/1023 is about 17% torque
        deploy_msg.goal_acceleration=20
        deploy_msg.control_mode=DynamixelFullConfig.CONTINUOUS_ANGLE
        deploy_msg.goal_velocity=1.4
        for i in xrange(100):
            self.servo_full_config_pub.publish(deploy_msg)
            yield util.sleep(8.5/100)
    
    @util.cancellableInlineCallbacks
    def retract_hydrophone(self):
        #WHEN THE HYDROPHONES RETRACT CABLE SHOULD FEED BACK OVER THE TOP OF THE SERVO
        deploy_msg=DynamixelFullConfig()
        deploy_msg.id=4 #id 4 is the stern servo for the hydrophones
        deploy_msg.led=0
        deploy_msg.goal_position=4.3 # 2.4 rad/s~22 rpm NOTE: we explicitly retract to pi to try and avoid being at the 0/2*PI boundary on a powerup
        deploy_msg.moving_speed=1.4 # 1.4 rad/s~22 rpm
        deploy_msg.torque_limit=143 # 143/1023 is about 14% torque (so we don't break the rope if someone didn't feed them correctly to start)
        deploy_msg.goal_acceleration=20
        deploy_msg.control_mode=DynamixelFullConfig.CONTINUOUS_ANGLE
        deploy_msg.goal_velocity=1.4
        self.servo_full_config_pub.publish(deploy_msg)
        for i in xrange(100):
            self.servo_full_config_pub.publish(deploy_msg)
            yield util.sleep(20/100)
    
    
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
            if abs(msg.freq-frequency) < 1.5e3:
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
    
    @util.cancellableInlineCallbacks
    def get_scan(self):
        msg = yield self._lidar_sub.get_next_message()
        defer.returnValue(msg)
    
    
    @util.cancellableInlineCallbacks
    def get_objects(self):
        msg = yield self._object_sub.get_next_message()
        defer.returnValue(msg)
    
    @util.cancellableInlineCallbacks
    def get_current_image(self):
        msg = yield self._current_image_sub.get_next_message()
        defer.returnValue(msg)
    

    #SPP allign the craft based on what pings the hydrophones hear for a given freq
    @util.cancellableInlineCallbacks
    def hydrophone_align(self, frequency):
        good = 0
        while True:
                x = self.float()
                try:
                    yield util.sleep(.5)
                    ping_return = yield self.get_processed_ping(frequency)
                finally:
                    x.cancel()
                print ping_return
                if ping_return.declination > 1.2:
                    good += 1
                    if good > 4: return
                elif abs(ping_return.heading) > math.radians(30):
                    good = 0
                    yield self.move.yaw_left(ping_return.heading).go()
                else:
                    good = 0
                    yield self.move.forward(2).go()
    
    
    @util.cancellableInlineCallbacks
    def get_ecef_pos(self):
        msg = yield self._absodom_sub.get_next_message()
        defer.returnValue(orientation_helpers.xyz_array(msg.pose.pose.position))
    
    @util.cancellableInlineCallbacks
    def go_to_ecef_pos(self, pos, speed=0, turn=True):
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
def get_boat(node_handle, need_trajectory=True):
    if node_handle not in _boats:
        _boats[node_handle] = None # placeholder to prevent this from happening reentrantly
        _boats[node_handle] = yield _Boat(node_handle)._init(need_trajectory)
        # XXX remove on nodehandle shutdown
    defer.returnValue(_boats[node_handle])
