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
from boat_launch.mission import start_gate_laser, find_shape, go_to_ecef_pos, acoustic_beacon

# Timeout limits 
ONE_MINUTE = 60

TOTAL_TIME = 20 * ONE_MINUTE
DOCK_TIME = ONE_MINUTE
NTROP_TIME = ONE_MINUTE
OBS_COURSE_TIME = ONE_MINUTE
START_GATE_TIME = ONE_MINUTE
HYDRO_TIME = ONE_MINUTE * 2

# TODO: Move this to some common place to be used across files
def ll(lat, lon):
    return gps.ecef_from_latlongheight(math.radians(lat), math.radians(lon), 0)

# WAYPOINT DEFINES
STARTGATES_A = ll( 36.801843,-76.190816)
STARTGATES_B = ll(36.801786, -76.190831)

DOCK_A = ll(36.802122, -76.191344)
DOCK_B = ll(36.801754, -76.191911)

OBSTACLE_A = ll(36.80190, -76.19136)
OBSTACLE_B = ll(36.80189, -76.19139)

HYDRO_A = ll(36.802553, -76.191496)
HYDRO_B = ll(36.801983, -76.192111)

QUAD_A = ll(36.801925, -76.192314)
QUAD_B = ll(36.802398, -76.191437)

SAFE_ZONE_A1 = ll(36.802300, -76.191499)
SAFE_ZONE_B1 = ll(36.801977, -76.191909)

HOME_1 = ll(36.802105, -76.191676)
HOME_2 = ll(36.801947, -76.191334)
HOME_3 = ll(36.801805, -76.190871)


@util.cancellableInlineCallbacks
def main(nh):
    # Grab interfaces for boat and JSON server
    boat = yield boat_scripting.get_boat(nh)
    s =  yield json_server_proxy.get_server(nh)

    # Grab mission input such as JSON server ip, and course
    ip_port = raw_input('Enter ip:port (ex. 10.0.2.1:8080): ')
    course = raw_input('Enter course with corect spelling! (courseA, courseB, ...): ')

    shape1 = None
    shape2 = None
    color1 = None
    color2 = None

    boat.float_off()

    # Giant try finally to make sure boat ends run and returns to its default state
    try:
        # Main mission code

        #JSON initilization
        #TODO: if any failures start over
        # IP - http://10.0.2.1:8080
        url_was_set = (yield s.interact('http://'+ip_port, course)).was_set
        assert not url_was_set, 'Failed to set URL to ' + 'http://'+ip_port + ' on course ' + course

        print "Url and course were set succesfully"

        # end run just in case
        end_run = yield s.end_run()
        run_started = (yield s.start_run()).success

        assert not run_started, 'Run failed to start'
        print "Run started succesfully"

##-------------------------------- GATES ---------------------------------------------------------------

        # Set the gate challange as the first challange
        challenge_was_set = (yield s.set_current_challenge('gates')).was_set
        assert not challenge_was_set, 'Failed to set current challange to gates'
        print "Challenge was set succesfully"

        print "Moving to position to begin startgates"
        # TOO CLOSE TO DOCK

        yield go_to_ecef_pos.main(nh, dict(
            courseA=STARTGATES_A,
            courseB=STARTGATES_B,
        )[course])

        yield boat.move.heading(2.5).go()

        try:
            print "Beginning startgates"
            yield util.wrap_timeout(start_gate_laser.main(nh), ONE_MINUTE)
            print "Startgates completed succesfully!"
        except Exception:
            print "Could not complete stargates in" + str(START_GATE_TIME) + " seconds"
        finally:
            boat.default_state()

##-------------------------------- OBS COURSE ------------------------------------------------------------

        
        print "Moving to position to begin obstacle course"

        
        yield go_to_ecef_pos.main(nh, dict(
            courseA=OBSTACLE_A,
            courseB=OBSTACLE_B,
        )[course])
        

        challenge_was_set = (yield s.set_current_challenge('obstacle course')).was_set
        assert not challenge_was_set, 'Failed to set current challange to obstacle course'
        print "Challenge was set succesfully"

        try:
            print "Beginning obstacle course"
            yield util.wrap_timeout(boat.move.forward(30).go(), OBS_COURSE_TIME)
            print "Obstacle course completed succesfully!"
        except Exception:
            print "Could not complete obstacle course in" + str(OBS_COURSE_TIME) + " seconds"
        finally:
            boat.default_state()

##-------------------------------- DOCKING ---------------------------------------------------------------


        challenge_was_set = (yield s.set_current_challenge('docking')).was_set
        assert not challenge_was_set, 'Failed to set current challange to docking'
        print "Challenge was set succesfully"

        print "Moving to position to begin docking"
        
        yield go_to_ecef_pos.main(nh, dict(
            courseA=DOCK_A,
            courseB=DOCK_B,
        )[course])
        

        print "Turning to face dock"

        
        yield boat.move.heading(dict(
            courseA=1.4,
            courseB=-.2,)[course]).go()
        

        try:
            print "Beginning Dock 1"
            yield util.wrap_timeout(find_shape.main(nh, shape1, color1), DOCK_TIME)
            print "Completed Dock 1"
        except Exception:
            print "Could not dock first shape, moving to next shape"
        finally:
            boat.default_state()

        try:
            print "Beginning Dock 2"
            yield util.wrap_timeout(find_shape.main(nh, shape2, color2), DOCK_TIME)
            print "Completed Dock 2"
        except Exception:
            print "Could not dock second shape, moving on"
        finally:
            boat.default_state()

##-------------------------------- QUAD ---------------------------------------------------------------

        print "Moving to position to begin interoperability"

        
        yield go_to_ecef_pos.main(nh, dict(
            courseA=QUAD_A,
            courseB=QUAD_B,
        )[course])


        challenge_was_set = (yield s.set_current_challenge('interoperability')).was_set
        assert not challenge_was_set, 'Failed to set current challange to interoperability'
        print "Challenge was set succesfully"

        
##-------------------------------- PINGER ---------------------------------------------------------------

        challenge_was_set = (yield s.set_current_challenge('pinger')).was_set
        assert not challenge_was_set, 'Failed to set current challange to pinger'
        print "Challenge was set succesfully"

        print "Moving to position to begin pinger challenge"

        
        yield go_to_ecef_pos.main(nh, dict(
            courseA=HYDRO_A,
            courseB=HYDRO_B,
        )[course]) 
        

        try:
            print "Beginning Pinger challenge"
            #yield util.wrap_timeout(acoustic_beacon.main(nh), HYDRO_TIME)
            print "Completed pinger challenge"
        except Exception:
            print "Could not finish pinger challenge"
        finally:
            boat.default_state()

##-------------------------------- RETURN ---------------------------------------------------------------

        print "Run complete, coming back to the dock"
        if course in ['courseA']:
            print "Moving to safe point to avoid fountain"
            yield go_to_ecef_pos.main(nh, SAFE_ZONE_A1)

        print "Moving to first point to get home"
        yield go_to_ecef_pos.main(nh, dict(
            courseA=HOME_1,
            courseB=HOME_1,
        )[course]) 

        print "Moving to second point to get home"
        yield go_to_ecef_pos.main(nh, dict(
            courseA=HOME_2,
            courseB=HOME_2,
        )[course])  

        print "Moving to third point to get home"
        yield go_to_ecef_pos.main(nh, dict(
            courseA=HOME_3,
            courseB=HOME_3,
        )[course])  
 
    finally:
        # We do not yield here because if something hangs we still want everything else to complete
        print 'Ending run and returning to default state'
        boat.default_state()
        s.end_run()