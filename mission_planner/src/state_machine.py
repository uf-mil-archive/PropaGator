#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('mission_planner')
import smach,smach_ros
from smach_ros import SimpleActionState
from rings.msg import ShootRingsAction,ShootRingsGoal


class sleep(smach.State):
   def __init__(self,sleep_time):
          smach.State.__init__(self, outcomes=['succeeded','aborted'])
          self.sleep_time = sleep_time
   def execute(self,data):
        self.timer = 0
        while(self.timer/10 < self.sleep_time):
            rospy.sleep(.1)
            self.timer = self.timer + 1
            if (self.preempt_requested()):
                self.service_preempt()
                return 'aborted'
        return 'succeeded'
                

def main():
  rospy.init_node('state_machine')

  rings_concurrence = smach.Concurrence(outcomes=['rings_done'],
                                          default_outcome = 'rings_done',
                                          child_termination_cb = lambda outcome_map : True,
                                          outcome_cb = lambda outcome_map : 'rings_done')
  with rings_concurrence:
          rings_goal = ShootRingsGoal()
          rings_goal.attempts = 5
          smach.Concurrence.add('RingsTask', SimpleActionState('shoot_rings',
                                          ShootRingsAction,
                                          goal=rings_goal))
                                 
          smach.Concurrence.add('RingsTimeout',sleep(15))

  sm = smach.StateMachine(outcomes = ['rings_done','succeeded','aborted','preempted'])
  with sm:
          smach.StateMachine.add('Rings', rings_concurrence, transitions={'rings_done':'Button'})
          smach.StateMachine.add('Button',sleep(2),transitions={'succeeded':'Signals','aborted':'Signals'})
          smach.StateMachine.add('Signals',sleep(2),transitions={'succeeded':'Rings','aborted':'Rings'})

  sis = smach_ros.IntrospectionServer('mission_planner', sm, '/MISSIONS')
  sis.start()
  outcome = sm.execute()

  rospy.spin()
  sis.stop()


if __name__ == '__main__':
  main()

