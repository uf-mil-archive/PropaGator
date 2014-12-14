#!/usr/bin/env python
from __future__ import division

import rospy

from txros import util

import boat_scripting

#@util.cancellableInlineCallbacks
#def abcd():
#	boat = yield boat_scripting.get_boat(nh)

#	rospy.loginfo('Got a boat');


if __name__ == '__main__':
    try:
        #Setup ros
    	#rospy.init_node('LakeAliceTestNode')
    	nh = txros.NodeHandle.from_argv('LakeAliceTestNode', anonymous=True)
    	#abcd()

    except rospy.ROSInterruptException: pass
