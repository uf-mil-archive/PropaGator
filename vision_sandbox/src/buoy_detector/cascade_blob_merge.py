__author__ = 'Ralph F. Leyva'

import os
import sys
import roslib
import rospy
import cv2

from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

import numpy as np

## Cascade Classifier
cascade_path = os.path.join(os.path.expanduser('~'), 'Desktop', 'Classifiers', 'cascade.xml')
buoy_cascade = cv2.CascadeClassifier(cascade_path)

BLUE_MIN_HSV = np.array([107, 0, 0],np.uint8)
BLUE_MAX_HSV = np.array([111, 255, 255],np.uint8)

GREEN_MIN_HSV = np.array([70, 0, 0],np.uint8)
GREEN_MAX_HSV = np.array([85, 255, 255],np.uint8)

ORANGE_MIN_HSV = np.array([0, 50, 50],np.uint8)
ORANGE_MAX_HSV = np.array([19, 255, 255],np.uint8)

RED_MIN_HSV = np.array([175, 0, 0],np.uint8)
RED_MAX_HSV = np.array([179, 255, 255],np.uint8)

YELLOW_MIN_HSV = np.array([25, 0, 0],np.uint8)
YELLOW_MAX_HSV = np.array([33, 255, 255],np.uint8)

## Utility Functions
def convert_bgr2lab(cv_image):
  return cv2.cvtColor(cv_image,cv2.COLOR_BGR2LAB)

def convert_bgr2hsv(cv_image):
  return cv2.cvtColor(cv_image, cv2.COLOR_BGR2HSV)

def convert_bgr2greyscale(cv_image):
  return cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)

## Translates image from ROS Image msg to OpenCV/NumPy array
class image_converter:
  def __init__(self):
    self.bridge = CvBridge()
    self.image_blob_pub = rospy.Publisher("/camera/cv2_blob_out", Image)
    self.image_cascade_pub = rospy.Publisher("/camera/cv2_cascade_out", Image)
    self.image_sub = rospy.Subscriber("/camera/image_raw", Image, self.callback)

  def callback(self,data):
    try:
      cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
    except CvBridgeError, e:
      print e

    temp_blobs_img = blob_process(cv_image)
    found_blobs_img = detect_bouys(temp_blobs_img)
    cascade_blobs_img = cascade_process(cv_image)

    #final_out = cv2.bitwise_or(found_blobs_img, cascade_blobs_img)

    try:
      self.image_blob_pub.publish(self.bridge.cv2_to_imgmsg(found_blobs_img, "bgr8"))
      self.image_cascade_pub.publish(self.bridge.cv2_to_imgmsg(cascade_blobs_img,"bgr8"))
      #self.image_cascade_pub.publish(self.bridge.cv2_to_imgmsg(final_out,"bgr8"))
    except CvBridgeError, e:
      print e

# Buoy detection frunctions
def cascade_process(cv_image):
  cv_grey = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)

  buoys = buoy_cascade.detectMultiScale(cv_grey, 3, 5)

  for (x,y,w,h) in buoys:
    cv2.rectangle(cv_image,(x,y),(x+w,y+h),(0,255,0),2)
    roi_gray = cv_grey[y:y+h, x:x+w]
    roi_color = cv_image[y:y+h, x:x+w]
  return cv_image

def blob_process(cv_image):
  hsv_cv_image = convert_bgr2hsv(cv_image)

  yellow_mask_hsv = cv2.inRange(hsv_cv_image, YELLOW_MIN_HSV, YELLOW_MAX_HSV)
  red_mask_hsv = cv2.inRange(hsv_cv_image, RED_MIN_HSV, RED_MAX_HSV)
  green_mask_hsv = cv2.inRange(hsv_cv_image, GREEN_MIN_HSV, GREEN_MAX_HSV)
  blue_mask_hsv = cv2.inRange(hsv_cv_image, BLUE_MIN_HSV, BLUE_MAX_HSV)

  yellow_masked_img = cv2.bitwise_and(cv_image, cv_image, mask=yellow_mask_hsv)
  red_masked_img = cv2.bitwise_and(cv_image, cv_image, mask=red_mask_hsv)
  green_masked_img = cv2.bitwise_and(cv_image, cv_image, mask=green_mask_hsv)
  blue_masked_img = cv2.bitwise_and(cv_image, cv_image, mask=blue_mask_hsv)

  yr_or_img = cv2.bitwise_or(yellow_masked_img, red_masked_img)
  bg_or_img = cv2.bitwise_or(green_masked_img, blue_masked_img)
  final_or_img = cv2.bitwise_or(yr_or_img, bg_or_img)
  #final_or_img = cv2.bitwise_or(yr_or_img, green_masked_img)

  #kernel = np.ones((5,5),np.uint8)
  kernel = np.array([[0,1,1,1,0],[1,1,1,1,1],[1,1,1,1,1],[1,1,1,1,1],[0,1,1,1,0]],np.uint8)

  eroded_masked_img = cv2.erode(final_or_img, kernel, iterations = 1)
  dilatated_masked_img = cv2.dilate(eroded_masked_img,kernel,iterations = 2)
  #ilatated_masked_img = cv2.dilate(cv_image,kernel,iterations = 1)
  #bilateral = cv2.bilateralFilter(cv_image, 2,25,25)
  return dilatated_masked_img
  #return bilateral
  bouy_detector = cv2.SimpleBlobDetector()
  keypoints = bouy_detector.detect(processed_cv_image)
  cv2.drawKeypoints(processed_cv_image, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
  return processed_cv_image

def detect_countours(cv_image, processed_cv_image):
  edges = cv2.Canny(processed_cv_image, 100, 100)
  contours, hierarchy = cv2.findContours(edges, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
  print contours[0]

  x,y,w,h = cv2.boundingRect(contours[0])
  cv2.rectangle(cv_image,(x,y),(x+w,y+h),(255,0,0),2)
  return cv_image

# MAIN FUNCTION
def main(args):
  ic = image_converter()
  rospy.init_node('image_converter', anonymous=True)

  try:
    rospy.spin()
  except KeyboardInterrupt:
    print "Shutting down"

if __name__ == '__main__':
  main(sys.argv)
