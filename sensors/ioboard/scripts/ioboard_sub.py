#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('ioboard')
import IOBoard
import actionlib
from ioboard.msg import IOBoardAction,IOBoardActionResult
from power_router.srv import SetKill

rospy.init_node('motor_driver')

ioboard = IOBoard.IOBoard('IOBoard')
killservice = rospy.ServiceProxy('/power_router/setkill', SetKill)

global running
running = False

def kill_callback(event):
    if (not(running)):
        data = ioboard.check_port()
        if (len(data) > 0):
            if (data[0] == 'E'):
                print 'kill'
                killservice(True)

rospy.Timer(rospy.Duration(.01),kill_callback)

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
                check = ioboard.check_port()
                while (len(check) == 0):
                    check = ioboard.check_port()
                if (check == 'E'):
                    killservice(True)
                done = True
            if (done):
                self.server.set_succeeded()
            else:
                done = True
                self.server.set_preempted()
        
        elif (goal.command == 'Shoot2'):
            running = True
            while (not(self.server.is_preempt_requested()) and not(done)):
                ioboard.shoot(3)
                check = ioboard.check_port()
                
                while (len(check) == 0):
                    check = ioboard.check_port()
                if (check[0] == 'E'):
                    killservice(True)
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
                check = ioboard.check_port()
                while (len(check) == 0):
                    check = ioboard.check_port()
                if (check[0] == 'E'):
                    killservice(True)
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
