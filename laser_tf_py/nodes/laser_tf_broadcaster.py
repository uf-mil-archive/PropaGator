#!/usr/bin/env python
import roslib
roslib.load_manifest('laser_tf_py')
import rospy
import math
import serial
from time import sleep
import tf
import numpy
import select
from std_msgs.msg import Bool

##########################
## LIDAR Commands:	##
##  S - Sweep		##
##  H - Halt		##
##  m - Set min angle	##
##	(in its units)  ##
##  M - Set max angle	##
##	(in its units)	##
##  All input/commands	##
##	echoed back	##
##  h - User help	##
##  u - User up		##
##  d - User down	##
##########################

port = '/dev/propagator_ioboard'
lidarPivot_to_lens_x = 0.1		# Distance forward from LIDAR pivot to LIDAR lens
lidarPivot_to_lens_y = 0 		# Distance left from LIDAR pivot to LIDAR lens
lidarPivot_to_lens_z = -0.05		# Distance up from LIDAR pivot to LIDAR lens
pitchAngleOffset = 0			# Pitch offset downwards
robotBase_to_lidarPivot_x = 0		# Distance forward
robotBase_to_lidarPivot_y = 0		# Distance left
robotBase_to_lidarPivot_z = 0.8763	# Distance up
baseLink_yaw =  math.pi/7

sweep_max = 320
sweep_min = 0
sweep_play = 10
sweeping_up = True

if __name__ == '__main__':
  rospy.init_node('laser_tf_broadcaster')
  pitch = 0.0
  br = tf.TransformBroadcaster()
  pub_complete = rospy.Publisher('scan_complete', Bool)
  # A positive pitch is a downward pitch from the point of the view of the robot
  try:
    ser = serial.Serial(port, 9600, timeout=1)
    try:
      ser.flushInput()
      ser.write("S")
      while not "S" in ser.readline():
        rospy.logwarn("Resending command to I/O board.")
        ser.write("S")
      while not rospy.is_shutdown():
        try:
          data = ser.readline()
        except (IOError, select.error):
          rospy.logwarn("Did not read from serial port.")

        try:
          pitch = -(float(data) - 138.51)/13.19 + pitchAngleOffset
          if (sweeping_up is True) and (float(data) > sweep_max-sweep_play):
            sweeping_up = False
            pub_complete.publish(True)
            rospy.logdebug("Finished sweep!")
          elif (sweeping_up is False) and (float(data) < sweep_min+sweep_play):
            sweeping_up = True
#            pub_complete.publish(True)
#            rospy.logdebug("Finished sweep!")
        except ValueError:
          rospy.logwarn("Got bad data.")
          ser.close()
          ser.open()
          ser.flushInput()

        T = tf.transformations.rotation_matrix((pitch/180)*math.pi, (0, 1, 0)).dot(tf.transformations.translation_matrix((lidarPivot_to_lens_x, lidarPivot_to_lens_y, lidarPivot_to_lens_z)))
        T = tf.transformations.translation_matrix((robotBase_to_lidarPivot_x, robotBase_to_lidarPivot_y, robotBase_to_lidarPivot_z)).dot(T)
        T = tf.transformations.rotation_matrix((baseLink_yaw), (0, 0, 1)).dot(T)
        T = tf.transformations.translation_from_matrix(T)

        br.sendTransform(T,
         tf.transformations.quaternion_from_euler(math.pi, (pitch/180)*math.pi, baseLink_yaw),
         rospy.Time.now(),
         "/laser",
         "/world")

      ser.flushInput()
      ser.write("H")
      while not "H" in ser.readline():
        rospy.logwarn("Resending halt command to I/O board.")
        ser.write("H")
      ser.close()
    except rospy.ROSInterruptException: pass
  except serial.SerialTimeoutException:
    rospy.logerr("Could not open serial port!")

