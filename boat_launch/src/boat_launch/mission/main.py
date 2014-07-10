from __future__ import division

import traceback
import time

from txros import util
from twisted.internet import threads, stdio, protocol, defer
from twisted.protocols import basic

import boat_scripting

from boat_launch.missions import dock2


@util.cancellableInlineCallbacks
def main_list(boat, nh):
    try:
        print 'main start'
	print "starting dock2"
        yield dock2.main()
        print 'main end'
    finally:
        print 'main finally start'
        yield util.sleep(3)
        print 'main finally end'

@util.cancellableInlineCallbacks
def fail_list(boat):
    try:
        print 'fail start'
        yield utils.sleep(1)
        print 'fail end'
    finally:
        print 'fail finally'

@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)
    
    while True:
        time_left_str = yield util.nonblocking_raw_input('Enter time left: (e.g. 5:40) ')
        try:
	    if not time_left_str:
		time_left = 60 * 10
		break
            m, s = time_left_str.split(':')
            time_left = 60 * int(m) + int(s)
        except Exception:
            traceback.print_exc()
        else:
            end_time = time.time() + time_left
            del time_left
            break
    
    while True:
        course = yield util.nonblocking_raw_input('Course: (A or B) ')
        if course in ['A', 'B']:
            break
    
    try:
        yield util.wrap_timeout(main_list(boat), max(0, end_time - time.time()))
    except Exception:
        import traceback
        traceback.print_exc()
    
    yield fail_list(boat)
