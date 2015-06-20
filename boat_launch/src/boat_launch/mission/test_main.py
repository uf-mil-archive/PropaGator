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
			print "Docking --- circle"
			yield util.wrap_timeout(find_circle.main(nh), 5)
		except Exception:
			print "Could not find circle, moving   to next shape"

	if shape == 'triangle':
		try:
			print "Docking --- triangle"
			yield util.wrap_timeout(find_triangle.main(nh), 5)
		except Exception:
			print "Could not find triangle, moving to next shape"

	if shape == 'cross':
		try:
			print "Docking --- cross"
			yield util.wrap_timeout(find_cross.main(nh), 5)
		except Exception:
			print "Could not find cross, moving to next shape"


@util.cancellableInlineCallbacks
def main(nh):
	try:
		yield do_dock(nh, 'circle')
	except Exception:
		traceback.print_exc()

	try:
		yield do_dock(nh, 'triangle')
	except Exception:
		traceback.print_exc()

	try:
		yield do_dock(nh, 'cross')
	except Exception:
		traceback.print_exc()
	
	
	