#!/usr/bin/python

from __future__ import division
import rospy
import numpy as np
import math
from txros import util
import rospy
import boat_scripting

from boat_launch.mission import retract_hydrophone, deploy_hydrophone


@util.cancellableInlineCallbacks
def main(nh):
	boat = yield boat_scripting.get_boat(nh)


	yield deploy_hydrophone.main(nh)
	yield retract_hydrophone.main(nh)
