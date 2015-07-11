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
from os import listdir


# Sub mission imports
from boat_launch.mission import start_gate_laser, find_shape, go_to_ecef_pos, acoustic_beacon

# Timeout limits 
ONE_MINUTE = 1

TOTAL_TIME = 20 * ONE_MINUTE
DOCK_TIME = 5 * ONE_MINUTE
NTROP_TIME = ONE_MINUTE
OBS_COURSE_TIME = 2 * ONE_MINUTE
START_GATE_TIME = ONE_MINUTE
HYDRO_TIME = ONE_MINUTE * 3

# TODO: Move this to some common place to be used across files
def ll(lat, lon):
    return gps.ecef_from_latlongheight(math.radians(lat), math.radians(lon), 0)

# WAYPOINT DEFINES
STARTGATE = {
    'courseA'   :   ll(36.801987,-76.191173),
    'courseB'   :   ll(36.801786, -76.190831)
}

DOCK = {
    'courseA'   :   ll(36.802211, -76.191353),
    'courseB'   :   ll(36.801710, -76.191940)
}

OBSTACLE = {
    'courseA'   :   ll(36.802202, -76.191630),
    'courseB'   :   ll(36.80189, -76.19139)
 }

HYDRO = {
    'courseA'   :   ll(36.802737, -76.191463),
    'courseB'   :   ll(36.801983, -76.192111)
 }

QUAD = {
    'courseA'   :   ll(36.802686, -76.191256),
    'courseB'   :   ll(36.801925, -76.192314)
 }

SAFE_POINT_1 = {
    'courseA'   :   ll(36.802409, -76.191586),
    'courseB'   :   ll(36.801977, -76.191909)
 }

SAFE_POINT_2 = {
    'courseA'   :   ll(36.802409, -76.191586),
    'courseB'   :   ll(36.801977, -76.191909)
 }

HOME_1 = {
    'courseA'   :   ll(36.802105, -76.191676),
    'courseB'   :   ll(36.802105, -76.191676)
}

HOME_2 = {
    'courseA'   :   ll(36.801947, -76.191334),
    'courseB'   :   ll(36.801947, -76.191334)
}

HOME_3 = {
    'courseA'   :   ll(36.801805, -76.190871),
    'courseB'   :   ll(36.801805, -76.190871)
}

# OTHER DEFINES
DOCK_HEADING = {
    'courseA'   :   1.4,
    'courseB'   :   -0.2
}

@util.cancellableInlineCallbacks
def start_gates(nh, boat, s):
    print "Moving to position to begin startgates"

    # Set the gate challange as the first challange
    s.set_current_challenge('gates')

    
    # TOO CLOSE TO DOCK
    #yield go_to_ecef_pos.main(nh, STARTGATE[course])
    yield boat.hold_at_current_pos()

    #yield boat.move.heading(-2.5).go()

    print "Beginning startgates"
    #yield util.wrap_timeout(start_gate_laser.main(nh), ONE_MINUTE)  if uncommented add a try except block
    yield boat.move.forward(2).go()

    for i in xrange(6):
        print 'Crawl:', i
        yield boat.move.forward(5).go()
    print "Startgates completed succesfully!"


@util.cancellableInlineCallbacks
def obstical_course(nh, boat, s):
    global obstical_info
    print "Moving to position to begin obstacle course"

    s.set_current_challenge('obstacles')
    #yield go_to_ecef_pos.main(nh, OBSTACLE[course])

    # Get start gate info
    obstical_info = yield obstical_info
    print 'JSON says: \n\n' + str(obstical_info) + '\n\n'

    entrance = obstical_info.entrance
    exit = obstical_info.exit

    print "Beginning obstacle course"
    yield boat.move.forward(30).go()
    print "Obstacle course completed succesfully!"

@util.cancellableInlineCallbacks
def docking(nh, boat, s):
    global docking_info

    print "Moving to position to begin docking"
    s.set_current_challenge('docking')        
    
    yield go_to_ecef_pos.main(nh, DOCK[course])
    
    print "Turning to face dock"

    
    yield boat.move.heading(DOCK_HEADING[math.pi/4]).go()
    
    # Get dock info
    docking_info = yield docking_info
    print 'JSON says: \n\n' + str(docking_info) + '\n\n'

    shape1 = docking_info.first_dock_symbol
    color1 = docking_info.first_dock_color
    try:
        print "Beginning Dock 1"
        yield util.wrap_timeout(find_shape.main(nh, shape1, color1), DOCK_TIME / 2.0)
        print "Completed Dock 1"
    except Exception:
        print "Could not dock first shape, moving to next shape"
    finally:
        boat.default_state()

    shape2 = docking_info.second_dock_symbol
    color2 = docking_info.second_dock_color
    try:
        print "Beginning Dock 2"
        yield util.wrap_timeout(find_shape.main(nh, shape2, color2), DOCK_TIME / 2.0)
        print "Completed Dock 2"
    except Exception:
        print "Could not dock second shape, moving on"
    finally:
        boat.default_state()

