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
import shutil


# Sub mission imports
from boat_launch.mission import start_gate_laser, find_shape, go_to_ecef_pos, acoustic_beacon, ssocr_simple, circle_buoy

# Timeout limits 
ONE_MINUTE = 60

TOTAL_TIME = 20 * ONE_MINUTE
DOCK_TIME = 5 * ONE_MINUTE
NTROP_TIME = 2 * ONE_MINUTE
OBS_COURSE_TIME = 2 * ONE_MINUTE
START_GATE_TIME = 3 * ONE_MINUTE
HYDRO_TIME = ONE_MINUTE * 3
ECEF_TIME = ONE_MINUTE * 1.5

# TODO: Move this to some common place to be used across files
def ll(lat, lon):
    return gps.ecef_from_latlongheight(math.radians(lat), math.radians(lon), 0)

# WAYPOINT DEFINES
STARTGATE = {
    'courseA'   :   ll(36.801987,-76.191173),
    'courseB'   :   ll(36.801786, -76.190831)
}

DOCK = {
    'courseA'   :   ll(36.801818, -76.191383),
    #'courseB'   :   ll(36.801710, -76.191940)
    'courseB'   :   ll(36.801726, -76.191935)
}

DOCK_HEADING = {
    'courseA'   :   -1.91,
    #'courseB'   :   -0.2
    'courseB'   :   -0.78539
}

OBSTACLE_ENTRANCE = {
    'courseA'   :   ll(36.801889, -76.191560),
    'courseB'   :   ll(36.801805, -76.191234)
 }

OBSTACLE_ENTRANCE_HEADING = {
    'courseA'   :   1.48
}

OBSTACLE_EXIT = {
    'courseA'   :   ll(36.802276, -76.191532),
    #'courseB'   :   ll(36.80189, -76.19139)
    'courseB'   :   ll(36.801987, -76.191651)
 }


HYDRO = {
    'courseA'   :   ll(36.802737, -76.191463),
    #'courseB'   :   ll(36.801983, -76.192111)
    'courseB'   :   ll(36.801925, -76.192289)
 }

HYDRO_HEADING = {
    'courseA'   :   2.6179
 }

INTROP_HEADING = {
    'courseA'   :   -0.52
 }

QUAD = {
    'courseA'   :   ll(36.802686, -76.191256),
    #'courseB'   :   ll(36.801925, -76.192314)
    'courseB'   :   ll(36.801801, -76.191922)
 }

# COVE ENTRANCE
SAFE_POINT_1 = {
    'courseA'   :   ll(36.802483, -76.191549),
    'courseB'   :   ll(36.801977, -76.191909)
 }

# COVE INSIDE
SAFE_POINT_2 = {
    'courseA'   :   ll(36.802729, -76.191375),
    'courseB'   :   ll(36.801977, -76.191909)
 }



HOME_1 = {
    'courseA'   :   ll(36.802103, -76.191502),
    #'courseB'   :   ll(36.802105, -76.191676)
    'courseB'   :   ll(36.801869, -76.191538)
}

HOME_2 = {
    'courseA'   :   ll(36.801880, -76.190897),
    #'courseB'   :   ll(36.801947, -76.191334)
    'courseB'   :   ll(36.801698, -76.191054)
}


HOME_3_HEADING = {
    'courseA'   :   -0.5235
}

# OTHER DEFINES


@util.cancellableInlineCallbacks
def start_gates(nh, boat, s):
    print "Moving to position to begin startgates"

    # Set the gate challange as the first challange
    s.set_current_challenge('gates')


    #yield boat.move.heading(-2.5).go()

    print "Beginning startgates"
    #yield util.wrap_timeout(start_gate_laser.main(nh), ONE_MINUTE)  if uncommented add a try except block
    yield boat.move.forward(30).go()


@util.cancellableInlineCallbacks
def obstical_course(nh, boat, s):
    global obstical_info
    print "Moving to position to begin obstacle course"

    s.set_current_challenge('obstacles')
    
    try:
        yield util.wrap_timeout(go_to_ecef_pos.main(nh, OBSTACLE[course]), ECEF_TIME)
    except:
        print 'ECEF timeout'
    
    # ADD HEADING!!!!!!!!!!!!!!!!!!!!!!

    yield boat.move.heading(OBSTACLE_ENTRANCE_HEADING[course]).go()

    # ADD HEADING!!!!!!!!!!!!!!!!!!!!!!!!!


    # Get start gate info
    obstical_info = yield obstical_info
    print 'JSON says: \n\n' + str(obstical_info) + '\n\n'

    entrance = obstical_info.entrance
    exit = obstical_info.exit

    print "Beginning obstacle course"
    yield boat.move.forward(30).go()
    print "Obstacle course completed succesfully!"

    try:
        yield util.wrap_timeout(go_to_ecef_pos.main(nh, OBSTACLE_EXIT[course]), ECEF_TIME)
    except:
        print 'ECEF timeout'




