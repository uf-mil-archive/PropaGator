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
def do_dock(nh, shape):

	if shape == 'circle':
		try:
			yield util.wrap_timeout(find_circle.main(nh), 40)
		except Exception:
			print "Could not find circle, moving to next shape"

	if shape == 'triangle':
		try:
			yield util.wrap_timeout(find_triangle.main(nh), 40)
		except Exception:
			traceback.print_exc()
			print "Could not find triangle, moving to next shape"

	if shape == 'cross':
		try:
			yield util.wrap_timeout(find_cross.main(nh), 40)
		except Exception:
			print "Could not find cross, moving to next shape"


@util.cancellableInlineCallbacks
def main(nh):


	try:
		yield util.wrap_timeout(do_dock(nh, 'circle'), 40)
	except Exception:
		traceback.print_exc()

	try:
		yield util.wrap_timeout(do_dock(nh, 'triangle'), 40)
	except Exception:
		traceback.print_exc()

	try:
		yield util.wrap_timeout(do_dock(nh, 'cross'), 40)
	except Exception:
		traceback.print_exc()
	
	
	