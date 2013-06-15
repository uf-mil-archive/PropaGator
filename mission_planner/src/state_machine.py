#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('mission_planner')
import smach,smach_ros
from smach_ros import SimpleActionState
from rings.msg import ShootRingsAction,ShootRingsGoal


class button(smach.State):
 def __init__(self):
        smach.State.__init__(self, outcomes=['signals'])
 def execute(self,data):
        rospy.sleep(1) 
        return 'signals'

class signals(smach.State):
 def __init__(self):
        smach.State.__init__(self, outcomes=['rings'])
 def execute(self,data):
        rospy.sleep(1) 
        return 'rings'


def main():
        rospy.init_node('state_machine')

        sm = smach.StateMachine(outcomes = ['button','signals','rings','done','succeeded','aborted','preempted'])

        with sm:
                rings_goal = ShootRingsGoal()
                rings_goal.attempts = 5
                smach.StateMachine.add('Rings', SimpleActionState('shoot_rings',
                                                ShootRingsAction,
                                                goal=rings_goal),
                                                transitions={'succeeded':'Button'})
                smach.StateMachine.add('Button',button(),transitions={'signals':'Signals'})
                smach.StateMachine.add('Signals',signals(),transitions={'rings':'Rings'})

        outcome = sm.execute()


if __name__ == '__main__':
        main()
