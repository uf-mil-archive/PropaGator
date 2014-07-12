from __future__ import division

import math
import traceback
import time
import json
import math
from twisted.web import client
from twisted.internet import defer
import random

from txros import util

import boat_scripting
from rawgps_common import gps

from boat_launch.mission import dock2, acoustic_beacon, gate2

def ll(lat, lon):
    return gps.ecef_from_latlongheight(math.radians(lat), math.radians(lon), 0)

class CourseInterface(object):
    def __init__(self, host, port, team_code):
        self._host = host
        self._port = port
        self._team_code = team_code
    
    @util.cancellableInlineCallbacks
    def _get(self, name, course):
        data = yield client.getPage(
            url='http://%s:%i/%s/course%s/%s' % (self._host, self._port, name, course, self._team_code),
            timeout=10,
        )
        print data
        defer.returnValue(json.loads(data))
    
    @util.cancellableInlineCallbacks
    def start_automated_docking(self, course):
        x = yield self._get('automatedDocking', course)
        defer.returnValue(x['dockingBay'])
    
    @util.cancellableInlineCallbacks
    def start_obstacle_avoidance(self, course):
        x = yield self._get('obstacleAvoidance', course)
        s = x['gateCode']
        assert len(s) == 5
        assert s[0] == '(' and s[2] == ',' and s[4] == ')'
        defer.returnValue((s[1], s[3]))
    
    @util.cancellableInlineCallbacks
    def activate_light_sequence(self, course):
        x = yield self._get('lightSequence/activate', course)
        defer.returnValue(x['success'])

ci = CourseInterface('192.168.1.40', 9000, 'UF') # 9443 for HTTPS

@util.cancellableInlineCallbacks
def do_obstacle_course(nh, boat, course):
    print 'going to obstacle course'
    # XXX
    yield boat.go_to_ecef_pos(dict(
        A=ll(36.80245, -76.19130),
        B=ll(36.80174, -76.19138),
    )[course])
    try:
        gates = yield ci.start_automated_docking(course)
    except Exception:
        traceback.print_exc()
        gates = random.choice(['1','2','3']), random.choice(['X', 'Y', 'Z'])
        print 'Defaulting to', gates
    print 'Got', gates
    if gates[0] == '1':
        yield boat.move.left(2.5).go()
    elif gates[0] == '3':
        yield boat.move.right(2.5).go()
    yield boat.move.forward(20).go()
    if gates[0] == '1':
        yield boat.move.right(2.5).go()
    elif gates[0] == '3':
        yield boat.move.left(2.5).go()
    
    if gates[1] == 'X':
        yield boat.move.left(2.5).go()
    elif gates[1] == 'Z':
        yield boat.move.right(2.5).go()
    yield boat.move.forward(20).go()


@util.cancellableInlineCallbacks
def do_dock(nh, boat, course):
    print "starting dock2"
    yield boat.go_to_ecef_pos(dict(
        pool=[1220427.17101, -4965353.32773, 3799835.96511],
        A=ll(36.80225, -76.19128),
        B=ll(36.80174, -76.19189),
    )[course])
    yield boat.move.heading_deg(dict(
        pool=60,
        A=60,
        B=90-146,
    )[course]).go()
    try:
        dock_item = yield ci.start_automated_docking(course)
    except Exception:
        traceback.print_exc()
        dock_item = 'cruciform'
        print 'Defaulting to', dock_item
    print 'dock_item:', dock_item
    yield dock2.main(nh, dock_item)

@util.cancellableInlineCallbacks
def main_list(nh, boat, course):
    try:
        print 'main start'
        
        yield boat.move.forward(3).go()
        
        print 'Activating light sequence'
        try:
            res = yield ci.activate_light_sequence(course)
        except Exception:
            print 'LIGHT SEQUENCE FAILED! WARNING!'
            yield util.sleep(5)
        else:
            print 'Result:', res
        
        print 'Running gate2'
        try:
            yield util.wrap_timeout(gate2.main(nh, 'left' if course == 'B' else 'right'), 60*2)
        except Exception:
            traceback.print_exc()
        
        
        try:
            yield util.wrap_timeout(do_obstacle_course(nh, boat, course), 60)
        except Exception:
            traceback.print_exc()
        
        # safe point
        print 'Going to safe point 1'
        yield boat.go_to_ecef_pos(dict(
            pool=[1220416.51743, -4965356.4575, 3799838.03177],
            A=ll(36.802187, -76.191501), # from google earth
            B=ll(36.801972, -76.191849), # from google earth
        )[course])
        
        try:
            yield util.wrap_timeout(do_dock(nh, boat, course), 60*4)
        except Exception:
            traceback.print_exc()
        
        
        # safe point
        print 'Going to safe point 2'
        yield boat.go_to_ecef_pos(dict(
            pool=[1220416.51743, -4965356.4575, 3799838.03177],
            A=ll( 36.802358, -76.191629), # from google earth
            B=ll(36.801972, -76.191849), # from google earth
        )[course])
        
        print 'going to pinger'
        yield boat.go_to_ecef_pos(dict(
            A=ll(36.80270, -76.19140), # 28 kHz
            B=ll(36.90190, -76.19228), # 33 kHz
        )[course])
        
        print 'acoustic_beacon'
        yield acoustic_beacon.main(nh)
        
        print 'main end'
    finally:
        print 'main finally start'
        #yield util.sleep(3)
        print 'main finally end'

@util.cancellableInlineCallbacks
def fail_list(nh, boat):
    try:
        print 'fail start'
        yield util.sleep(1)
        print 'fail end'
    finally:
        print 'fail finally'

@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)
    
    while True:
        yield util.sleep(.1)
        time_left_str = yield util.nonblocking_raw_input('Enter time left: (e.g. 5:40) ')
        try:
            m, s = time_left_str.split(':')
            time_left = 60 * int(m) + int(s)
        except Exception:
            traceback.print_exc()
        else:
            end_time = time.time() + time_left
            del time_left
            break
    
    while True:
        yield util.sleep(.1)
        course = yield util.nonblocking_raw_input('Course: (A or B) ')
        if course in ['A', 'B', 'pool']:
            break
    
    try:
        yield util.wrap_timeout(main_list(nh, boat, course), max(0, end_time - time.time()))
    except Exception:
        traceback.print_exc()
    
    yield fail_list(nh, boat)
