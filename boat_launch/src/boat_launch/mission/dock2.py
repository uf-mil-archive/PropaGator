from __future__ import division

import itertools

from geometry_msgs.msg import PoseWithCovariance, Quaternion, Pose, Point
from txros import util
import roslib

import boat_scripting
from object_finder import msg as object_finder_msg
from sim import threed

def from_obj(filename):
    mesh = threed.mesh_from_obj(filename)
    
    res = object_finder_msg.Mesh()
    
    components = []
    for mtl, x in itertools.groupby(zip(mesh.indices, mesh.materials), lambda (triangle, mtl): mtl):
        triangles = []
        for triangle, mtl_ in x:
            assert len(triangle) == 3
            assert mtl_ is mtl
            
            triangles.append(object_finder_msg.Triangle(
                corners=[Point(*mesh.vertices[vert_index]) for vert_index, tex_index, normal_index in triangle],
            ))
        
        components.append(object_finder_msg.Component(
            name=mtl.name,
            color=object_finder_msg.Color(
                r=mtl.Kd[0],
                g=mtl.Kd[1],
                b=mtl.Kd[2],
            ),
            triangles=triangles,
        ))
    
    components.sort(key=lambda component: component.name)
    
    return object_finder_msg.Mesh(components=components)

targetdesc = object_finder_msg.TargetDesc(
    type=object_finder_msg.TargetDesc.TYPE_MESH,
    mesh=None,
    prior_distribution=PoseWithCovariance(
        pose=Pose(
            orientation=None, # set later
        ),
        covariance=[
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
        ],
    ),
    min_dist=1,
    max_dist=6,
    disallow_yawing=True,
)

@util.cancellableInlineCallbacks
def main(nh):
    print 'aa'
    boat = yield boat_scripting.get_boat(nh)
    print 'bb'
    
    start_pose = boat.move
    
    for x in ['cruciform', 'circle', 'triangle'][::-1]:
        yield start_pose.backward(0).go()
        
        targetdesc.mesh = from_obj(roslib.packages.resource_file('boatsim', 'models', x + '.obj'))
        targetdesc.prior_distribution.pose.orientation = Quaternion(*boat.pose.orientation)
        
        print 'a'
        yield boat.visual_approach_3d('forward', 4, targetdesc)
        yield boat.move.forward(1.5).go(speed=.3)
        fwd_task = boat.move.forward(100).go(speed=.2)
        try:
            yield boat.wait_for_bump()
        finally:
            fwd_task.cancel()
        print 'b'
    yield start_pose.backward(0).go()
    
    #yield boat.move.forward(1).go()
    #yield boat.move.backward(1.5).go()
    
    #yield boat.move.depth(0.5).go()
