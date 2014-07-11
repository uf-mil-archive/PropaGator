from __future__ import division

import traceback
import time
import json
from twisted.web import client
from twisted.internet import defer

from txros import util

import boat_scripting

from boat_launch.mission import dock2, acoustic_beacon

def ll(lat, lon):
    return gps.ecef_from_latlongheight([math.radians(lat), math.radians(lon), 0])

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
    def activate_light_sequence(self, course):
        x = yield self._get('lightSequence/activate', course)
        defer.returnValue(x['success'])

ci = CourseInterface('192.168.1.40', 9000, 'UF') # 9443 for HTTPS

@util.cancellableInlineCallbacks
def main_list(nh, boat, course):
    try:
        print 'main start'
        if course in ['A', 'B']:
            yield boat.move.heading_deg(90-298).go()
            yield boat.move.forward(30).go()
        
        print 'going to obstacle course'
        yield boat.go_to_ecef_pos(dict(
            A=ll(36.80245, -76.19130),
            B=ll(36.80174, -76.19138),
        )[course])
        
        yield boat.go_to_ecef_pos(dict(
            pool=[1220416.51743, -4965356.4575, 3799838.03177],
            A=ll(36.802101, -76.191527), # from google earth
            B=ll(36.802101, -76.191527), # from google earth
        )[course])
        
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
        
        print 'acoustic_beacon'
        yield acoustic_beacon.main(nh)
        
        print 'going to pinger'
        yield boat.go_to_ecef_pos(dict(
            A=ll(36.80270, -76.19140), # 28 kHz
            B=ll(36.90190, -76.19228), # 33 kHz
        )[course])
        
        print 'main end'
    finally:
        print 'main finally start'
        yield util.sleep(3)
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

@util.cancellableInlineCallbacks
def main(nh):
    print 'a'
    dock_item = yield ci.start_automated_docking('A')
    print 'dock_item:', dock_item