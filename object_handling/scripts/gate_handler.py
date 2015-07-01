#!/usr/bin/env python
import numpy
from geometry_msgs.msg import Vector3
from std_msgs.msg import Header
from object_handling.msg import Buoys, Gate, Gates
from itertools import *
import rospy

"""
This node finds gates
    Gates are a complex object composed of two buoys
    Parameters define what two properties two buoys must have to be considered a gate
    The parameters are:
        maximum distance apart
        minimum distance apart
        the minimum perpendicular distance an outside buoy can be to the line drawn between the buoys composing the gate 

Math:
    * All combinations of buoy pairs are considered which means there are n * (n - 1) / 2 possible buoy pairs
        where n is the number of buoys in

Algorithim: 
* 

"""

def xy_array(o):
    return numpy.array([o.x, o.y])

def to_vector(o):
    return Vector3(o[0], o[1], 0) 

class gate_handler:
    def __init__(self):
        rospy.init_node('gate_handler', anonymous=False)#, log_level=rospy.DEBUG)
        # Publishers
        self.gate_pub = rospy.Publisher('gates', Gates, queue_size = 10)
        # Subscribers
        self.buoys_sub = rospy.Subscriber('buoys', Buoys, self.buoysCb)
        # Parameters
        # Maximum width
        self.max_width = rospy.get_param('~max_width', 5.0)
        # Minimum width
        self.min_width = rospy.get_param('~min_width', 1.0)
        # to be to different gates they must have this much angular tolerance
        self.same_ang_tol = rospy.get_param('~same_angular_tolerance', 15.0 * numpy.pi / 180.0)
        # Distance tolerance
        self.same_dis_tol = rospy.get_param('~same_distance_tolerance', 0.5)
        # Intersecting buoy distance
        self.intersecting_buoy_tol = rospy.get_param('intersecting_buoy_tolerance', 0.5)

        # to be to different objects they must be this far apart
        self.lifetime = rospy.get_param('~lifetime', 5.0)

        self.gates = list()

        rospy.Timer(rospy.Duration(0.1), self.updateCb)

    # make sure the object hasn't goen stale (lived longer than it should have)
    def isFreshObject(self, g):
        now = rospy.get_time()
        if now - g[0] < self.lifetime:
            return True
        else:
            return False

    # Check if two gates are the same
    def isSameGate(self, g1, g2):
        # Check angular tolerance
        if abs(g1.yaw - g2.yaw) > self.same_ang_tol:
            return False

        # Check distance tolerance
        dis = numpy.linalg.norm(xy_array(g1.position) - xy_array(g2.position))
        if dis > self.same_dis_tol:
            return False

        return True 

    # Insert a gate
    def insertGate(self, new_g):
        # Check if gate already exist
        for i, old_g in enumerate(self.gates):
            # Drop the time
            old_g = old_g[1]
            if self.isSameGate(new_g, old_g):
                # Replace old gate
                self.gates[i] = (rospy.get_time(), new_g)
                return

        # If its made it this far it is a new gate
        self.gates.append((rospy.get_time(), new_g))

    # Callback for when new buoys are recieved
    def buoysCb(self, buoys):
        rospy.logdebug('Buoys callback start')
        # Check to make sure there are at least two buoys
        # If not try again
        if len(buoys.buoys) < 2:
            rospy.logdebug('1 or less buoys, therfore no new gates')
            return

        # Make sure the buoys are at least min meters apart and
        #   no more than max meters apart
        for comb in combinations(buoys.buoys, 2):
            # Get numpy arrays of the positoins
            b0 = xy_array(comb[0].position)
            b1 = xy_array(comb[1].position)
            rospy.logdebug('Combination: ' + str(b0) + ' : ' + str(b1))

            #
            # Width check
            #
            buoy_to_buoy = b0 - b1 
            width = numpy.linalg.norm(buoy_to_buoy)
            if width < self.min_width or width > self.max_width:
                continue
            rospy.logdebug('Pass width check')

            #
            # Check to see if any other buoy intersects the line drawn between the two composing buoys
            #
            intersecting_buoy = False
            for b in buoys.buoys:
                # Convert to numpy array
                b = xy_array(b.position)

                # Check if this is one of the buoys in the gate (if the distance is less than 1cm its the same buoy)
                if numpy.linalg.norm(b - b0) < 0.01:
                    continue
                elif numpy.linalg.norm(b - b1) < 0.01:
                    continue

                rospy.logdebug('Intersecting test: Pass same buoy check')

                # Shift points such that buoy 0 is at 0,0
                b = b - b0
                b1_shift = b1 - b0
                b_proj = buoy_to_buoy * (numpy.dot(b, buoy_to_buoy) / (width**2))

                # Check if the projection is between the two composing buoys in the shifted cordinates
                if numpy.linalg.norm(b_proj) > width:
                    # Does not intersect
                    continue
                elif numpy.linalg.norm(b_proj - b1_shift) > width:
                    # Does not intersect
                    continue

                # This buoys projection is between the composing buoys, check perpendicular distance
                b_perp = b - b_proj
                if numpy.linalg.norm(b_perp) < self.intersecting_buoy_tol:
                    # There is a buoy between the composing buoys this is not a gate
                    intersecting_buoy = True
                    break

            if intersecting_buoy:
                continue
            rospy.logdebug('Pass intersecting buoy check')

            #
            # This is a gate
            #
            new_g = Gate(
                    position = to_vector((b0 + b1) / 2),
                    buoy1 = comb[0],
                    buoy2 = comb[1],
                    yaw = numpy.arctan2(buoy_to_buoy[1], buoy_to_buoy[0]) % numpy.pi,
                    width = width
                )

            # Insert the new gate
            self.insertGate(new_g)


    def updateCb(self, event):
        self.gates = filter(self.isFreshObject, self.gates)

        gates = Gates(
            header = Header(
                stamp = rospy.Time.now(),
                frame_id = '/enu'),
            gates = [g[1] for g in self.gates]
        )

        self.gate_pub.publish(gates)

# Run the node
if __name__ == '__main__':
    node = gate_handler()
    rospy.spin()