#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('ioboard')
import IOBoard
import actionlib
from ioboard.msg import IOBoardAction,IOBoardActionResult

rospy.init_node('motor_driver')

#ioboard = IOBoard()

class IOBoardServer:

 def __init__(self):
        self.server = actionlib.SimpleActionServer('ioboard_command', IOBoardAction, self.execute, False)
        self._result = IOBoardActionResult
        self.server.start()
        print "IO board server started"

 def execute(self,goal):

     if (goal.command == 'S'):
        #ioboard.shoot()
        print "shooting"
        self.server.set_succeeded() 
     elif (goal.command == 'T'):
        #self._result.temp = ioboard.read_temp()
        self._result.temp = "98"
        self.server.set_succeeded(self._result) 
     elif (goal.command == 'K'):
        #send kill to power router
        print 'killing'
        self.server.set_succeeded()
     else:
        self.server.set_aborted() 

server = IOBoardServer()
rospy.spin()
