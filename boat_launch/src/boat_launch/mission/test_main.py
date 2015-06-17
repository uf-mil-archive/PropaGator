#!/usr/bin/python

from __future__ import division
import rospy
import numpy as np
import math
from txros import util
import rospy
import boat_scripting

from boat_launch.mission import deploy_hydrophone


@util.cancellableInlineCallbacks
def main(nh):
	boat = yield boat_scripting.get_boat(nh)

	#yield boat.deploy_hydrophone()
	#yield boat.retract_hydrophone()
