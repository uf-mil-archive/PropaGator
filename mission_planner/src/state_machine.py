#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('mission_planner')
import smach,smach_ros
from smach_ros import SimpleActionState
from rings.msg import ShootRingsAction,ShootRingsGoal
from path_planner.msg import TraverseBuoysAction,TraverseBuoysGoal
from buoys.msg import FindBuoysAction,FindBuoysGoal
from button.msg import FindButtonAction,FindButtonGoal

ringsTimeout = 20
buoysTimeout = 10
buttonTimeout = 10

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
        
#RINGS--------------------------------------------------------------------------------------------------
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
                                 
          smach.Concurrence.add('RingsTimeout',sleep(ringsTimeout))

        
#BUOYS--------------------------------------------------------------------------------------------------
  buoys_concurrence = smach.Concurrence(outcomes=['buoys_done'],
                                          default_outcome = 'buoys_done',
                                          child_termination_cb = lambda outcome_map : True,
                                          outcome_cb = lambda outcome_map : 'buoys_done')
  with buoys_concurrence:
          smach.Concurrence.add('BuoysTask', SimpleActionState('traverse_buoys',
                                          TraverseBuoysAction))
          smach.Concurrence.add('BuoysFinder', SimpleActionState('find_buoys',
                                          FindBuoysAction))
                                 
          smach.Concurrence.add('BuoysTimeout',sleep(buoysTimeout))

        
#BUTTON--------------------------------------------------------------------------------------------------
  button_concurrence = smach.Concurrence(outcomes=['button_done'],
                                          default_outcome = 'button_done',
                                          child_termination_cb = lambda outcome_map : True,
                                          outcome_cb = lambda outcome_map : 'button_done')
  with buoys_concurrence:
          smach.Concurrence.add('ButtonTask', SimpleActionState('find_button',
                                          FindButtonAction))
                                 
          smach.Concurrence.add('ButtonTimeout',sleep(buttonTimeout))


#-------------------------------------------------------------------------------------------------------

  sm = smach.StateMachine(outcomes = ['rings_done','buoys_done','succeeded','aborted','preempted'])
  with sm:
          
          smach.StateMachine.add('Buoys', buoys_concurrence, transitions={'buoys_done':'Rings'})
          smach.StateMachine.add('Rings', rings_concurrence, transitions={'rings_done':'Button'})
          smach.StateMachine.add('Button', button_concurrence, transitions={'button_done':'Signals'})
          smach.StateMachine.add('Signals',sleep(2),transitions={'succeeded':'Buoys','aborted':'Buoys'})

  sis = smach_ros.IntrospectionServer('mission_planner', sm, '/MISSIONS')
  sis.start()
  outcome = sm.execute()

  rospy.spin()
  sis.stop()


if __name__ == '__main__':
  main()

