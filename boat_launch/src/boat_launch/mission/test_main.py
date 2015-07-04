#!/usr/bin/python

from __future__ import division
import rospy
import numpy as np
import math
from txros import util
import rospy
import boat_scripting
import traceback
from boat_launch.mission import start_gate_laser, find_shape go_to_ecef_pos
from rawgps_common import gps
from server_interaction import json_server

SPEED = 1

ONE_MINUTE = 60

TOTAL_TIME = 20 * ONE_MINUTE
CIRCLE_TIME = ONE_MINUTE
TRIANGLE_TIME = ONE_MINUTE
CROSS_TIME = ONE_MINUTE
ECEF_TIME = 2 * ONE_MINUTE
START_GATE_TIME = ONE_MINUTE

NORTH = np.pi/2
EAST = 0
SOUTH = (3*np.pi)/2
WEST = np.pi

# TODO: Move this to some common place to be used across files
def ll(lat, lon):
    return gps.ecef_from_latlongheight(math.radians(lat), math.radians(lon), 0)

# LAKE WAYPOINTS

WAYPOINT_A = ll(29.641090, -82.361292)
WAYPOINT_B = ll(29.641260, -82.361197)
WAYPOINT_C = ll(29.641394, -82.361293)
WAYPOINT_D = ll(29.641651, -82.361444)
WAYPOINT_E = ll(29.641245, -82.361014)
WAYPOINT_F = ll(29.641360, -82.361104)
WAYPOINT_G = ll(29.641104, -82.361470)
WAYPOINT_H = ll(29.641274, -82.361403)

@util.cancellableInlineCallbacks
def do_dock(nh, shape):
	boat = yield boat_scripting.get_boat(nh)
	if shape == 'circle':
		try:
			print "Docking --- circle"
			yield util.wrap_timeout(find_shape.main(nh, 'circle'), CIRCLE_TIME)
		except Exception:
			print "Could not find circle, moving  to next shape"

	if shape == 'triangle':
		try:
			print "Docking --- triangle"
			yield util.wrap_timeout(find_shape.main(nh, 'triangle'), TRIANGLE_TIME)
		except Exception:
			print "Could not find triangle, moving to next shape"

	if shape == 'cross':
		try:
			print "Docking --- cross"
			yield util.wrap_timeout(find_shape.main(nh, 'cross'), CROSS_TIME)
		except Exception:
			print "Could not find cross, moving to next shape"


@util.cancellableInlineCallbacks
def main(nh):

	assert TOTAL_TIME > CIRCLE_TIME + CROSS_TIME + TRIANGLE_TIME + (ECEF_TIME * 4) + START_GATE_TIME

	'''

	Test Main Mission:

	1. Go to point A and turn North, then begin speed gate challenge - One minute timeout
	2. Do speedgates, then go to point F and turn South - One minute timeout
	3. Go to position E to begin docking challenges - One minute each
	4. When done with docking move back to point A and turn north

	SPEED ONLY SET TO ONE TO START

	'''

	boat = yield boat_scripting.get_boat(nh)

	s = json_server.server_interaction('http://ec2-52-7-253-202.compute-1.amazonaws.com:80','courseA')

	s.set_current_challenge('gates')

	s.start_run()

	print "Starting main mission"

	print "Moving to first position: ", WAYPOINT_A

	try:
		s.set_current_challenge('gates')
		yield util.wrap_timeout(go_to_ecef_pos.main(nh, WAYPOINT_A, SPEED), ECEF_TIME)
		yield boat.move.heading(NORTH).go()
		print "Arrived at first position"
	except Exception:
		print "Could not make it to first position in " + str(ECEF_TIME) + " seconds"


	print "Starting speed gates now"
	try:
		s.set_current_challenge('gates')
		start_gate = yield util.wrap_timeout(start_gate_laser.main(nh), ONE_MINUTE)
		print "Startgates completed succesfully!"
	except Exception:
		print "Could not complete stargates in" + str(START_GATE_TIME) + " seconds"
 

	print "Moving to safe position now: ", WAYPOINT_F

	try:
		# boat.set_current_challenge('Go to point F')
		yield util.wrap_timeout(go_to_ecef_pos.main(nh, WAYPOINT_F, SPEED), ONE_MINUTE)
		yield boat.move.heading(SOUTH).go()
		print "Moved to " + WAYPOINT_F + ", now moving to " + str(WAYPOINT_E) + " to begin docking"
	except Exception:
		print "Could not make it to second position in " + str(ONE_MINUTE) + " seconds"

	try:
		# boat.set_current_challenge('Go to point E')
		yield util.wrap_timeout(go_to_ecef_pos.main(nh, WAYPOINT_E, SPEED), ONE_MINUTE)
		yield boat.move.heading(EAST).go()
		print "Moved to " + WAYPOINT_E + ", starting docking challenge"
	except Exception:
		print "Could not make it to third position in " + ONE_MINUTE + " seconds"

	try:
		s.set_current_challenge('docking')
		yield do_dock(nh, 'circle')
	except Exception:
		pass
		#traceback.print_exc()

	try:
		s.set_current_challenge('docking')
		yield do_dock(nh, 'triangle')
	except Exception:
		pass
		#traceback.print_exc()

	try:
		s.set_current_challenge('docking')
		yield do_dock(nh, 'cross')
	except Exception:
		pass
		#traceback.print_exc()



	print "Docking complete"
	print "Moving back to startate begining position", WAYPOINT_A

	try:
		s.set_current_challenge('return')
		yield util.wrap_timeout(go_to_ecef_pos.main(nh, WAYPOINT_A, SPEED), ONE_MINUTE)
		yield boat.move.heading(NORTH).go()
		print "Arrived at ", WAYPOINT_A
	except Exception:
		print "Could not make it to third position in " + str(ONE_MINUTE) + " seconds"

	
	
	
	