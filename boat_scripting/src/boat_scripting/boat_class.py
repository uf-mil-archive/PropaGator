from __future__ import division

import json
import math
import traceback
import time
import Queue
import scipy

import numpy
from twisted.internet import defer
import sensor_msgs.point_cloud2 as pc2


from txros import action, util, tf

#SPP having access to the gps methods will allow us to easily switch from ECEF to lat/long
import rawgps_common


import genpy
from std_msgs.msg import Header, Float64, Int16, Bool
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
from sensor_msgs.msg import LaserScan, PointCloud2
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
        self._wrench_sub = self._node_handle.subscribe('wrench', WrenchStamped)
        #The absodom topic has the lat long information ebeded in the nav_msgs/Odometry. the lat long is under pose.pose.position and this is what is reported back in the JSON messages for a chalenge.
        self._absodom_sub = self._node_handle.subscribe('absodom', Odometry)
        #SPP subscribe the boat to the processed pings, so the missions can utalize the processed acoustic pings
        self._hydrophone_ping_sub = self._node_handle.subscribe('hydrophones/processed', ProcessedPing)
        #SPP subscribe the boat to the GPS messages, so the missions can easly get the data for their JSON messages
        self._hydrophone_freq_sub= self._node_handle.subscribe('hydrophones/desired_freq', Float64)
        
        self.servo_full_config_pub = self._node_handle.advertise('dynamixel/dynamixel_full_config', DynamixelFullConfig)
        
        self._send_constant_wrench_service = self._node_handle.get_service_client('send_constant_wrench', SendConstantWrench)

        self._lidar_sub_raw = self._node_handle.subscribe('lidar/scan', LaserScan)

        self._lidar_sub_pointcloud = self._node_handle.subscribe('lidar/raw_pc', PointCloud2)
        
        self._object_sub = self._node_handle.subscribe('objects', Buoys)

        self._start_gate_vision_sub = self._node_handle.subscribe('start_gate_vision', Float64)

        self._circle_detect = self._node_handle.subscribe("circle_detected" , Bool)
        self._square_detect = self._node_handle.subscribe("cross_detected" , Bool)
        self._triangle_detect = self._node_handle.subscribe("triangle_detected" , Bool)
        self._circle_pos = self._node_handle.subscribe("circle_x_pos" , Int16)
        self._cross_pos = self._node_handle.subscribe("cross_x_pos" , Int16)
        self._triangle_pos = self._node_handle.subscribe("triangle_x_pos" , Int16)

        self._odom_pub = self._node_handle.advertise('odom', Odometry)


        if(need_trajectory == True):

            yield self._trajectory_sub.get_next_message()
        
        defer.returnValue(self)

    # Do not use Pose unless you are sure that trajectory is being published (i.e. 
    #    when you got a boat need_trajectory was set to true)
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

    @util.cancellableInlineCallbacks
    def hold_at_current_pos(self):

        odom = yield self.get_gps_odom_rel()
        print odom
        odom_to_send = yield orientation_helpers.PoseEditor.from_Pose(odom.header.frame_id, odom.pose.pose)
        goal = yield odom_to_send.as_MoveToGoal()
        self._moveto_action_client.send_goal(goal)
              
    @util.cancellableInlineCallbacks
    def get_shape_location(self, shape):
        if shape == 'circle':
            msg = yield self._circle_pos.get_next_message()
            defer.returnValue(msg)
        if shape == 'cross':
            msg = yield self._cross_pos.get_next_message()
            defer.returnValue(msg)
        if shape == 'triangle':
            msg = yield self._triangle_pos.get_next_message()
            defer.returnValue(msg)

        print 'Invalid shape ', shape
        assert False
    
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
    def to_baselink(self, msg):
        transform = yield self._tf_listener.get_transform('/base_link', msg.header.frame_id, msg.header.stamp)
        res = []
        for p in pc2.read_points(msg, field_names=("x", "y", "z"), skip_nans=False, uvs=[]):
            res.append(transform.transform_point((p[0], p[1], p[2])))
        defer.returnValue(res)
    
    @util.cancellableInlineCallbacks
    def get_gps_odom(self):
        msg = yield self._absodom_sub.get_next_message()
        defer.returnValue(msg)

    @util.cancellableInlineCallbacks
    def get_gps_odom_rel(self):
        msg = yield self._odom_sub.get_next_message()
        defer.returnValue(msg)
    
    @util.cancellableInlineCallbacks
    def get_scan(self):
        msg = yield self._lidar_sub_raw.get_next_message()
        defer.returnValue(msg)

    @util.cancellableInlineCallbacks
    def get_pointcloud(self):
        msg = yield self._lidar_sub_pointcloud.get_next_message()
        defer.returnValue(msg)
    
    @util.cancellableInlineCallbacks
    def get_objects(self):
        msg = yield self._object_sub.get_next_message()
        defer.returnValue(msg.buoys)
        
    @util.cancellableInlineCallbacks
    def get_start_gate_vision(self):
        msg = yield self._start_gate_vision_sub.get_next_message()
        defer.returnValue(msg.data)
    
    @util.cancellableInlineCallbacks
    def get_ecef_pos(self):
        msg = yield self._absodom_sub.get_next_message()
        defer.returnValue(orientation_helpers.xyz_array(msg.pose.pose.position))
    
    
_boats = {}
@util.cancellableInlineCallbacks
def get_boat(node_handle, need_trajectory=True):
    if node_handle not in _boats:
        _boats[node_handle] = None # placeholder to prevent this from happening reentrantly
        _boats[node_handle] = yield _Boat(node_handle)._init(need_trajectory)
        # XXX remove on nodehandle shutdown
    defer.returnValue(_boats[node_handle])
