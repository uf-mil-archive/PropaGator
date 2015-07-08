#! /usr/bin/env python

from __future__ import division
from std_msgs.msg import Float64, Header
from txros import util
import rospy, numpy
import boat_scripting
from uf_common.orientation_helpers import xyz_array, xyzw_array, quat_to_rotvec
import sensor_msgs.point_cloud2 as pc2
import math
from visualization_msgs.msg import MarkerArray, Marker
from geometry_msgs.msg import Vector3
from uf_common.orientation_helpers import *

def to_vector(x, y):
    return Vector3(x ,y , 0) 

@util.cancellableInlineCallbacks
def main(nh):

	boat = yield boat_scripting.get_boat(nh)

	try:
	    
	    rospy.init_node("test")
	    gate_viz_pub = rospy.Publisher('gates_viz', MarkerArray, queue_size = 10)


	    while not rospy.is_shutdown():

		    hold = []

		    x_mean = 0
		    y_mean = 0
		    numerator = 0
		    denom = 0

		    
		    # loops until lidar gets a good filtered lidar reading
		    while len(hold) <= 0:
		        pointcloud = yield boat.get_pointcloud()
		        yield util.sleep(.1) # sleep to avoid tooPast errors
		        pointcloud_base = yield boat.to_baselink(pointcloud)
		        yield util.sleep(.1) # sleep to avoid tooPast errors

		        pointcloud_enu = []

		        for p in pc2.read_points(pointcloud, field_names=("x", "y", "z"), skip_nans=False, uvs=[]):
		            pointcloud_enu.append((p[0], p[1], p[2]))

		        yield util.sleep(.1) # sleep to avoid tooPast errors

		        point_in_base = []
		        hold = []

		        # Filter lidar data to only data right in front of the boat
		        print "Filtering lidar data"
		        for i in range(0, len(pointcloud_base)):
		        	temp = pointcloud_base[i]
		        	if abs(temp[1]) < .1:
		        		hold.append(pointcloud_enu[i])

		    for i in hold:
		    	x_mean += i[0]
		    	y_mean += i[1]

		    y_mean = y_mean/len(hold)
		    x_mean = x_mean/len(hold)


		    for i in hold:
		    	numerator += (i[0] - x_mean)*(i[1] - y_mean)
		    	denom += (i[0] - x_mean)*(i[0] - x_mean)

		    if denom == 0: denom = 1
		    m = numerator/denom
		    b = y_mean - (m * x_mean)

		    x1 = 1
		    x2 = 100

		    point1 = m*x1 + b
		    point2 = m*x2 + b

		    position = boat.odom.position[0:2]
		    yaw = quat_to_rotvec(boat.odom.orientation)[2]
		    heading = numpy.array([numpy.cos(yaw), numpy.sin(yaw)])

		    # Line we want to be orthagonal to
		    position1 = to_vector(x1, point1)
		    position2 = to_vector(x2, point2)
		    position3 = to_vector((heading[0]) + boat.odom.position[0], (heading[1]) + boat.odom.position[1])
		    position4 = to_vector((heading[0]+ .01) + boat.odom.position[0], (heading[1]+.1) + boat.odom.position[1])
		    
		    vec1 = numpy.array([x1, point1, 0])

		    angle = 2

		    while angle > .91 and angle < .89:

			    print angle

			    position = boat.odom.position[0:2]
			    yaw = quat_to_rotvec(boat.odom.orientation)[2]
			    heading = numpy.array([numpy.cos(yaw), numpy.sin(yaw)])


			    vec2 = numpy.array([(heading[0]), (heading[1]), 0])

			    numerator = numpy.dot(vec1, vec2)
			    one = numpy.linalg.norm(vec1)
			    two =  numpy.linalg.norm(vec2)
			    cosine = numerator / (one * two)
			    angle = math.acos(cosine)

			    #print angle
			    #print vec1
			    #print vec2


			    if vec2[0] < 0:
			    	yield boat.move.turn_left(.01).go()

			    	print "moving ", -angle

			    if vec2[0] > 0:
			    	yield boat.move.turn_left(-.01).go()
			    	print "moving  " , angle

			    yield util.sleep(.1)

		    
		    array = MarkerArray()
		    m = Marker()
		    m.header = Header(frame_id = '/enu', stamp = rospy.Time.now())
		    m.ns = 'gates'
		    m.id = 1
		    m.type = Marker.LINE_STRIP
		    m.action = Marker.ADD
		    m.scale.x = 0.2
		    m.color.r = 1
		    m.color.b = 0.75
		    m.color.g = 0.1
		    m.color.a = 1.0
		    m.points.append(position1)
		    m.points.append(position2)
		    array.markers.append(m)

		    m = Marker()
		    m.header = Header(frame_id = '/enu', stamp = rospy.Time.now())
		    m.ns = 'gates'
		    m.id = 2
		    m.type = Marker.LINE_STRIP
		    m.action = Marker.ADD
		    m.scale.x = 0.2
		    m.color.r = 1
		    m.color.b = 0.75
		    m.color.g = 0.1
		    m.color.a = 1.0
		    m.points.append(position3)
		    m.points.append(position4)
		    array.markers.append(m)

		    gate_viz_pub.publish(array)



		    #yield boat.move.yaw_left(angle)

		    
		    
	except Exception, e:
		print e


    

    


