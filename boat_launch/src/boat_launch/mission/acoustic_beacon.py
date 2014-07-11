from __future__ import division

from txros import util

# we will move the boat based on the info in the processed messages
from hydrophones.msg import ProcessedPing
# we will also subscribe to a simple message that tells us the freq to look fo
from std_msgs.msg import Float64
# we will publish the pingers lat long with a nav_msgs/Odometry message like the /absodom topic. the lat long is under pose.pose.position
from geometry_msgs.msg import Point

import boat_scripting

#SPP the gps methods is exported here only fur testing currently
from rawgps_common import gps

#NOTE: run missions does ros init node for us
    

@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)
    float_df = boat.float()
    print "Starting ping mission"
    yield boat.deploy_hydrophone()
    print "Deploying Hydrophone"
    #yield boat.hydrophone_align(boat.get_hydrophone_freq())
    print "Finished ping mission"
    # Note: /absodom is Earth-Centered,Earth-Fixed (ECEF), so This means that ECEF rotates with the earth and a point fixed on the surface of the earth do not change.
    # See: http://en.wikipedia.org/wiki/ECEF
    msg = yield boat.get_gps_odom()
    temp = gps.latlongheight_from_ecef([msg.pose.pose.position.x,msg.pose.pose.position.y,msg.pose.pose.position.z])
    print "latitude: ", temp[0]," longitude: ", temp[1]
    float_df.cancel()
    yield boat.retract_hydrophone()
    print "Retracting Hydrophone"
