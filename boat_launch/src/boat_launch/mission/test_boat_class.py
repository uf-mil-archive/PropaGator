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

	#boat.pan_lidar(freq = 2, min_angle = 2.9, max_angle=3.14)
	yield go_to_ecef_pos.main(nh, ll(36.80190, -76.19136))

	while True:
		for i in xrange(0,3):
			print "yield boat.move.forward(2).go()"
			yield boat.move.forward(2).go()

		print "Rotate 180"
		yield boat.move.yaw_right_deg(180).go()

		for i in xrange(0,3):
			print "yield boat.move.forward(2).go()"
			yield boat.move.forward(2).go()

	#boat.still_lidar(nominal_angle=2.8)
