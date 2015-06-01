from __future__ import division
from txros import util
import boat_scripting
import station_hold
from std_msgs.msg import Bool 
import rospy

found_point = False

def callback(msg):
	global found_point
	found_point = msg.data

rospy.Subscriber = ("circle_detected" , Bool, callback)

@util.cancellableInlineCallbacks
def main(nh):
    global found_point
    boat = yield boat_scripting.get_boat(nh)
    '''
    while found_point == False:
    	print 'Turning'
    	yield boat.move.turn_left_deg(10).go()
    '''

   # station_hold.hold_at_current_pos()



