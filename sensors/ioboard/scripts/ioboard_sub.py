#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('ioboard')
import IOBoard
import actionlib
from ioboard.msg import IOBoardAction,IOBoardActionResult

rospy.init_node('motor_driver')

global running = False
ioboard = IOBoard.IOBoard('IOBoard')

def kill_check(event):
        global running
        if (not(running)):
                if (ioboard.check_kill()):
                        #kill
                
rospy.Timer(rospy.Duration(.2),kill_check)


class IOBoardServer:

 def __init__(self):
        self.server = actionlib.SimpleActionServer('ioboard_command', IOBoardAction, self.execute, False)
        self._result = IOBoardActionResult
        
        self.server.start()
        print "IO board server started"

 def execute(self,goal):
     done = False
     global running 
     if (goal.command == 'Shoot3'):
        running = True
        while (not(self.server.is_preempt_requested()) and not(done)):
                ioboard.shoot(3)
                while (not(ioboard.shoot_status()[0])):
                            
                done = True
        if (done):
                self.server.set_succeeded()
        else:
                done = True
                self.server.set_preempted() 

     elif (goal.command == 'Shoot2'):
        running = True
        while (not(self.server.is_preempt_requested()) and not(done)):
                ioboard.shoot(2)
                while (not(ioboard.shoot_status())):{}   
                done = True
        if (done):
                self.server.set_succeeded()
        else:
                done = True
                self.server.set_preempted() 

     elif (goal.command == 'Temp'):
        ioboard.read_temp()
        running = True
        while (not(self.server.is_preempt_requested()) and not(done)):
                check = ioboard.temp_status()
                if(check):
                        self._result.temp = check
                        done = True
        self.server.set_succeeded(self._result)

     running = False 

server = IOBoardServer()
rospy.spin()

