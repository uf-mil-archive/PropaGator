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
from laser_tf_py.msg import ScanAngle

##########################
## LIDAR Commands:	##
##  S - Sweep		##
##  H - Halt		##
##  All input/commands	##
##	echoed back	##
##  h - Set high angle	##
##  l - Set low angle	##
##  u - User up		##
##  d - User down	##
##########################

OFFBOARD_TESTING = True;	# T = Boat is not in water
LIDAR_UPSIDE_DOWN = False;	# T = LIDAR mounted with lense on bottom (upside down)

port = '/dev/propagator_navid_motordriver'

# IN RADIANS, NOT DEGREES!!
if (OFFBOARD_TESTING):	# Constants for when the lidar is off the boat
  bl_lu_x	= 0		# Distance forward, BaseLink to Lidar Unit
  bl_lu_y	= 0		# Distance left, BaseLink to Lidar Unit
  bl_lu_z	= 1		# Distance up, BaseLink to Lidar Unit
  lu_r		= 0		# Roll of Lidar Unit
  lu_p		= 0		# Pitch of Lidar Unit
  lu_y		= 0		# Yaw of Lidar Unit
  lu_lp_x	= 0		# Distance forward, Lidar Unit to Lidar Pivot
  lu_lp_y	= 0		# Distance left, Lidar Unit to Lidar Pivot
  lu_lp_z	= 0		# Distance up, Lidar Unit to Lidar Pivot
  lp_r		= 0		# Roll of Lidar Pivot
  lp_p		= 0		# Pitch of Lidar Pivot
  lp_y		= 0		# Yaw of Lidar Pivot
  lp_lm_x	= 0.1016	# Distance forward, Lidar Pivot to Lidar Mirror
  lp_lm_y	= 0#!		# Distance left, Lidar Pivot to Lidar Mirror
  lp_lm_z	= -0.0508	# Distance up, Lidar Pivot to Lidar Mirror
else:
  bl_lu_x	= 0.5588	# Distance forward, BaseLink to Lidar Unit
  bl_lu_y	= 0		# Distance left, BaseLink to Lidar Unit
  bl_lu_z	= 0.1524	# Distance up, BaseLink to Lidar Unit
  lu_r		= 0		# Roll of Lidar Unit
  lu_p		= 0		# Pitch of Lidar Unit
  lu_y		= 0		# Yaw of Lidar Unit
  lu_lp_x	= 0		# Distance forward, Lidar Unit to Lidar Pivot
  lu_lp_y	= 0		# Distance left, Lidar Unit to Lidar Pivot
  lu_lp_z	= 0		# Distance up, Lidar Unit to Lidar Pivot
  lp_r		= 0		# Roll of Lidar Pivot
  lp_p		= 0		# Pitch of Lidar Pivot
  lp_y		= 0		# Yaw of Lidar Pivot
  lp_lm_x	= 0.1016	# Distance forward, Lidar Pivot to Lidar Mirror
  lp_lm_y	= 0#!		# Distance left, Lidar Pivot to Lidar Mirror
  lp_lm_z	= -0.0508	# Distance up, Lidar Pivot to Lidar Mirror

def scan_angle_callback(angle_msg):
  # update angles
  min_angle = angle_msg.min_angle
  max_angle = angle_msg.max_angle

  ser.flushInput()
  ser.write("l")
  while not "l" in ser.readline():
    rospy.logwarn("Retrying to init angle set min state.")
    ser.write("l")

  ser.flushInput()
  ser.write(str(min_angle))
  while not "l"+str(min_angle) in ser.readline():
    rospy.logwarn("Retrying to send min angle.")
    ser.write(str(min_angle))
  rospy.logdebug("Min angle updated to "+str(min_angle))

  ser.flushInput()
  ser.write("h")
  while not "h" in ser.readline():
    rospy.logwarn("Retrying to init angle set max state.")
    ser.write("h")

  ser.flushInput()
  ser.write(str(max_angle))
  while not "h"+str(max_angle) in ser.readline():
    rospy.logwarn("Retrying to send max angle.")
    ser.write(str(max_angle))
  rospy.logdebug("Max angle updated to "+str(max_angle))


if __name__ == '__main__':
  rospy.init_node('laser_tf_broadcaster')
  pitch = 0.0
  br = tf.TransformBroadcaster()
  pub_complete = rospy.Publisher('scan_complete', Bool)
  rospy.Subscriber("/lidar_angle", ScanAngle, scan_angle_callback)
  # A positive pitch is a downward pitch from the point of the view of the robot
  try:
    ser = serial.Serial(port, 9600, timeout=1)
    try:
      ser.flushInput()
      ser.write("S")
      while not "S" in ser.readline():
        rospy.logwarn("Resending command to I/O board.")
        ser.write("S")
      sleep(4)
      while not rospy.is_shutdown():
        try:
          data = ser.readline()
        except (IOError, select.error):
          rospy.logwarn("Did not read from serial port.")
          ser.close()
          ser.open()
          ser.flushInput()

        try:
          pitch = (float(data) - 288)/11 + pitchAngleOffset
          lp_p = (pitch/180)*math.pi
        except ValueError:
          if ("C" in data):	# If we got a scan complete signal
            pub_complete.publish(True)
            rospy.logdebug("Finished sweep!")
          else:			# Otherwise we got garbage
            rospy.logwarn("Got bad data:")
            rospy.logwarn(data)
            ser.close()
            ser.open()
            ser.flushInput()

        T = tf.transformations.translation_matrix((lp_lm_x, lp_lm_y, lp_lm_z))
        T = tf.transformations.rotation_matrix((lp_y), (0, 0, 1)).dot(T)
        T = tf.transformations.rotation_matrix((lp_p), (0, 1, 0)).dot(T)
        T = tf.transformations.rotation_matrix((lp_r), (1, 0, 0)).dot(T)
        T = tf.transformations.translation_matrix((lu_lp_x, lu_lp_y, lu_lp_z)).dot(T)
        T = tf.transformations.rotation_matrix((lu_y), (0, 0, 1)).dot(T)
        T = tf.transformations.rotation_matrix((lu_p), (0, 1, 0)).dot(T)
        T = tf.transformations.rotation_matrix((lu_r), (1, 0, 0)).dot(T)
        T = tf.transformations.translation_matrix((bl_lu_x, bl_lu_y, bl_lu_z)).dot(T)
        T = tf.transformations.translation_from_matrix(T)

        total_roll = lu_r + lp_r
        total_pitch = lu_p + lp_p
        total_yaw = lu_y + lp_y

        if (LIDAR_UPSIDE_DOWN):
          total_roll = total_roll + math.pi	# compensates for Lidar being mounted upside down

        if (OFFBOARD_TESTING):
          fframe = "/world"
        else:	# otherwise we're on the boat
          fframe = "/base_link"

        br.sendTransform(T,
         tf.transformations.quaternion_from_euler(total_roll, total_pitch, total_yaw),
         rospy.Time.now(),
         "/laser",
         fframe)

      ser.flushInput()
      ser.write("H")
      while not "H" in ser.readline():
        ser.flushInput()
        rospy.logwarn("Resending halt command to I/O board.")
        ser.write("H")
      ser.close()
    except rospy.ROSInterruptException: pass
  except serial.SerialTimeoutException:
    rospy.logdebug("Could not open serial port!")

