#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('path_planner')
from std_msgs.msg import Header
import math,time,numpy
from visualization_msgs.msg import Marker,MarkerArray
from std_msgs.msg import ColorRGBA
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Pose,Quaternion,Point,WrenchStamped,Wrench,Vector3,Vector3Stamped,PoseStamped
import random,numpy,threading
import actionlib
from sensor_msgs.msg import Image,PointCloud2,PointField
from c3_trajectory_generator.msg import MoveToAction, MoveToGoal
from uf_common.msg import MoveToAction, MoveToGoal
from pointcloud2xyz import *
from uf_common.orientation_helpers import lookat, get_perpendicular,PoseEditor
from path_planner.msg import TraverseBuoysAction
from controller.srv import Enable,EnableResponse

rospy.init_node('channel_navigation')
#-----------------------------------------------------------------------
waypoint = actionlib.SimpleActionClient('moveto', MoveToAction)
output_wrench = rospy.Publisher('wrench', WrenchStamped)

lock = threading.Lock()
global current_position,running
current_position = []
running = False

def distance (p1,p2):
	return (math.sqrt((p2[1]-p1[1])**2 + (p2[0]-p1[0])**2))
	
def vector(event):
        if (running):
                global master_cloud,current_position
                deviation = 0
                print 'len',len(master_cloud)
                lock.acquire()
                for point in master_cloud:
                        deviation = deviation + (math.copysign(1,point[1])/numpy.linalg.norm(numpy.array([point[0],point[1]])))
                lock.release()        
                print 'deviation',deviation
                waypoint.send_goal(current_pose_editor.as_MoveToGoal(linear=[0,0,0],angular=[0,0,10*deviation]))
                
                '''
                output_wrench.publish(WrenchStamped(
				        header = Header(
					        stamp=rospy.Time.now(),
					        frame_id="/base_link",
					        ),
				        wrench=Wrench(
					        force = Vector3(x= .2,y= 0,z= 0),
					        torque = Vector3(x=0,y= 0,z= .01*deviation),
					        ))
					        )	         
                '''
rospy.Timer(rospy.Duration(.1),vector)

def in_range(x):
        max_distance = 8
        min_distance = 1
        if (all(math.fabs(i) < max_distance for i in x) and all(math.fabs(i) > min_distance for i in x)):
                return True
        else:
                return False

global master_cloud
master_cloud = []
def pointcloud_callback(msg):
        lock.acquire()
        global master_cloud
        cloud = pointcloud2_to_xyz_array(msg)
        if (len(cloud) > 0):
                master_cloud = filter(in_range,cloud)
        lock.release()
rospy.Subscriber("/cloud_3d",PointCloud2,pointcloud_callback)
#-----------------------------------------------------------------------

def pose_callback(msg):
        global current_position,current_pose_editor
	current_pose_editor = PoseEditor.from_Odometry(msg)
	current_position = [msg.pose.pose.position.x,msg.pose.pose.position.y]
rospy.Subscriber('/sim_odom', Odometry, pose_callback)


class TraverseBuoysServer:

 def __init__(self):
        self.server = actionlib.SimpleActionServer('traverse_buoys', TraverseBuoysAction, self.execute, False)
        global running
        waypoint.cancel_goal()
        running = False
        self.controller_enable = rospy.ServiceProxy('/controller/enable', Enable)
        self.server.start()
        print "path_planner server started"

 def execute(self,goal):
        global running
        #(numpy.linalg.norm(numpy.array(ecef_position)-numpy.array(end_position)) > 5) and
        self.controller_enable(False)
        while ( not(self.server.is_preempt_requested())):
             running = True
        running = False
        self.controller_enable(True)
        if (numpy.linalg.norm(numpy.array(ecef_position)-numpy.array(end_position)) < 5):
                self.server.set_succeeded()
        else:
                self.server.set_preempted()                  

server = TraverseBuoysServer()
rospy.spin()
waypoint.cancel_goal()





