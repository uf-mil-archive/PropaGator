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
from quad.msg import LaunchQuadAction,LaunchQuadGoal
from gps_waypoints.msg import GoToWaypointAction,GoToWaypointGoal
from rawgps_common.gps import ecef_from_latlongheight,enu_from_ecef

runTime = 200 #minutes
ringsTimeout  = (5.0/20)*runTime*60
buoysTimeout  = (3.0/20)*runTime*60
buttonTimeout = (5.0/20)*runTime*60
quadTimeout   = (4.0/20)*runTime*60

print "buoys timeout: ",buoysTimeout
print "rings timeout: ",ringsTimeout
print "button timeout: ",buttonTimeout
print "quad timeout: ",quadTimeout

course = 'b'

def read_task_pos(task):
    path = roslib.packages.resource_file('gps_waypoints','saved','task_loc_'+course+'.txt')
    
    with open(path,'r+') as task_loc:
        for line in task_loc:
            data = line.split(',')
            
            if data[0] == task:
                return [float(data[1]),float(data[2]),float(data[3])]
    return ecef_from_latlongheight(1.0,2.0,3.0)

if (course == 'a'):
    ring_offset = [-4,4]
    ring_orientation = [1,-1]
    
    button_offset = [4,-4]
    button_orientation = [0,1]
    
    spock_offset = [4,4]
    spock_orientation = [1,1]
    
    quad_offset = [0,0]
