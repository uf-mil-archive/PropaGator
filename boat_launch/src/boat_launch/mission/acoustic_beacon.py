import math 
import traceback 
from txros import util 
import random 

from hydrophones.msg import ProcessedPing

import boat_scripting
from rawgps_common import gps
from boat_launch.mission import hydrophone_align

course_alice = [29.641341, -82.361133]

@util.cancellableInlineCallbacks
def _send_result(course, lat, lon):
    # Send over the results and the color of the buoy through the JSON server
    # I will populate this field come lakeday 
    pass

@util.cancellableInlineCallbacks 
def main(nh, course=course_alice, min_freq=27E3, max_freq=40E3):
    # nh = Node Handler from txros
    # course = Set of GPS points 
    # freq = 27440 Remnant from previous mission file. I don't really know why this is here.
    boat = yield boat_scripting.get_boat(nh)

    try:
        print "Beginning ping mission" 
        print "Deploying hydrophone..."
        yield boat.deploy_hydrophone()
        print "Hydrophone has been deployed"
        
        try:
            yield util.wrap_timeout(hydrophone_align.main(nh, min_freq, max_freq), 60*2)
        except Exception:
            traceback.print_exc()
        print "Ping mission has finished"

        #temp = gps.latlongheight_from_ecef(boat.odom.position)
        #print "latitude: ", temp[0], " longitude: ", temp[1]

        # In the main mission state machine remember to rerun the retract hydrophone mission 
        # in case this mission timesout.
        print "Retracting hydrophone..."
        yield boat.retract_hydrophone()
        print "Hydrophone retracted"
        print "Done"

    finally:
        boat.default_state()