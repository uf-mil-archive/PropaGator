#!/usr/bin/python

from __future__ import division
import rospy
import numpy as np
import math
from txros import util
import rospy
import boat_scripting
import traceback
from boat_launch.mission import retract_hydrophone, deploy_hydrophone, find_circle, find_cross, find_triangle

@util.cancellableInlineCallbacks
def do_dock(nh, boat, shape):

	if shape == 'circle':
		try:
			print "Docking --- circle"
			yield util.wrap_timeout(find_circle.main(nh, boat), 5)
		except Exception:
			print "Could not find circle, moving to next shape"

	if shape == 'triangle':
		try:
			print "Docking --- triangle"
			yield util.wrap_timeout(find_triangle.main(nh, boat), 5)
		except Exception:
			print "Could not find triangle, moving to next shape"

	if shape == 'cross':
		try:
			print "Docking --- cross"
			yield util.wrap_timeout(find_cross.main(nh, boat), 5)
		except Exception:
			print "Could not find cross, moving to next shape"


@util.cancellableInlineCallbacks
def main(nh):
	boat = yield boat_scripting.get_boat(nh)

	try:
		yield util.wrap_timeout(do_dock(nh, boat, 'circle'), 5)
	except Exception:
		traceback.print_exc()

	try:
		yield util.wrap_timeout(do_dock(nh, boat, 'triangle'), 5)
	except Exception:
		traceback.print_exc()

	try:
		yield util.wrap_timeout(do_dock(nh, boat, 'cross'), 5)
	except Exception:
		traceback.print_exc()
	
	
	