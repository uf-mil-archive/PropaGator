from txros import util
import boat_scripting

@util.cancellableInlineCallbacks
def main(nh):
	boat = yield boat_scripting.get_boat(nh)

	#boat.pan_lidar(freq = 2, min_angle = 2.9, max_angle=3.14)

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
