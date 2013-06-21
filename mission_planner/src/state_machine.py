#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('mission_planner')
import smach,smach_ros
from smach_ros import SimpleActionState
from rings.msg import ShootRingsAction,ShootRingsGoal


class sleep(smach.State):
   def __init__(self,time):
          smach.State.__init__(self, outcomes=['rings_timeout'])
          self.time = time
   def execute(self,data):
          self.timer = 0 
          while (self.timer/10 < self.time):
                  rospy.sleep(.1)
                  if (self.server.is_preempt_requested()):
                        break
                  self.timer = self.timer + 1
          return 'timeout'

  
  
def main():
  rospy.init_node('state_machine')

  rings_concurrence = smach.Concurrence(outcomes=['rings_done'],
                                          default_outcome = 'rings_done',
                                          child_termination_cb = lambda outcome_map : True,
                                          outcome_cb = lambda outcome_map : 'rings_done')
  with rings_concurrence:
          rings_goal = ShootRingsGoal()
          smach.Concurrence.add('Rings', SimpleActionState('shoot_rings',
                                          ShootRingsAction,
                                          goal=rings_goal))
                                 
          smach.Concurrence.add('RingsTimeout',sleep(15))

  sm = smach.StateMachine(outcomes = ['button','signals','rings_done','done','succeeded','aborted','preempted'])
  with sm:
          smach.StateMachine.add('Rings', rings_concurrence, transitions={'rings_done':'Button'})
          smach.StateMachine.add('Button',sleep(2),transitions={'signals':'Signals'})
          smach.StateMachine.add('Signals',sleep(2),transitions={'rings':'Rings'})

  sis = smach_ros.IntrospectionServer('mission_planner', sm, '/MISSIONS')
  sis.start()
  outcome = sm.execute()

  rospy.spin()
  sis.stop()


if __name__ == '__main__':
  main()

