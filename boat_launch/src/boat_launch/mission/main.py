from __future__ import division

import traceback
import time

from txros import util

import boat_scripting

from boat_launch.mission import dock2


@util.cancellableInlineCallbacks
def main_list(nh, boat, course):
    try:
        print 'main start'
        print "starting dock2"
        assert course == 'A'; yield boat.go_to_ecef_pos([1220411.29049, -4965401.85057, 3799840.98773])
        yield boat.move.heading(45).go()
        yield dock2.main(nh)
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
        if course in ['A', 'B']:
            break
    
    try:
        yield util.wrap_timeout(main_list(nh, boat, course), max(0, end_time - time.time()))
    except Exception:
        import traceback
        traceback.print_exc()
    
    yield fail_list(nh, boat)
