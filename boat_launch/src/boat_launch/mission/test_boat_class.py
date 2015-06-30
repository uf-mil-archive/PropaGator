from txros import util
import boat_scripting
from boat_launch.mission import go_to_ecef_pos
from rawgps_common import gps
import math

# TODO: Move this to some common place to be used across files
def ll(lat, lon):
    return gps.ecef_from_latlongheight(math.radians(lat), math.radians(lon), 0)

@util.cancellableInlineCallbacks
def main(nh, location='lake_alice'):
    boat = yield boat_scripting.get_boat(nh)

    boat.set_current_challenge('test')
    try:
        while True:
            print 'Float 1 sec'
            boat.float_on()
            yield util.sleep(1)

            print 'Spin'
            boat.float_off()
            print 'O pose: ', boat.odom.position
            print 'O orien: ', boat.odom.orientation
            print 'P pose: ', boat.pose.position
            print 'P orien: ', boat.pose.orientation
            #yield boat.move.set_position(boat.odom.position).set_orientation(boat.odom.orientation).go()
            #yield boat.move.turn_left_deg(90).go()
    finally:
        boat.default_state()

"""
    boat.set_current_challenge('Go to place')

    print 'pan_lidar'
    boat.pan_lidar(freq = 2, min_angle = 2.9, max_angle=3.14)

    print 'Switch pp to point_shoot_pp'
    boat.switch_path_planner('point_shoot_pp')

    print 'Go to 36.80190, -76.19136'
    go_place = go_to_ecef_pos.main(nh, ll(36.80190, -76.19136))



    print 'sleep 3 sec'
    yield util.sleep(3)
    print 'float'
    boat.float_on()
    print 'sleep 3'
    yield util.sleep(3)
    print 'float off'
    boat.float_off()

    print 'Wait to get to 36.80190, -76.19136'
    yield go_place

    boat.set_current_challenge('Pace')

    while True:
        for i in xrange(0,3):
            print "yield boat.move.forward(2).go()"
            yield boat.move.forward(2).go()

        print "Rotate 180"
        yield boat.move.yaw_right_deg(180).go()

        for i in xrange(0,3):
            print "yield boat.move.forward(2).go()"
            yield boat.move.forward(2).go()

    print 'still lidar'
    boat.still_lidar(nominal_angle=2.8)
"""