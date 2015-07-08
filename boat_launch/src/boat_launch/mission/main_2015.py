#!/usr/bin/python

from __future__ import division
import rospy
import numpy as np
import math
import txros
from txros import util
import rospy
import boat_scripting
import traceback
from rawgps_common import gps
from server_interaction import json_server_proxy

# Sub mission imports
from boat_launch.mission import start_gate_laser, find_shape, go_to_ecef_pos

# Timeout limits 
ONE_MINUTE = 60

TOTAL_TIME = 20 * ONE_MINUTE
#DOCK_TIME = 
#NTROP_TIME = 
#OBSTICAL_COURSE_TIME = 
#START_GATE_TIME = 

# TODO: Move this to some common place to be used across files
def ll(lat, lon):
    return gps.ecef_from_latlongheight(math.radians(lat), math.radians(lon), 0)

# WAYPOINT DEFINES


@util.cancellableInlineCallbacks
def main(nh):
    # Grab interfaces for boat and JSON server
    boat = yield boat_scripting.get_boat(nh)
    s =  yield json_server_proxy.get_server(nh)

    # Grab mission input such as JSON server ip, and course
    ip_port = raw_input('Enter ip:port (ex. 10.0.2.1:8080): ')
    course = raw_input('Enter course with corect spelling!(courseA, courseB, ...): ')


    # Giant try finally to make sure boat ends run and returns to its default state
    try:
        # Main mission code

        # JSON initilization
        # TODO: if any failures start over
        url_was_set = (yield s.interact('http://'+ip_port, course)).was_set
        assert not url_was_set, 'Failed to set URL to ' + 'http://'+ip_port + ' on course ' + course

        print "Url and course were set succesfully"

        # Set the gate challange as the first challange
        challenge_was_set = (yield s.set_current_challenge('gates')).was_set
        assert not challenge_was_set, 'Failed to set current challange to gates'

        print "Challenge was set succesfully"

        # end run just in case
        end_run = yield s.end_run()
        run_started = (yield s.start_run()).success

        assert not run_started, 'Run failed to start'
        print "Run started succesfully"


        # Sub-missions should be wraped in timeouts to prevent any one task from eating all time


    finally:
        # We do not yield here because if something hangs we still want everything else to complete
        print 'Ending run and returning to default state'
        boat.default_state()
        s.end_run()