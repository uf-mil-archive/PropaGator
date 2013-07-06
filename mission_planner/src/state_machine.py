#!/usr/bin/env python

import roslib
import rospy
roslib.load_manifest('mission_planner')
import smach,smach_ros
from geometry_msgs.msg import Point
from smach_ros import SimpleActionState
from rings.msg import ShootRingsAction,ShootRingsGoal
from path_planner.msg import TraverseBuoysAction,TraverseBuoysGoal
from buoys.msg import FindBuoysAction,FindBuoysGoal
from button.msg import FindButtonAction,FindButtonGoal
from gps_waypoints.msg import GoToWaypointAction,GoToWaypointGoal
from rawgps_common.gps import ecef_from_latlongheight,enu_from_ecef

runTime = 20 #minutes
ringsTimeout = 20000
buoysTimeout = 10
buttonTimeout = 100000

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



def read_task_pos(task):
        path = roslib.packages.resource_file('gps_waypoints','saved','task_loc.txt')

        with open(path,'r+') as task_loc:
           for line in task_loc:
                data = line.split(',')

                if data[0] == task:
                   return [float(data[1]),float(data[2]),float(data[3])]
        return ecef_from_latlongheight(1.0,2.0,3.0)

def main():
     
  rospy.init_node('state_machine')
        
#RINGS--------------------------------------------------------------------------------------------------
  rings_concurrence_red = smach.Concurrence(outcomes=['rings_done'],
                                          default_outcome = 'rings_done',
                                          child_termination_cb = lambda outcome_map : True,
                                          outcome_cb = lambda outcome_map : 'rings_done')
  with rings_concurrence_red:
          rings_goal = ShootRingsGoal()
          rings_goal.color = 'red'
          smach.Concurrence.add('RingsTask', SimpleActionState('shoot_rings',
                                          ShootRingsAction,
                                          goal=rings_goal))
                                 
          smach.Concurrence.add('RingsTimeout',sleep(ringsTimeout))

  rings_concurrence_purple = smach.Concurrence(outcomes=['rings_done'],
                                          default_outcome = 'rings_done',
                                          child_termination_cb = lambda outcome_map : True,
                                          outcome_cb = lambda outcome_map : 'rings_done')
  with rings_concurrence_purple:
          rings_goal = ShootRingsGoal()
          rings_goal.color = 'purple'
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
  with button_concurrence:
          smach.Concurrence.add('ButtonTask', SimpleActionState('find_button',
                                          FindButtonAction))
                                 
          smach.Concurrence.add('ButtonTimeout',sleep(buttonTimeout))


#-------------------------------------------------------------------------------------------------------

  sm = smach.StateMachine(outcomes = ['succeeded','aborted','preempted'])
  with sm:
          
          #smach.StateMachine.add('Buoys', buoys_concurrence, transitions={'buoys_done':'GoToRingsRed'})

          rings_pos_goal = GoToWaypointGoal()
          ring_pos = read_task_pos('rings')
          rings_pos_goal.waypoint = Point(x = ring_pos[0],y = ring_pos[1],z = ring_pos[2])
          smach.StateMachine.add('GoToRingsRed', SimpleActionState('go_waypoint',
                                         GoToWaypointAction,
                                          goal=rings_pos_goal),
                                          transitions={'succeeded':'RingsRed'})

          smach.StateMachine.add('RingsRed', rings_concurrence_red, transitions={'rings_done':'GoToButton'})
          '''
          rings_pos_goal = GoToWaypointGoal()
          ring_pos = read_task_pos('rings')
          rings_pos_goal.waypoint = Point(x = ring_pos[0],y = ring_pos[1],z = ring_pos[2])
          smach.StateMachine.add('GoToRingsPurple', SimpleActionState('go_waypoint',
                                         GoToWaypointAction,
                                          goal=rings_pos_goal),
                                          transitions={'succeeded':'RingsPurple'})

          smach.StateMachine.add('RingsPurple', rings_concurrence_purple, transitions={'rings_done':'GoToButton'})
          '''
          button_pos_goal = GoToWaypointGoal()
          button_pos = read_task_pos('button')
          button_pos_goal.waypoint = Point(x = button_pos[0],y = button_pos[1],z = button_pos[2])
          smach.StateMachine.add('GoToButton', SimpleActionState('go_waypoint',
                                         GoToWaypointAction,
                                          goal=button_pos_goal),
                                          transitions={'succeeded':'Button'})

          smach.StateMachine.add('Button', button_concurrence, transitions={'button_done':'GoToRingsRed'})
          '''
          spock_pos_goal = GoToWaypointGoal()
          spock_pos = read_task_pos('spock')
          spock_pos_goal.waypoint = Point(x = spock_pos[0],y = spock_pos[1],z = spock_pos[2])
          smach.StateMachine.add('GoToSpock', SimpleActionState('go_waypoint',
                                         GoToWaypointAction,
                                          goal=spock_pos_goal),
                                          transitions={'succeeded':'Spock'})

          smach.StateMachine.add('Spock',sleep(2),transitions={'succeeded':'Buoys','aborted':'Buoys'})
          '''

  sis = smach_ros.IntrospectionServer('mission_planner', sm, '/MISSIONS')
  sis.start()
  outcome = sm.execute()

  rospy.spin()
  sis.stop()


if __name__ == '__main__':
  main()

