#!/usr/bin/env python
import numpy
from geometry_msgs.msg import Vector3
from std_msgs.msg import Header
from object_handling.msg import Buoys, Gate, Gates
from visualization_msgs.msg import MarkerArray, Marker
from nav_msgs.msg import Odometry
from uf_common.orientation_helpers import xyz_array
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
        self.gate_viz_pub = rospy.Publisher('gates_viz', MarkerArray, queue_size = 10)
        # Parameters
        # Maximum width
        self.max_width = rospy.get_param('~max_width', 8.0)
        # Minimum width
        self.min_width = rospy.get_param('~min_width', 1.0)
        # to be to different gates they must have this much angular tolerance
        self.same_ang_tol = rospy.get_param('~same_angular_tolerance', 30.0 * numpy.pi / 180.0)
        # Distance tolerance
        self.same_dis_tol = rospy.get_param('~same_distance_tolerance', 1)
        # Intersecting buoy distance
        self.intersecting_buoy_tol = rospy.get_param('intersecting_buoy_tolerance', 0.5)

        # to be to different objects they must be this far apart
        self.lifetime = rospy.get_param('~lifetime', 5.0)

        self.gates = list()

        # Subscribers
        self.buoys_sub = rospy.Subscriber('buoys', Buoys, self.buoysCb, queue_size=1)

    def visualize(self):
        m_array = MarkerArray()
        for i, g in enumerate(self.gates):
            m = Marker()
            m.header = Header(frame_id = '/enu', stamp = rospy.Time.now())
            m.ns = 'gates'
            m.id = i
            m.type = Marker.LINE_STRIP
            m.action = Marker.ADD
            m.scale.x = 0.1
            m.color.r = 0.5
            m.color.b = 0.75
            m.color.g = 0.1
            m.color.a = 1.0
            m.points.append(g[1].buoy1.position)
            m.points.append(g[1].buoy2.position)
            m.lifetime = rospy.Duration(self.lifetime)
            m_array.markers.append(m)

        self.gate_viz_pub.publish(m_array)

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
                #rospy.logdebug('Replace an old gate')
                return

        # If its made it this far it is a new gate
        rospy.logdebug('New gate')
        self.gates.append((rospy.get_time(), new_g))

    # Publish all gates that are fresh
    def sendGates(self):
        self.gates = filter(self.isFreshObject, self.gates)

        gates = Gates(
            header = Header(
                stamp = rospy.Time.now(),
                frame_id = '/enu'),
            gates = [g[1] for g in self.gates]
        )

        self.gate_pub.publish(gates)

        self.visualize()

    # Callback for when new buoys are recieved
    def buoysCb(self, buoys):
        rospy.logdebug('Buoys callback start with ' + str(len(buoys.buoys)) + ' buoys')
        # Check to make sure there are at least two buoys
        # If not try again
        if len(buoys.buoys) < 2:
            rospy.logdebug('1 or less buoys, therfore no new gates')
            self.sendGates()
            return

        # Make sure the buoys are at least min meters apart and
        #   no more than max meters apart
        for iii, buoy0 in enumerate(buoys.buoys[0:-1]):
            b0 = xy_array(buoy0.position)
            # Sort the list based on clossness to b0
            # Drop all buoys already checked and the same buoy
            sorted_buoys = buoys.buoys[(iii + 1):]
            sorted_buoys = sorted(sorted_buoys, 
                cmp = lambda x, y: cmp(numpy.linalg.norm(b0 - xy_array(x.position)),
                                       numpy.linalg.norm(b0 - xy_array(y.position)) ))

            for jjj, buoy1 in enumerate(sorted_buoys):
                b1 = xy_array(buoy1.position)
                rospy.logdebug('Combination: ' + str(b0) + ' : ' + str(b1))

                #
                # Width check
                #
                buoy_to_buoy = b0 - b1 
                width = numpy.linalg.norm(buoy_to_buoy)
                if width < self.min_width:
                    rospy.logdebug('Fail width check: to close')
                    continue
                elif width > self.max_width:
                    rospy.logdebug('Fail width check: to far apart')
                    # Since buoys are sorted by closeness we can break this loop
                    break
                rospy.logdebug('Pass width check')

                #
                # Check to see if any other buoy intersects the line drawn between the two composing buoys
                #
                intersecting_buoy = False
                rospy.logdebug('Checking ' + str(len(sorted_buoys[:jjj])) + ' possible intersecting buoy(s)')
                for b_sect in sorted_buoys[:jjj]:
                    # Convert to numpy array
                    b_sect = xy_array(b_sect.position)

                    # Shift points such that buoy 0 is at 0,0
                    b_sect = b_sect - b0
                    b1_shift = b1 - b0
                    b_sect_proj = buoy_to_buoy * (numpy.dot(b_sect, buoy_to_buoy) / (width**2))

                    # Check if the projection is between the two composing buoys in the shifted cordinates
                    if numpy.linalg.norm(b_sect_proj - b1_shift) > width:
                        # Does not intersect
                        continue

                    # This buoys projection is between the composing buoys, check perpendicular distance
                    b_sect_perp = b_sect - b_sect_proj
                    if numpy.linalg.norm(b_sect_perp) < self.intersecting_buoy_tol:
                        # There is a buoy between the composing buoys this is not a gate
                        intersecting_buoy = True
                        rospy.logdebug('Intersecting test: Fail buoy intersects')
                        break
                if intersecting_buoy:
                    continue
                rospy.logdebug('Pass Intersecting test')
                
                #
                # This is a gate
                #
                new_g = Gate(
                        position = to_vector((b0 + b1) / 2),
                        buoy1 = buoy0,
                        buoy2 = buoy1,
                        yaw = numpy.arctan2(buoy_to_buoy[1], buoy_to_buoy[0]) % numpy.pi,
                        width = width
                    )

                # Insert the new gate
                self.insertGate(new_g)

        self.sendGates()

# Run the node
if __name__ == '__main__':
    node = gate_handler()
    rospy.spin()