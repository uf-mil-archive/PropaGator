from __future__ import division

import json
import math
import traceback

import numpy
from twisted.internet import defer

from txros import action, util, tf

from uf_common.msg import MoveToAction, MoveToGoal, PoseTwistStamped, Float64Stamped
from legacy_vision.msg import FindAction, FindGoal
from uf_common import orientation_helpers
from tf import transformations


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
        
        yield self._trajectory_sub.get_next_message()
        
        defer.returnValue(self)
    
    @property
    def _pose(self):
        return orientation_helpers.PoseEditor.from_PoseTwistStamped(
            self._trajectory_sub.get_last_message())
    
    @property
    def move(self):
        return _PoseProxy(self, self._pose)

_boats = {}
def get_boat(node_handle):
    if node_handle not in _boats:
        _boats[node_handle] = _Boat(node_handle)._init()
        # XXX remove on nodehandle shutdown
    return _boats[node_handle]
