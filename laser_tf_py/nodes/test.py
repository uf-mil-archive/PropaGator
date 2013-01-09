#!/usr/bin/env python
import roslib
roslib.load_manifest('laser_tf_py')
import rospy
import math
import serial
from time import sleep
import tf
import numpy

if __name__ == '__main__':

        T = tf.transformations.translation_matrix((0, 0, 1)).dot(tf.transformations.translation_matrix((1, 0, -.5)).dot(tf.transformations.rotation_matrix((1/180)*math.pi, (0, 1, 0))))
        T = tf.transformations.translation_from_matrix(T) 



