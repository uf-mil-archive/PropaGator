#!/usr/bin/env python

import os
import sys
import roslib
import rospy
import cv2

from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

import numpy as np

cascade_path = os.path.join(os.path.expanduser('~'), 'catkin_ws', 'src','uf-mil', 'PropaGator', 'camera_docking', 'scripts','cross_01.xml')
print(cascade_path)

class image_converter:
  def __init__(self):
    self.bridge = CvBridge()
    self.image_pub = rospy.Publisher("/camera/cascade_out", Image, queue_size = 1)
    self.image_sub = rospy.Subscriber("/camera/image_raw", Image, self.callback)

  def callback(self,data):
    try:
      cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
    except CvBridgeError, e:
      print e

    cv2.rectangle(cv_image,(0,300),(1240,1080),(0,0,0),-1)

    final_img = image_process(cv_image)

    try:
      self.image_pub.publish(self.bridge.cv2_to_imgmsg(final_img, "bgr8"))
    except CvBridgeError, e:
      print e

# Cascade Classifier
buoy_cascade = cv2.CascadeClassifier(cascade_path)

def image_process(cv_image):
    cv_grey = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)
    buoys = buoy_cascade.detectMultiScale(cv_grey, 3, 5)

    for (x,y,w,h) in buoys:
        cv2.rectangle(cv_image,(x,y),(x+w,y+h),(0,200,25),2)
        roi_gray = cv_grey[y:y+h, x:x+w]
        roi_color = cv_image[y:y+h, x:x+w]
    return cv_image

# MAIN FUNCTION
def main(args):
  rospy.init_node('image_converter', anonymous=True)
  ic = image_converter()

  try:
    rospy.spin()
  except KeyboardInterrupt:
    print "Shutting down"

  cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)
