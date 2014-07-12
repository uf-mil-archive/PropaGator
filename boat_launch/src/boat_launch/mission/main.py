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
        print 'GET', 'http://%s:%i/%s/course%s/%s' % (self._host, self._port, name, course, self._team_code)
        data = yield client.getPage(
            url='http://%s:%i/%s/course%s/%s' % (self._host, self._port, name, course, self._team_code),
            timeout=10,
        )
        print data
        defer.returnValue(json.loads(data))
    @util.cancellableInlineCallbacks
    def _post(self, name, course, params):
        print 'POST', 'http://%s:%i/%s/course%s/%s' % (self._host, self._port, name, course, self._team_code), params
        data = yield client.getPage(
            method='POST',
            url='http://%s:%i/%s/course%s/%s' % (self._host, self._port, name, course, self._team_code),
            timeout=10,
            headers={'Content-Type': 'application/json'},
            postdata=json.dumps(params),
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
        x = yield self._post('lightSequence/activate', course, None)
        defer.returnValue(x['success'])
    
    @util.cancellableInlineCallbacks
    def report_light_sequence(self, course, sequence):
        x = yield self._post('lightSequence/report', course, dict(
            sequence=sequence,
        ))
        defer.returnValue(x['success'])
    
    def send_pinger_answer(self, course, color, lat, lon):
        return self._post('pinger', course, dict(
            course=course,
            team=self._team_code,
            buoyColor=color,
            buoyPosition=dict(
                datum="WGS84",
                latitude=lat,
                longitude=lon,
            ),
        ))

ci = CourseInterface('192.168.1.40', 9000, 'UF') # 9443 for HTTPS

@util.cancellableInlineCallbacks
def do_obstacle_course(nh, boat, course):
    print 'going to obstacle course'
    yield boat.go_to_ecef_pos(dict(
        A=ll( 36.80198, -76.19129),
        B=ll(36.80189, -76.19139),
    )[course])
    yield boat.move.heading_deg(dict(
        A=60+90,
        B=60+90,
    )[course]).go()
    try:
        gates = yield ci.start_obstacle_avoidance(course)
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
        A=[1220408.5926, -4965366.56212, 3799819.98198],
        B=ll(36.801754, -76.191911),
    )[course])
    yield boat.move.heading_deg(dict(
        pool=60,
        A=60,
        B=90-146,
    )[course]).go()
    yield boat.move.backward(3).go()
    try:
        dock_item = yield ci.start_automated_docking(course)
    except Exception:
        traceback.print_exc()
        dock_item = 'cruciform'
        print 'Defaulting to', dock_item
    print 'dock_item:', dock_item
    s = boat.move
    try:
        yield util.wrap_timeout(dock2.main(nh, dock_item), 40)
    except Exception:
        traceback.print_exc()
    yield s.left(2).go()
    try:
        yield util.wrap_timeout(dock2.main(nh, 'circle'), 40)
    except Exception:
        traceback.print_exc()
    yield s.right(2).go()
    try:
        yield util.wrap_timeout(dock2.main(nh, 'triangle'), 40)
    except Exception:
        traceback.print_exc()
    yield s.right(4).go()
    try:
        yield util.wrap_timeout(dock2.main(nh, 'cruciform'), 40)
    except Exception:
        traceback.print_exc()

@util.cancellableInlineCallbacks
def main_list(nh, boat, course):
    try:
        print 'main start'
        
        #yield boat.move.forward(3).go()
        
        print 'Activating light sequence'
        try:
            res = yield ci.activate_light_sequence(course)
        except Exception:
            traceback.print_exc()
            print 'LIGHT SEQUENCE FAILED! WARNING!'
            #yield util.sleep(5)
        else:
            print 'Result:', res
        yield util.sleep(2.0)
        colors = ["red", "green", "blue", "yellow"]
        yield ci.report_light_sequence(course, [random.choice(colors) for i in xrange(3)])
        
        print 'Running gate2'
        yield boat.move.forward(50).go()
        #try:
        #    yield util.wrap_timeout(gate2.main(nh, 'left' if course == 'B' else 'right'), 60*2)
        #except Exception:
        #    traceback.print_exc()
        
        
        try:
            yield util.wrap_timeout(do_obstacle_course(nh, boat, course), 2*60)
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
            A=ll(36.80270, -76.19140),
            B=ll(36.80175, -76.19230),
        )[course])
        freq = dict(
            A=27e3,
            B=32e3,
        )[course]
        
        print 'acoustic_beacon'
        yield acoustic_beacon.main(nh, ci, course, freq)
        
        print 'Going to safe point 2'
        yield boat.go_to_ecef_pos(dict(
            pool=[1220416.51743, -4965356.4575, 3799838.03177],
            A=ll( 36.802358, -76.191629), # from google earth
            B=ll(36.801972, -76.191849), # from google earth
        )[course])
        
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
