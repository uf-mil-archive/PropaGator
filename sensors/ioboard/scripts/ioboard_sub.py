#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('ioboard')
import IOBoard
import actionlib
from ioboard.msg import IOBoardAction,IOBoardActionResult

rospy.init_node('motor_driver')

global running = False





class IOBoardServer:

 def __init__(self):
        self.server = actionlib.SimpleActionServer('ioboard_command', IOBoardAction, self.execute, False)
        self._result = IOBoardActionResult
        self.ioboard = IOBoard.IOBoard('IOBoard')
        self.killservice = rospy.ServiceProxy('/power_router/setkill', SetKill)
        self.server.start()
        print "IO board server started"

 def execute(self,goal):
     done = False
     global running 
     if (goal.command == 'Shoot3'):
        running = True
        while (not(self.server.is_preempt_requested()) and not(done)):
                self.ioboard.shoot(3)
                check = self.ioboard.check_port()
                while (check != 'S'):
                        if (check == 'E'):
                           self.killservice(True)
                        check = self.ioboard.check_port()           
                done = True
        if (done):
                self.server.set_succeeded()
        else:
                done = True
                self.server.set_preempted() 

     elif (goal.command == 'Shoot2'):
        running = True
        while (not(self.server.is_preempt_requested()) and not(done)):
                self.ioboard.shoot(3)
                check = self.ioboard.check_port()
                while (check != 's'):
                        if (check == 'E'):
                           self.killservice(True)
                        check = self.ioboard.check_port()           
                done = True
        if (done):
                self.server.set_succeeded()
        else:
                done = True
                self.server.set_preempted()
     elif (goal.command == 'Temp'):
        self.ioboard.read_temp()
        running = True
        while (not(self.server.is_preempt_requested()) and not(done)):
                check = self.ioboard.check_port()
                while (check == [] or check == 'E'):
                        if (check == 'E'):
                           self.killservice(True)
                        check = self.ioboard.check_port()           
                done = True
        if (done):
                self._result.temp = check
                self.server.set_succeeded(self._result)
        else:
                done = True
                self.server.set_preempted()
     running = False 

server = IOBoardServer()
rospy.spin()