@util.cancellableInlineCallbacks
def docking(nh, boat, s):
    global docking_info
    global course

    print "Moving to position to begin docking"
    s.set_current_challenge('docking')        
    
    try:
        yield util.wrap_timeout(go_to_ecef_pos.main(nh, DOCK[course]), ECEF_TIME)
    except:
        print 'ECEF timeout'
    print "Turning to face dock"

    
    yield boat.move.heading(DOCK_HEADING[course]).go()
    
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
    finally: pass
        #boat.default_state()

    shape2 = docking_info.second_dock_symbol
    color2 = docking_info.second_dock_color
    try:
        print "Beginning Dock 2"
        yield util.wrap_timeout(find_shape.main(nh, shape2, color2), DOCK_TIME / 2.0)
        print "Completed Dock 2"
    except Exception:
        print "Could not dock second shape, moving on"
    finally: pass
        #boat.default_state()

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

    
    try:
        yield util.wrap_timeout(go_to_ecef_pos.main(nh, SAFE_POINT_2[course]), ECEF_TIME)
    except:
        print 'ECEF timeout'

    to_get_send = ['a','a']
    to_get_send = ssocr_simple.main(images_path)

    # Send the image but don't yield that way we can move while it sends
    print "Sending image", to_get_send
    sent_image = yield s.send_image_info(to_get_send[1], to_get_send[0])   # FOUR the sake of testing
    print "Removing temporary work path"
    shutil.rmtree(images_path)

    # Wait here for show
    print 'Chilling at interoperability challange'
    yield util.sleep(5)

    
@util.cancellableInlineCallbacks
def pinger(nh, boat, s):
    print "Moving to position to begin pinger challenge"
    s.set_current_challenge('pinger')

    yield boat.move.heading(HYDRO_HEADING[course]).go()

    print "Beginning Pinger challenge"

    try:
        print "Beginning Pinger"
        yield util.wrap_timeout(circle_buoy.main(nh), HYDRO_TIME)
        print "Completed Pinger"
    except Exception:
        print "Could not dock second shape, moving on"
    finally: pass
        #boat.default_state()

    '''


    color = 'blue'
    for c in ['blue', 'green', 'red', 'yellow', 'black']:
        check = yield s.send_buoy_info(c)
        if check.is_right_buoy:
            print 'Got the right buoy: ' + c
            color = c
            break
        else:
            print 'Got the wrong buoy: ' + c

    '''

sent_image = None
course = None
obstical_info = None
docking_info = None
images_info = None
sent_image = None

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
            print 'succesfully'
        except Exception as e:
            print 'Could not complete start gates: ' + str(e)
        finally:
            print 'Finally start gate'
            #boat.default_state()

##-------------------------------- DOCKING ---------------------------------------------------------------

        try:
            yield util.wrap_timeout(docking(nh, boat, s), DOCK_TIME)
        except:
            print 'Could not complete docking'
        finally: pass
            #boat.default_state()


##-------------------------------- OBS COURSE ------------------------------------------------------------
        
        print 'obstical'
        try:
            yield util.wrap_timeout(obstical_course(nh, boat, s), OBS_COURSE_TIME)
            print 'util'
        except:
            print 'Could not complete obstacle course'
        finally: pass
            #boat.default_state()
        

##-------------------------------- SAFE ZONE 1 ---------------------------------------------------------


        try:
            yield util.wrap_timeout(go_to_ecef_pos.main(nh, SAFE_POINT_1[course]), ECEF_TIME)
        except:
            print 'ECEF timeout'

        try:
            yield util.wrap_timeout(go_to_ecef_pos.main(nh, SAFE_POINT_2[course]), ECEF_TIME)
        except:
            print 'ECEF timeout'


##-------------------------------- QUAD ---------------------------------------------------------------
    
        try:
            yield util.wrap_timeout(interoperability(nh, boat, s), NTROP_TIME)
        except:
            print 'Could not complete interoperability'
        finally: pass
            #boat.default_state()


## --------------- BACK TO SAFE POINT -------------------------------------------------------------

        try:
            yield util.wrap_timeout(go_to_ecef_pos.main(nh, SAFE_POINT_2[course]), ECEF_TIME)
        except:
            print 'ECEF timeout'

    ## NEED HEADING!!!!!!!!!!!!!



    ## NEED HEADING!!!!!!!!!!!!!!!

##-------------------------------- PINGER ---------------------------------------------------------------

        try:
            yield util.wrap_timeout(pinger(nh, boat, s), HYDRO_TIME)
        except:
            print 'Could not complete pinger'
        finally: pass
            #boat.default_state()

        try:
            yield util.wrap_timeout(go_to_ecef_pos.main(nh, SAFE_POINT_2[course]), ECEF_TIME)
        except:
            print 'ECEF timeout'

        try:
            yield util.wrap_timeout(go_to_ecef_pos.main(nh, SAFE_POINT_1[course]), ECEF_TIME)
        except:
            print 'ECEF timeout'

##-------------------------------- RETURN ---------------------------------------------------------------

        print "Run complete, coming back to the dock"
        s.set_current_challenge('return')

        print "Moving to first point to get home"
        try:
            yield util.wrap_timeout(go_to_ecef_pos.main(nh, HOME_1[course]), ECEF_TIME)
        except:
            print 'ECEF timeout'        
        print "Moving to second point to get home"
        try:
            yield util.wrap_timeout(go_to_ecef_pos.main(nh, HOME_2[course]), ECEF_TIME)
        except:
            print 'ECEF timeout'


        print "Adjusting heading"
        yield boat.move.heading(HOME_3_HEADING[course]).go()

        dist = 100
        distances = yield boat.get_distance_from_object(.1)
        while dist > 2:
            yield boat.move.forward(1).go()
            distances = yield boat.get_distance_from_object(.1)
            dist = distances[0] - 1

##------------------------------ CLEAN UP -----------------------------------------------------
        
        print 'Returned to dock'
        # Make sure quad copter image sent before we end the mission
        yield sent_image

        print 'Eneded run succesfully! Go Gators!'
        s.end_run()

 
    finally:
        # We do not yield here because if something hangs we still want everything else to complete
        print 'Finally: Ending run and returning to default state'
        s.end_run()
        boat.default_state()
        
