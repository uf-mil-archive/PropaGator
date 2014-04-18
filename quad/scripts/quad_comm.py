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

rospy.Subscriber('/latlong',PointStamped,pos_callback)

class LaunchQuadServer:
    
    def __init__(self):
        self.server = actionlib.SimpleActionServer('launch_quad', LaunchQuadAction, self.execute, False)
        self.xbee = xbee.xbee('ttyUSB0')
        
        self.server.start()
        print "quad server started"
    
    def execute(self,goal):
        global pos
        
        dock = [goal.lat,goal.long,goal.alt]
        done = False
        puck_done = False
        
        self.xbee.send_start(dock)                                            #send start to quad followed by gps location of dock
        
        while (not(self.server.is_preempt_requested()) and not(puck_done)):
            puck_done = self.xbee.check_puck_status()                    #check to see if quad got puck
        
        
        
        while (not(self.server.is_preempt_requested()) and not(done)):        #wait for quad to signal it is finished
            done = self.xbee.check_done()
            self.xbee.send_pos(pos)                                               #send boat position to quad
            rospy.sleep(1)
        
        if (done == False):                                                   #means action was pre-empted by state machine, will tell quad that mission timed out
            self.xbee.timeout()
            self.server.set_preempted()
        else:                                                                 #success was reported by quad, will send
            self.server.set_succeeded()


server = LaunchQuadServer()
rospy.spin()