@util.cancellableInlineCallbacks
def interoperability(nh, boat, s):
    global docking_info
    global images_info
    global sent_image
    print "Moving to position to begin interoperability"
    s.set_current_challenge('interop')

    # Get the images
    images_info = yield images_info
    print 'JSON says: \n\n' + str(images_info) + '\n\n'
    images_path = images_info.file_path
    images_count = images_info.image_count


    # GOES TO POINT 1
    yield go_to_ecef_pos.main(nh, SAFE_POINT_1[course])
    yield go_to_ecef_pos.main(nh, QUAD[course])

    # Send the image but don't yield that way we can move while it sends
    #sent_image = s.send_image_info('TODO.jpg', 'ALL_CAPS_NUMBER')
    sent_image = yield s.send_image_info('0.png', 'ZERO')   # FOUR the sake of testing

    # Wait here for show
    print 'Chilling ad interop challange'
    yield util.sleep(5)

@util.cancellableInlineCallbacks
def pinger(nh, boat, s):
    print "Moving to position to begin pinger challenge"
    s.set_current_challenge('pinger')

    yield go_to_ecef_pos.main(nh, HYDRO[course])
    print "Beginning Pinger challenge"

    color = 'blue'
    for c in ['blue', 'green', 'red', 'yellow', 'black']:
        check = yield s.send_buoy_info(c)
        if check.is_right_buoy:
            print 'Got the right buoy: ' + c
            color = c
            break
        else:
            print 'Got the wrong buoy: ' + c

    
    #yield acoustic_beacon.main(nh)
    print "Completed pinger challenge"


@util.cancellableInlineCallbacks
def main(nh):
    global course
    global obstical_info
    global docking_info
    global images_info
    global sent_image

    # Grab interfaces for boat and JSON server
    boat = yield boat_scripting.get_boat(nh)
    s =  yield json_server_proxy.get_server(nh)

    # Grab mission input such as JSON server ip, and course
    ip_port = raw_input('Enter ip:port (ex. 10.0.2.1:8080): ')
    course = raw_input('Enter course with corect spelling! (courseA, courseB, ...): ')

    # Check that the course is in the dictionaries
    assert course in DOCK.keys(), '%s is not in %s' % (course, DOCK.keys())


    shape1 = None
    shape2 = None
    color1 = None
    color2 = None

    boat.default_state()

    # Giant try finally to make sure boat ends run and returns to its default state
    try:
        # Main mission code

        # JSON initilization
        # TODO: if any failures start over
        # IP - http://10.0.2.1:8080

        url_was_set = (yield s.interact('http://'+ip_port, course)).was_set
        assert url_was_set, 'Failed to set URL to ' + 'http://'+ip_port + ' on course ' + course

        # Set the current challange
        set_challenge = (yield s.set_current_challenge('gates')).was_set
        print "Url and course were set succesfully"

        # end run before start just in case
        end_run = yield s.end_run()
        run_started = (yield s.start_run()).success
        assert run_started, 'Run failed to start'
        print "Run started succesfully"


##------------------------------- Grab all JSON data --------------------------------------------------

        print 'Starting mass JSON download'
        obstical_info = s.get_gate_info()
        docking_info = s.get_dock_info()
        images_info = s.get_server_images()
        

##-------------------------------- GATES ---------------------------------------------------------------

        try:
            yield util.wrap_timeout(start_gates(nh, boat, s), START_GATE_TIME)
        except:
            'Could not complete start gates'
        finally:
            boat.default_state()


##-------------------------------- OBS COURSE ------------------------------------------------------------

        try:
            yield util.wrap_timeout(obstical_course(nh, boat, s), OBS_COURSE_TIME)
        except:
            'Could not complete obstacle course'
        finally:
            boat.default_state()
        

##-------------------------------- DOCKING ---------------------------------------------------------------

        try:
            yield util.wrap_timeout(docking(nh, boat, s), DOCK_TIME)
        except:
            'Could not complete docking'
        finally:
            boat.default_state()



##-------------------------------- QUAD ---------------------------------------------------------------
        
        try:
            yield util.wrap_timeout(interoperability(nh, boat, s), NTROP_TIME)
        except:
            'Could not complete interoperability'
        finally:
            boat.default_state()

        
##-------------------------------- PINGER ---------------------------------------------------------------

        try:
            yield util.wrap_timeout(pinger(nh, boat, s), HYDRO_TIME)
        except:
            'Could not complete pinger'
        finally:
            boat.default_state()

##-------------------------------- RETURN ---------------------------------------------------------------

        print "Run complete, coming back to the dock"
        s.set_current_challenge('return')

        if course is 'courseA':
            print "Moving to safe point to avoid fountain"
            yield go_to_ecef_pos.main(nh, SAFE_POINT_1[course])

        print "Moving to first point to get home"
        yield go_to_ecef_pos.main(nh, HOME_1[course]) 

        print "Moving to second point to get home"
        yield go_to_ecef_pos.main(nh, HOME_2[course])  

        print "Moving to third point to get home"
        yield go_to_ecef_pos.main(nh, HOME_3[course])  

##------------------------------ CLEAN UP -----------------------------------------------------
        
        print 'Returned to dock'
        # Make sure quad copter image sent before we end the mission
        yield sent_image

        print 'Eneded run succesfully! Go Gators!'
        s.end_run()
 
    finally:
        # We do not yield here because if something hangs we still want everything else to complete
        print 'Finally: Ending run and returning to default state'
        boat.default_state()
        s.end_run()
