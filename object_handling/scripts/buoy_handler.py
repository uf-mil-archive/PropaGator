#!/usr/bin/env python
import rospy
from object_handling.msg import Buoys, BuoyStamped
from visualization_msgs.msg import MarkerArray, Marker
from std_msgs.msg import Header
import numpy as np

# This node takes in raw object data and removes all duplicates then republishes an array of data

# All objects are assumed to be in the same reference frame (enu) so no transforms are required

# This mission is the demo day FSM
class object_handler:
	def __init__(self):
		rospy.init_node('object_handler', anonymous=False)
		# Publishers
		self.buoy_pub = rospy.Publisher('buoys', Buoys, queue_size = 10)
		self.buoy_viz_pub = rospy.Publisher('buoys_viz', MarkerArray, queue_size = 10)
		# Subscribers
		self.buoy_sub = rospy.Subscriber('/lidar/buoy', BuoyStamped, self.objectCb)
		# Parameters
		# to be to different objects they must be this far apart
		self.same_object_distance = rospy.get_param('~same_buoy_distance', 0.5)

		# to be to different objects they must be this far apart
		self.lifetime = rospy.get_param('~buoy_lifetime', 5.0)

		self.buoys = list()

		rospy.Timer(rospy.Duration(0.1), self.updateCb)

	def visualize(self):
		m_array = MarkerArray()
		for i, b in enumerate(self.buoys):
			m = Marker()
			m.header = Header(frame_id = '/enu', stamp = rospy.Time.now())
			m.ns = 'buoys'
			m.id = i
			m.type = Marker.SPHERE
			m.action = Marker.ADD
			m.pose.position = b.buoy.position
			m.scale.x = m.scale.y = m.scale.z = b.buoy.radius * 1.5
			m.color.r = 0.5
			m.color.b = 0.1
			m.color.g = 0.75
			m.color.a = 1.0
			m.lifetime = rospy.Duration(self.lifetime)
			m_array.markers.append(m)

		self.buoy_viz_pub.publish(m_array)

		
	def getDistanceBetweenObjects(self, obj1, obj2):
		return np.linalg.norm(np.array(
						[obj1.buoy.position.x - obj2.buoy.position.x, 
						 obj1.buoy.position.y - obj2.buoy.position.y,
						 obj1.buoy.position.z - obj2.buoy.position.z]))

	# Check the distance and type of object
	def isSameObject(self, obj1, obj2):
		# Get the distance
		distance = self.getDistanceBetweenObjects(obj1, obj2)

		# Check distance
		if distance < self.same_object_distance:				
			return True
		else:
			return False

	# make sure the object hasn't lived longer than it should
	def isFreshObject(self, obj):
		now = rospy.get_time()
		if now - obj.header.stamp.to_sec() < self.lifetime:
			return True
		else:
			return False

	def objectCb(self, new_obj):
		# Put the object in the object list
		for i, old_obj in enumerate(self.buoys):
			if self.isSameObject(new_obj, old_obj):
				# Replace the current object with its newer self
				self.buoys[i] = new_obj
				return

		# If we are this far then its a new object yay
		self.buoys.append(new_obj)						

	# Check that all objects are "Fresh" and publish them 
	def updateCb(self, event):
		# Remove old Objects
		self.buoys = filter(self.isFreshObject, self.buoys)

		# Publish objects
		objects = Buoys()
		objects.header.stamp = rospy.Time.now()
		objects.header.frame_id = '/enu'
		objects.buoys = [obj.buoy for obj in self.buoys]
		self.buoy_pub.publish(objects)

		self.visualize()



# Run the node
if __name__ == '__main__':
	node = object_handler()
	rospy.spin()
