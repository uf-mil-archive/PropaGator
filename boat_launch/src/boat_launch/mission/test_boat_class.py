from txros import util
import boat_scripting

@util.cancellableInlineCallbacks
def main(nh):
	boat = yield boat_scripting.get_boat(nh)

	boat.pan_lidar()

	print "yield boat.move.forward(2).go()"
	yield boat.move.forward(2).go()

	print "yield boat.move.turn_left_deg(90).go()"
	yield boat.move.turn_left_deg(90).go()

	print "yield boat.move.forward(1).go()"
	yield boat.move.forward(1).go()

	print "yield boat.move.turn_left_deg(150).go()"
	yield boat.move.turn_left_deg(150).go()

	print "yield boat.move.forward(0).go()"
	yield boat.move.forward(0).go()

	print "yield boat.move.turn_left_deg(0).go()"
	yield boat.move.turn_left_deg(0).go()

	print "yield boat.move.forward(0.001).go()"
	yield boat.move.forward(.001).go()

	print "yield boat.move.turn_left_deg(-100).go()"
	yield boat.move.turn_left_deg(-100).go()

	boat.still_lidar()
