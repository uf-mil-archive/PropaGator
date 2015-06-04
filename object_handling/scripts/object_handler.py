#!/usr/bin/env python
import rospy
from lidar_vision.msg import buoy
from object_handling.msg import Buoys
import numpy as np

# This node takes in raw object data and removes all duplicates then republishes an array of data

# All objects are assumed to be in the same reference frame (enu) so no transforms are required

# This mission is the demo day FSM
class object_handler:
	def __init__(self):
		rospy.init_node('object_handler', anonymous=False)
		# Publishers
		self.object_pub = rospy.Publisher('/objects', Buoys, queue_size = 10)
		# Subscribers
		self.object_sub = rospy.Subscriber('/lidar/buoy', buoy, self.objectCb)
		# Parameters
		self.same_object_distance = [ # to be to different objects they must be this far apart
			rospy.get_param('~same_buoy_distance', 0.1)]

		self.object_lifetime = [# to be to different objects they must be this far apart
			rospy.get_param('~buoy_lifetime', 5.0)]

		self.num_types = 1
		self.objects = [list() for x in range(self.num_types)]		# Do the callbacks run in different threads? Will this cause a problem???

		rospy.Timer(rospy.Duration(0.1), self.updateCb)
		
	def getDistanceBetweenObjects(self, obj1, obj2):
		return np.linalg.norm(np.array(
						[obj1.position.x - obj2.position.x, 
						 obj1.position.y - obj2.position.y,
						 obj1.position.z - obj2.position.z]))

	# Check the distance and type of object
	def isSameObject(self, obj1, obj2):
		# Check type
		if obj1.type != obj2.type:
			return False

		# Get the distance
		distance = self.getDistanceBetweenObjects(obj1, obj2)

		# Check distance
		if distance < self.same_object_distance[obj1.type]:
			return True
		else:
			return False

	# make sure the object hasn't lived longer than it should
	def isFreshObject(self, obj):
		now = rospy.get_time()
		if now - obj.header.stamp.to_sec() < self.object_lifetime[obj.type]:
			return True
		else:
			return False

	def objectCb(self, new_obj):
		# Put the object in the object list
		for i, old_obj in enumerate(self.objects[new_obj.type]):
			if self.isSameObject(new_obj, old_obj):
				# Replace the current object with its newer self
				self.objects[new_obj.type][i] = new_obj
				return

		# If we are this far then its a new object yay
		self.objects[new_obj.type].append(new_obj)

	# Check that all objects are "Fresh" and publish them 
	def updateCb(self, event):
		# This loop needs to be updated if we have more object types
		#	previously this was done with a standard object type rather than
		#	a msg type for each type of object
		for i in range(self.num_types):
			# Remove old Objects
			self.objects[i] = filter(self.isFreshObject, self.objects[i])

			# Publish objects
			objects = Buoys()
			objects.buoys = self.objects[i]
			self.object_pub.publish(objects)



# Run the node
if __name__ == '__main__':
	node = object_handler()
	rospy.spin()
