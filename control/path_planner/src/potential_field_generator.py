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
import pointcloud2xyz
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
	
def vector():
        if (running):
                global master_cloud,current_position
                deviation = 0
		sum_ = numpy.zeros(3)
		angle_sum = 0
		left = 0
		right = 0
		
                if (len(master_cloud) > 0):
		        for point in master_cloud:
				sum_ += numpy.array(point)
				angle_sum +=  math.atan2(point[1],point[0])
			mean_angle = angle_sum/float(len(master_cloud))
			mean_point = sum_/float(len(master_cloud))
                        '''
			for point in master_cloud:
				if (math.atan2(point[1],point[0]) < mean_angle):
					right += 1
					deviation += (1/numpy.linalg.norm(numpy.array([point[0],point[1]])))
				else:
					left += 1
					deviation -= (1/numpy.linalg.norm(numpy.array([point[0],point[1]])))
			print "left",left
			print "right",right
		        print 'deviation',deviation
                        '''
			print "cloud size",len(master_cloud)
                        print 'mean point',mean_point
			
			waypoint.send_goal(current_pose_editor.relative(numpy.array(mean_point)).as_MoveToGoal(speed = .8))
			#waypoint.send_goal_and_wait(current_pose_editor.yaw_left(mean_angle))
			
                #waypoint.send_goal(current_pose_editor.as_MoveToGoal(linear=[.8,0,0],angular=[0,0,0]))
                
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
#rospy.Timer(rospy.Duration(.1),vector)

def in_range(x):
        max_distance = 8
        min_distance = 1.5
        if ((min_distance <= numpy.linalg.norm(x) <= max_distance) and x[0] > 1):
                return True
        else:
                return False

global master_cloud
master_cloud = []
def pointcloud_callback(msg):
        global master_cloud
        cloud = pointcloud2xyz.pointcloud2_to_xyz_array(msg)
        if (len(cloud) > 0):
                master_cloud = filter(in_range,cloud)
	vector()
rospy.Subscriber("/cloud_3d",PointCloud2,pointcloud_callback)
#-----------------------------------------------------------------------
def pose_callback(msg):
        global current_position,current_pose_editor
	current_pose_editor = PoseEditor.from_Odometry(msg)
	current_position = [msg.pose.pose.position.x,msg.pose.pose.position.y]
rospy.Subscriber('/odom', Odometry, pose_callback)

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
        global running,current_pose_editor
        #(numpy.linalg.norm(numpy.array(ecef_position)-numpy.array(end_position)) > 5) and
        print "speed challenge"
	waypoint.send_goal_and_wait(current_pose_editor.forward(25))
	print "starting buoy channel"
        #self.controller_enable(False)
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





