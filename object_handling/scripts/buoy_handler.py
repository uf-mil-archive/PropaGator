#!/usr/bin/env python
import rospy
from object_handling.msg import Buoys, BuoyStamped
import numpy as np

# This node takes in raw object data and removes all duplicates then republishes an array of data

# All objects are assumed to be in the same reference frame (enu) so no transforms are required

# This mission is the demo day FSM
class object_handler:
	def __init__(self):
		rospy.init_node('object_handler', anonymous=False)
		# Publishers
		self.object_pub = rospy.Publisher('buoys', Buoys, queue_size = 10)
		# Subscribers
		self.object_sub = rospy.Subscriber('/lidar/buoy', BuoyStamped, self.objectCb)
		# Parameters
		# to be to different objects they must be this far apart
		self.same_object_distance = rospy.get_param('~same_buoy_distance', 0.5)

		# to be to different objects they must be this far apart
		self.object_lifetime = rospy.get_param('~buoy_lifetime', 5.0)

		self.objects = list()

		rospy.Timer(rospy.Duration(0.1), self.updateCb)
		
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
		if now - obj.header.stamp.to_sec() < self.object_lifetime:
			return True
		else:
			return False

	def objectCb(self, new_obj):
		# Put the object in the object list
		for i, old_obj in enumerate(self.objects):
			if self.isSameObject(new_obj, old_obj):
				# Replace the current object with its newer self
				self.objects[i] = new_obj
				return

		# If we are this far then its a new object yay
		self.objects.append(new_obj)						

	# Check that all objects are "Fresh" and publish them 
	def updateCb(self, event):
		# Remove old Objects
		self.objects = filter(self.isFreshObject, self.objects)

		# Publish objects
		objects = Buoys()
		objects.header.stamp = rospy.Time.now()
		objects.header.frame_id = '/enu'
		objects.buoys = [obj.buoy for obj in self.objects]
		self.object_pub.publish(objects)



# Run the node
if __name__ == '__main__':
	node = object_handler()
	rospy.spin()
