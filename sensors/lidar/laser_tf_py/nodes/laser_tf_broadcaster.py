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


OFFBOARD_TESTING = False
LIDAR_UPSIDE_DOWN = False	# T = LIDAR mounted with lense on bottom (upside down)

global IN_CALLBACK
IN_CALLBACK = False

#initial sweeping range
min_start = 15	#40
max_start =0
	#-10

#calibration points for angles
point1 = [2030,25]#35	#point = [encoder value,angle]
point2 = [3160,-20]#-20

m = (float(point2[1]-point1[1]))/(point2[0]-point1[0])
b = -(m*point1[0]-point1[1])

time_fudge = -.05	# adjust this number to calibrate offset in time laser_scan is received vs time tf received

#m = -0.0862588	# y is angle (degrees)
#b = 158.661986	# x is ticks of encoder

port = '/dev/propagator_navid_motordriver'

# IN RADIANS, NOT DEGREES!!
if (OFFBOARD_TESTING):	# Constants for when the lidar is off the boat
  bl_lu_x	= 0.8001	# Distance forward, BaseLink to Lidar Unit
  bl_lu_y	= 0	# Distance left, BaseLink to Lidar Unit
  bl_lu_z	= 0.254		# Distance up, BaseLink to Lidar Unit
  lu_r		= 0		# Roll of Lidar Unit
  lu_p		= 0		# Pitch of Lidar Unit
  lu_y		= 0		# Yaw of Lidar Unit
  lu_lp_x	= 0		# Distance forward, Lidar Unit to Lidar Pivot
  lu_lp_y	= 0		# Distance left, Lidar Unit to Lidar Pivot
  lu_lp_z	= 0		# Distance up, Lidar Unit to Lidar Pivot
  lp_r		= 0		# Roll of Lidar Pivot
  lp_p		= 0		# Pitch of Lidar Pivot
  lp_y		= 0		# Yaw of Lidar Pivot
  lp_lm_x	= 0.0127	# Distance forward, Lidar Pivot to Lidar Mirror
  lp_lm_y	= 0		# Distance left, Lidar Pivot to Lidar Mirror
  lp_lm_z	= 0.0317	# Distance up, Lidar Pivot to Lidar Mirror
else:
  bl_lu_x	= 0.8001	# Distance forward, BaseLink to Lidar Unit
  bl_lu_y	= 0	# Distance left, BaseLink to Lidar Unit
  bl_lu_z	= 0.254		# Distance up, BaseLink to Lidar Unit
  lu_r		= 0		# Roll of Lidar Unit
  lu_p		= 0		# Pitch of Lidar Unit
  lu_y		= 0		# Yaw of Lidar Unit
  lu_lp_x	= 0		# Distance forward, Lidar Unit to Lidar Pivot
  lu_lp_y	= 0		# Distance left, Lidar Unit to Lidar Pivot
  lu_lp_z	= 0		# Distance up, Lidar Unit to Lidar Pivot
  lp_r		= 0		# Roll of Lidar Pivot
  lp_p		= 0		# Pitch of Lidar Pivot
  lp_y		= 0		# Yaw of Lidar Pivot
  lp_lm_x	= 0.0127	# Distance forward, Lidar Pivot to Lidar Mirror
  lp_lm_y	= 0		# Distance left, Lidar Pivot to Lidar Mirror
  lp_lm_z	= 0.0317	# Distance up, Lidar Pivot to Lidar Mirror

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#!! min_angle is bottom (most positive) !!
#!! angle. max_angle is top		!!
#!! most negative angle			!!
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

