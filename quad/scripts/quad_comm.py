#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('quad')
from geometry_msgs.msg import PointStamped,Point
from nav_msgs.msg import Odometry
from rawgps_common.gps import ecef_from_latlongheight,enu_from_ecef
import threading,numpy
import xbee
import actionlib
from quad.msg import LaunchQuadAction
rospy.init_node('quad')



def pos_callback(msg):  
        global pos      
        pos = [msg.point.x,msg.point.y,msg.point.z]

class LaunchQuadServer:

 def __init__(self):
        self.server = actionlib.SimpleActionServer('launch_quad', LaunchQuadAction, self.execute, False)
        self.xbee = xbee.xbee('xbee')                 
        rospy.Subscriber('/latlong',PointStamped,pos_callback)
        self.server.start()
        print "quad server started"

 def execute(self,goal):
        global pos

        dock = [goal.lat,goal.long,goal.alt]
        done = False
        self.xbee.send_start(dock)

        while (not(self.server.is_preempt_requested()) and not(done)):
                 done = self.xbee.check_done()  

        if (done == False):
                self.xbee.timeout()
                self.server.set_preempted() 
        else:
                self.xbee.send_pos(pos)
                done = True
                self.server.set_succeeded()
        

server = LaunchQuadServer()
rospy.spin()
