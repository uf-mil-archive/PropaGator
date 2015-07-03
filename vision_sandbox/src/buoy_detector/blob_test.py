__author__ = 'Ralph F. Leyva'

#!/usr/bin/env python

import roslib
import sys
import rospy
import cv2

from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

import numpy as np

## COLOR BINS (HSV)
BLUE_MIN_HSV = np.array([111, 0, 0],np.uint8)
BLUE_MAX_HSV = np.array([115, 255, 255],np.uint8)

GREEN_MIN_HSV = np.array([65, 0, 0],np.uint8)
GREEN_MAX_HSV = np.array([75, 255, 255],np.uint8)

ORANGE_MIN_HSV = np.array([0, 50, 50],np.uint8)
ORANGE_MAX_HSV = np.array([19, 255, 255],np.uint8)

RED_MIN_HSV = np.array([165, 0, 0],np.uint8)
RED_MAX_HSV = np.array([179, 255, 255],np.uint8)

YELLOW_MIN_HSV = np.array([22, 0, 0],np.uint8)
YELLOW_MAX_HSV = np.array([45, 255, 255],np.uint8)

#BLACK_MIN_HSV
#BLACK_MAX_HSV


class image_converter:
  def __init__(self):
    self.bridge = CvBridge()
    self.image_pub = rospy.Publisher("/camera/cv2_out", Image)
    self.image_sub = rospy.Subscriber("/camera/image_raw", Image, self.callback)

  def callback(self,data):
    try:
      cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
    except CvBridgeError, e:
      print e

    masked_image = mask_image(cv_image)
    post_processed = process_image(masked_image)
    contour_detect = detect_countours(cv_image, post_processed)

    try:
      self.image_pub.publish(self.bridge.cv2_to_imgmsg(contour_detect, "bgr8"))
    except CvBridgeError, e:
      print e

## Utility Functions
def convert_bgr2lab(cv_image):
  return cv2.cvtColor(cv_image,cv2.COLOR_BGR2LAB)

def convert_bgr2hsv(cv_image):
  return cv2.cvtColor(cv_image, cv2.COLOR_BGR2HSV)

def convert_bgr2greyscale(cv_image):
  return cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)

## Image Processing
def mask_image(cv_image):
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
  return cv2.bitwise_or(yr_or_img, bg_or_img)

def process_image(cv_image):
  kernel = np.ones((5,5),np.uint8)

  eroded_masked_img = cv2.erode(cv_image, kernel, iterations = 3)
  dilatated_masked_img = cv2.dilate(eroded_masked_img,kernel,iterations = 3)
  return dilatated_masked_img

def detect_bouys(processed_cv_image):
  bouy_detector = cv2.SimpleBlobDetector()
  keypoints = bouy_detector.detect(processed_cv_image)
  cv2.drawKeypoints(processed_cv_image, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
  return processed_cv_image

def detect_countours(cv_image, processed_cv_image):
  edges = cv2.Canny(processed_cv_image, 100, 200, True)
  contours, hierarchy = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
  print contours[0]

  #x,y,w,h = cv2.boundingRect(contours[0])
  #img = cv2.rectangle(processed_cv_image,(x,y),(x+w,y+h),(255,0,0),2)
  #return processed_cv_image

  cv2.drawContours(cv_image, contours, -1, (0,255,0), 2)

  #x,y,w,h = cv2.boundingRect(contours[0])
  #img = cv2.rectangle(cv_image,(x,y),(x+w,y+h),(255,0,0),2)
  return cv_image


# Main Function
def main(args):
  ic = image_converter()
  rospy.init_node('image_converter', anonymous=True)

  try:
    rospy.spin()
  except KeyboardInterrupt:
    print "Shutting down"

  cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)