def scan_angle_callback(angle_msg):
  global IN_CALLBACK
  IN_CALLBACK = True
  sleep(2)
  rospy.logerr("start of callback")
  # update angles
  min_angle = angle_msg.min_angle
  max_angle = angle_msg.max_angle

  min_angle = int((min_angle - b)/m)
  max_angle = int((max_angle - b)/m)
  rospy.logerr("new angles: "+str(min_angle)+" and "+str(max_angle))

  ser.flushInput()
  ser.write("H")
  msg = ser.readline()
  while not "H" in msg:
    rospy.logwarn("Resending halt command to I/O board. Got "+msg+" not H")
    ser.write("H")
    msg = ser.readline()

  ser.flushInput()
  ser.write("l"+str(min_angle)+"\r")
  msg = ser.readline()

  while not "l"+str(min_angle) in msg:
    rospy.logwarn("Retrying to send min angle. Got "+msg+" not "+"l"+str(min_angle))
    ser.write("l"+str(min_angle)+"\r")
    msg = ser.readline()
  rospy.logdebug("Min angle updated to "+str(min_angle))

  ser.flushInput()
  ser.write("h"+str(max_angle)+"\r")
  msg = ser.readline()
  while not "h"+str(max_angle) in msg:
    rospy.logwarn("Retrying to send max angle. Got "+msg+" not "+"h"+str(min_angle))
    ser.write("h"+str(max_angle)+"\r")
    msg = ser.readline()
  rospy.logdebug("Max angle updated to "+str(max_angle))

  ser.flushInput()
  ser.write("S")
  msg = ser.readline()
  while not "S" in msg:
    rospy.logwarn("Resending command to I/O board. Got "+msg+" not S")
    ser.write("S")
    msg = ser.readline()

  rospy.logerr("end of callback")
  IN_CALLBACK = False

if __name__ == '__main__':
  rospy.init_node('laser_tf_broadcaster')
  pitch = 0.0
  br = tf.TransformBroadcaster()
  pub_complete = rospy.Publisher('scan_complete', Bool)
  rospy.Subscriber("/lidar_angle", ScanAngle, scan_angle_callback)
  # A positive pitch is a downward pitch from the point of the view of the robot
  try:
    ser = serial.Serial(port, 115200, timeout=1)
    try:
      ser.flushInput()
      ser.write("R")

      min_start = int((min_start - b)/m)
      max_start = int((max_start - b)/m)
      rospy.logerr("new angles: "+str(min_start)+" and "+str(max_start))

      ser.flushInput()
      ser.write("l"+str(min_start)+"\r")
      msg = ser.readline()
  

      while not "l"+str(min_start) in msg:
        rospy.logwarn("Retrying to send min angle. Got "+msg+" not "+"l"+str(min_start))
        ser.write("l"+str(min_start)+"\r")
        msg = ser.readline()
      rospy.logdebug("Min angle updated to "+str(min_start))

      ser.flushInput()
      ser.write("h"+str(max_start)+"\r")
      msg = ser.readline()
      while not "h"+str(max_start) in msg:
        rospy.logwarn("Retrying to send max angle. Got "+msg+" not "+"h"+str(min_start))
        ser.write("h"+str(max_start)+"\r")
        msg = ser.readline()
      rospy.logdebug("Max angle updated to "+str(max_start))
      
      ser.write("s800\r")
      for i in range(15):
      	ser.write("u")

      print "got back",ser.readline()
      ser.write("S")
      while not "S" in ser.readline():
        rospy.logdebug("Resending command to I/O board.")
        ser.write("S")
      sleep(4)
      while not rospy.is_shutdown():
        if (not IN_CALLBACK):
          try:
            data = ser.readline()
            time = rospy.Time.now()
	  except (IOError, select.error):
            rospy.logwarn("Did not read from serial port.")
            ser.close()
            ser.open()
            ser.flushInput()

          try:
            pitch = (m*float(data)) + b
            lp_p = (pitch/180)*math.pi
           # rospy.logerr(lp_p)
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

          br.sendTransform(T,
           tf.transformations.quaternion_from_euler(total_roll, total_pitch, total_yaw),
#           time,
           rospy.Time.now(), #+ rospy.Duration(time_fudge),
           "/laser",
           "/base_link")
#          rospy.logerr("published new laser tf")

      ser.flushInput()
      ser.write("H")
      rospy.logerr("leaving node")
      msg = ser.readline()
      '''
      while not "H" in msg:
        ser.flushInput()
        rospy.logwarn("Resending halt command to I/O board to end process. "+msg)
        ser.write("H")
        msg = ser.readline()
      '''
      ser.close()
    except rospy.ROSInterruptException:
      rospy.logerr("interruptException")
  except serial.SerialTimeoutException:
    rospy.logdebug("Could not open serial port!")

