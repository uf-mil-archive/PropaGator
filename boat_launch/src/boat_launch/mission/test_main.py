#!/usr/bin/python

from __future__ import division
import rospy
import numpy as np
import math
from txros import util
import rospy
import boat_scripting
import traceback
from boat_launch.mission import start_gate_vision, find_circle, find_cross, find_triangle, go_to_ecef_pos

SPEED = 1

ONE_MINUTE = 60

TOTAL TIME = 20 * ONE_MINUTE
CIRCLE_TIME = ONE_MINUTE
TRIANGLE_TIME = ONE_MINUTE
CROSS_TIME = ONE_MINUTE
ECEF_TIME = ONE_MINUTE
START_GATE_TIME = ONE_MINUTE

# LAKE WAYPOINTS

WAYPOINT_A = (29.641090, -82.361292)
WAYPOINT_B = (29.641260, -82.361197)
WAYPOINT_C = (29.641394, -82.361293)
WAYPOINT_D = (29.641651, -82.361444)
WAYPOINT_E = (29.641245, -82.361014)
WAYPOINT_F = (29.641360, -82.361104)
WAYPOINT_G = (29.641104, -82.361470)
WAYPOINT_H = (29.641274, -82.361403)

@util.cancellableInlineCallbacks
def do_dock(nh, shape):

	if shape == 'circle':
		try:
			print "Docking --- circle"
			yield util.wrap_timeout(find_circle.main(nh), CIRCLE_TIME)
		except Exception:
            yield boat.move.forward(-6).go()
			print "Could not find circle, moving  to next shape"

	if shape == 'triangle':
		try:
			print "Docking --- triangle"
			yield util.wrap_timeout(find_triangle.main(nh), TRIANGLE_TIME)
		except Exception:
            yield boat.move.forward(-6).go()
			print "Could not find triangle, moving to next shape"

	if shape == 'cross':
		try:
			print "Docking --- cross"
			yield util.wrap_timeout(find_cross.main(nh), CROSS_TIME)
		except Exception:
            yield boat.move.forward(-6).go()
			print "Could not find cross, moving to next shape"


@util.cancellableInlineCallbacks
def main(nh):

	print "Starting main mission"

	print "Moving to first position: ", WAYPOINT_A
	try:
		yield util.wrap_timeout(go_to_ecef_pos.main(nh, WAYPOINT_A, SPEED, False) ECEF_TIME)
		print "Arrived at first position"
	except Exception:
		print "Could not make it to first position in " + ECEF_TIME + " seconds"


	print "Starting speed gates now"
	try:

		start_gate = yield util.wrap_timeout(start_gate_vision.main(nh), ONE_MINUTE)
		print "Startgates completed succesfully!"
	except Exception:
		print "Could not complete stargates in" + START_GATE_TIME + " seconds"
 

	print "Moving to safe position now: ", WAYPOINT_F

	try:
		yield util.wrap_timeout(go_to_ecef_pos.main(nh, WAYPOINT_F, SPEED, False) ONE_MINUTE)
		print "Moved to " + WAYPOINT_F + ", now moving to " + WAYPOINT_E + " to begin docking"
	except Exception:
		print "Could not make it to second position in " + ONE_MINUTE + " seconds"

	try:
		yield util.wrap_timeout(go_to_ecef_pos.main(nh, WAYPOINT_E, SPEED, False) ONE_MINUTE)
		print "Moved to " + WAYPOINT_E + ", starting docking challenge"
	except Exception:
		print "Could not make it to third position in " + ONE_MINUTE + " seconds"

	try:
		yield do_dock(nh, 'circle')
	except Exception:
		pass
		#traceback.print_exc()

	try:
		yield do_dock(nh, 'triangle')
	except Exception:
		pass
		#traceback.print_exc()

	try:
		yield do_dock(nh, 'cross')
	except Exception:
		pass
		#traceback.print_exc()


	print "Docking complete"
	print "Moving back to startate begining position", WAYPOINT_A

	try:
		yield util.wrap_timeout(go_to_ecef_pos.main(nh, WAYPOINT_A, SPEED, False) ONE_MINUTE)
		print "Arrived at ", WAYPOINT_A
	except Exception:
		print "Could not make it to third position in " + ONE_MINUTE + " seconds"


	
	
	
	