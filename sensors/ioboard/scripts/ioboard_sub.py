#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('ioboard')
import IOBoard
import actionlib
from ioboard.msg import IOBoardAction,IOBoardActionResult

rospy.init_node('motor_driver')




class IOBoardServer:

 def __init__(self):
        self.server = actionlib.SimpleActionServer('ioboard_command', IOBoardAction, self.execute, False)
        self._result = IOBoardActionResult
        self.ioboard = IOBoard.IOBoard('ttyUSB0')
        self.server.start()
        print "IO board server started"

 def execute(self,goal):
     done = False

     if (goal.command == 'Shoot3'):
        while (not(self.server.is_preempt_requested()) and not(done)):
                self.ioboard.shoot(3)
                while (not(self.ioboard.shoot_status())):{}     
                done = True
        if (done):
                self.server.set_succeeded()
        else:
                done = True
                self.server.set_preempted() 

     elif (goal.command == 'Shoot2'):
        while (not(self.server.is_preempt_requested()) and not(done)):
                self.ioboard.shoot(2)
                while (not(self.ioboard.shoot_status())):{}   
                done = True
        if (done):
                self.server.set_succeeded()
        else:
                done = True
                self.server.set_preempted() 

     elif (goal.command == 'Temp'):
        self._result.temp = self.ioboard.read_temp()
        self.server.set_succeeded(self._result) 

     elif (goal.command == 'Kill'):
        #send kill to power router
        print 'killing'
        self.server.set_succeeded()

     else:
        self.server.set_aborted() 

server = IOBoardServer()
rospy.spin()

