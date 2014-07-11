from __future__ import division

import traceback
import time

from txros import util

import boat_scripting

from boat_launch.mission import dock2, acoustic_beacon


@util.cancellableInlineCallbacks
def main_list(nh, boat, course):
    try:
        print 'main start'
        yield boat.go_to_ecef_pos(dict(
            pool=[1220416.51743, -4965356.4575, 3799838.03177],
        )[course])
        print "starting dock2"
        yield boat.go_to_ecef_pos(dict(
            pool=[1220427.17101, -4965353.32773, 3799835.96511],
            A=[1220411.29049, -4965401.85057, 3799840.98773],
        )[course])
        yield boat.move.heading_deg(dict(
            pool=60,
            A=45,
        )[course]).go()
        yield dock2.main(nh)
        yield acoustic_beacon.main(nh)
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
