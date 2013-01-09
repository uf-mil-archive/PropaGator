#!/usr/bin/env python
import roslib
roslib.load_manifest('laser_tf_py')
import rospy
import math
import serial
from time import sleep
import tf
import numpy

port = '/dev/propagator_ioboard'
lidarPivot_to_lens_x = .1	#0.01	# Distance forward from LIDAR pivot to LIDAR lens
lidarPivot_to_lens_y = 0 	#0	# Distance left from LIDAR pivot to LIDAR lens
lidarPivot_to_lens_z = -.05	#-.1	# Distance up from LIDAR pivot to LIDAR lens
pitchAngleOffset = 1	#3		# Pitch offset downwards
robotBase_to_lidarPivot_x = 0		# Distance forward
robotBase_to_lidarPivot_y = 0		# Distance left
robotBase_to_lidarPivot_z = 0.8763	# Distance up

if __name__ == '__main__':
  rospy.init_node('laser_tf_broadcaster')
  pitch = 0.0
  br = tf.TransformBroadcaster()
  # A positive pitch is a downward pitch from the point of the view of the robot
  try:
    ser = serial.Serial(port, 9600, timeout=1)
    try:
      ser.write("s")
      while not rospy.is_shutdown():
        try:
          data = ser.readline()
        except IOError:
          rospy.logwarn("Did not read from serial port.")
        print "Got data:", data 

        try:
#          pitch = -(((float(data))/10) - 17) + pitchAngleOffset
          pitch = -(((float(data))/15) - 17) + pitchAngleOffset
        except ValueError:
          rospy.logwarn("Got bad data.")
          ser.close()
          ser.open()

        T = tf.transformations.rotation_matrix((pitch/180)*math.pi, (0, 1, 0)).dot(tf.transformations.translation_matrix((lidarPivot_to_lens_x, lidarPivot_to_lens_y, lidarPivot_to_lens_z)))
        T = tf.transformations.translation_matrix((robotBase_to_lidarPivot_x, robotBase_to_lidarPivot_y, robotBase_to_lidarPivot_z)).dot(T)
        T = tf.transformations.translation_from_matrix(T)

#        T = (0, 0, 1)

        br.sendTransform(T,
         tf.transformations.quaternion_from_euler(math.pi, (pitch/180)*math.pi, 0),
         rospy.Time.now(),
         "/laser",
         "/world")
    except rospy.ROSInterruptException: pass
  except SerialTimeoutException:
    rospy.logerr("Could not open serial port!")