elif (course == 'b'):
    ring_offset = [0,-4]
    ring_orientation = [0,1]
    
    button_offset = [4,-4]
    button_orientation = [-1,1]
    
    spock_offset = [4,4]
    spock_orientation = [1,1]
    
    quad_offset = [0,0]

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
    rings_concurrence_red = smach.Concurrence(outcomes=['rings_done'],
        default_outcome = 'rings_done',
        child_termination_cb = lambda outcome_map : True,
        outcome_cb = lambda outcome_map : 'rings_done')
    with rings_concurrence_red:
        rings_goal = ShootRingsGoal()
        rings_goal.color = 'red'
        rings_goal.orientation = [ring_orientation[0],ring_orientation[1],0]
        smach.Concurrence.add('RingsTask', SimpleActionState('shoot_rings',
            ShootRingsAction,
            goal=rings_goal))
        
        smach.Concurrence.add('RingsTimeout',sleep(ringsTimeout))
    
    rings_concurrence_orange = smach.Concurrence(outcomes=['rings_done'],
        default_outcome = 'rings_done',
        child_termination_cb = lambda outcome_map : True,
        outcome_cb = lambda outcome_map : 'rings_done')
    with rings_concurrence_orange:
        rings_goal = ShootRingsGoal()
        rings_goal.color = 'orange'
        rings_goal.orientation = [ring_orientation[0],ring_orientation[1],0]
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
        #smach.Concurrence.add('BuoysFinder', SimpleActionState('find_buoys',
        #FindBuoysAction))
        
        smach.Concurrence.add('BuoysTimeout',sleep(buoysTimeout))
    
    
    #BUTTON--------------------------------------------------------------------------------------------------
    button_concurrence_left = smach.Concurrence(outcomes=['button_done'],
        default_outcome = 'button_done',
        child_termination_cb = lambda outcome_map : True,
        outcome_cb = lambda outcome_map : 'button_done')
    with button_concurrence_left:
        button_goal = FindButtonGoal()
        button_goal.side = 'left'
        button_goal.orientation = [button_orientation[0],button_orientation[1],0]
        smach.Concurrence.add('ButtonTask', SimpleActionState('find_button',
            FindButtonAction,
            goal=button_goal))
        
        smach.Concurrence.add('ButtonTimeout',sleep(buttonTimeout))
    
    button_concurrence_right = smach.Concurrence(outcomes=['button_done'],
        default_outcome = 'button_done',
        child_termination_cb = lambda outcome_map : True,
        outcome_cb = lambda outcome_map : 'button_done')
    with button_concurrence_right:
        button_goal = FindButtonGoal()
        button_goal.side = 'right'
        button_goal.orientation = [button_orientation[0],button_orientation[1],0]
        smach.Concurrence.add('ButtonTask', SimpleActionState('find_button',
            FindButtonAction,
            goal=button_goal))
        
        smach.Concurrence.add('ButtonTimeout',sleep(buttonTimeout))
    
    #QUAD--------------------------------------------------------------------------------------------------
    '''
    quad_goal = LaunchQuadGoal
    quad_goal_coord = read_task_pos('dock')
    quad_goal.lat = quad_goal_coord[0]
    quad_goal.long = quad_goal_coord[1]
    quad_goal.alt = quad_goal_coord[2]
    quad_concurrence = smach.Concurrence(outcomes=['quad_done'],
        default_outcome = 'quad_done',
        child_termination_cb = lambda outcome_map : True,
        outcome_cb = lambda outcome_map : 'quad_done')
    with button_concurrence:
        smach.Concurrence.add('QuadTask', SimpleActionState('launch_quad',
            LaunchQuadAction,
            goal=quad_goal))
        
        smach.Concurrence.add('QuadTimeout',sleep(quadTimeout))
    '''
    
    #-------------------------------------------------------------------------------------------------------
    
    sm = smach.StateMachine(outcomes = ['succeeded','aborted','preempted'])
    with sm:
        
        safe_pos_goal = GoToWaypointGoal()
        safe_pos = read_task_pos('buoys')
        safe_pos_goal.waypoint = Point(x = safe_pos[0],y = safe_pos[1],z = safe_pos[2])
        safe_pos_goal.offset = Point(x = 0,y = 0,z = 0)
        
        smach.StateMachine.add('Buoys', buoys_concurrence, transitions={'buoys_done':'GoToSafeZone0'})
        
        smach.StateMachine.add('GoToSafeZone0', SimpleActionState('go_waypoint',
            GoToWaypointAction,
                goal=safe_pos_goal),
                transitions={'succeeded':'GoToRingsRed'})
        
        rings_pos_goal = GoToWaypointGoal()
        ring_pos = read_task_pos('rings')
        rings_pos_goal.waypoint = Point(x = ring_pos[0],y = ring_pos[1],z = ring_pos[2])
        rings_pos_goal.offset = Point(x = ring_offset[0],y = ring_offset[1],z = 0)
        smach.StateMachine.add('GoToRingsRed', SimpleActionState('go_waypoint',
            GoToWaypointAction,
                goal=rings_pos_goal),
                transitions={'succeeded':'RingsRed'})
        
        smach.StateMachine.add('RingsRed', rings_concurrence_red, transitions={'rings_done':'RingsOrange'})
        smach.StateMachine.add('RingsOrange', rings_concurrence_orange, transitions={'rings_done':'GoToSafeZone1'})
        
        smach.StateMachine.add('GoToSafeZone1', SimpleActionState('go_waypoint',
            GoToWaypointAction,
                goal=safe_pos_goal),
                transitions={'succeeded':'GoToButton'})
        
        button_pos_goal = GoToWaypointGoal()
        button_pos = read_task_pos('button')
        button_pos_goal.waypoint = Point(x = button_pos[0],y = button_pos[1],z = button_pos[2])
        button_pos_goal.offset = Point(x = button_offset[0],y = button_offset[1],z = 0)
        smach.StateMachine.add('GoToButton', SimpleActionState('go_waypoint',
            GoToWaypointAction,
                goal=button_pos_goal),
                transitions={'succeeded':'ButtonRight'})
        
        smach.StateMachine.add('ButtonRight', button_concurrence_right, transitions={'button_done':'ButtonLeft'})
        smach.StateMachine.add('ButtonLeft', button_concurrence_left, transitions={'button_done':'GoToSafeZone2'})
        
        smach.StateMachine.add('GoToSafeZone2', SimpleActionState('go_waypoint',
            GoToWaypointAction,
                goal=safe_pos_goal),
                transitions={'succeeded':'GoToSpock'})
        
        '''
        quad_pos_goal = GoToWaypointGoal()
        quad_pos = read_task_pos('quad')
        quad_pos_goal.waypoint = Point(x = quad_pos[0],y = quad_pos[1],z = quad_pos[2])
        quad_pos_goal.offset = Point(x = quad_offset[0],y = quad_offset[1],z = 0)
        smach.StateMachine.add('GoToQuad', SimpleActionState('go_waypoint',
            GoToWaypointAction,
                goal=quad_pos_goal),
                transitions={'succeeded':'Quad'})
        
        smach.StateMachine.add('Quad', quad_concurrence, transitions={'quad_done':'GoToSpock'})
        '''
        
        spock_pos_goal = GoToWaypointGoal()
        spock_pos = read_task_pos('spock')
        spock_pos_goal.waypoint = Point(x = spock_pos[0],y = spock_pos[1],z = spock_pos[2])
        spock_pos_goal.offset = Point(x = spock_offset[0],y = spock_offset[1],z = 0)
        smach.StateMachine.add('GoToSpock', SimpleActionState('go_waypoint',
            GoToWaypointAction,
                goal=spock_pos_goal),
                transitions={'succeeded':'Spock'})
        
        smach.StateMachine.add('Spock',sleep(2),transitions={'succeeded':'GoToSafeZone3','aborted':'GoToSafeZone3'})
        
        smach.StateMachine.add('GoToSafeZone3', SimpleActionState('go_waypoint',
            GoToWaypointAction,
                goal=safe_pos_goal),
                transitions={'succeeded':'GoToDock'})
        
        dock_pos_goal = GoToWaypointGoal()
        dock_pos = read_task_pos('dock')
        dock_pos_goal.waypoint = Point(x = ring_pos[0],y = ring_pos[1],z = ring_pos[2])
        smach.StateMachine.add('GoToDock', SimpleActionState('go_waypoint',
            GoToWaypointAction,
                goal=rings_pos_goal),
                transitions={'succeeded':'DONE'})
        
        smach.StateMachine.add('DONE',sleep(2),transitions={'succeeded':'DONE','aborted':'DONE'})
    
    sis = smach_ros.IntrospectionServer('mission_planner', sm, '/MISSIONS')
    sis.start()
    outcome = sm.execute()
    
    rospy.spin()
    sis.stop()


if __name__ == '__main__':
    main()
