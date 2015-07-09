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


@util.cancellableInlineCallbacks
def main(nh):
    s =  yield json_server_proxy.get_server(nh)

    #url_was_set = s.interact('http://ec2-52-7-253-202.compute-1.amazonaws.com:80','openTest')
    course = raw_input('Course: ')
    url_was_set = yield s.interact('http://10.0.2.1:8080',course) 

    if url_was_set.was_set:

        print "Url and course were set succesfully"

        challenge_was_set = (yield s.set_current_challenge('gates')).was_set

        if challenge_was_set:

            print "Challenge was set succesfully"

            yield s.end_run()
            print 'Ended last run'
            run_started = (yield s.start_run()).success

            if run_started:

                print "Run started succesfully"

                images = yield s.get_server_images()
                print images

                dock = yield s.get_dock_info()

                print dock

                gates = yield s.get_gate_info()

                print gates

                test = yield s.send_buoy_info('blue')
                print test
    
                yield util.sleep(60)


    yield s.end_run()
    print 'Ended run'
    