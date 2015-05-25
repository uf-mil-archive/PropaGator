#!/usr/bin/env python
import roslib
roslib.load_manifest('camera_docking')
import sys
import rospy
import cv2
import numpy as np
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError



class image_converter:

  def __init__(self):
    self.mask = rospy.Publisher("mask",Image, queue_size = 1)
    self.contours = rospy.Publisher("contours",Image, queue_size = 1)

    self.bridge = CvBridge()
    self.image_sub = rospy.Subscriber("/camera/image_raw",Image,self.callback)

  def callback(self,data):
    try:
      cv_image = self.bridge.imgmsg_to_cv2(data, "rgb8")
    except CvBridgeError, e:
      print e

    (rows,cols,channels) = cv_image.shape

    #cv2.line(cv_image, (0,280), (1240,280), (255,0,0), 10) 
    #cv2.line(cv_image, (0,600), (1240,600), (255,0,0), 10)

    cap = cv_image
    
#**************** while(cap.isOpened()): *************************************************************
    counter = 0
    sign1_sum = []
    sign2_sum = []
    sign3_sum = []

    frame = cap
    frame_real = cap
    
    height = frame_real.shape[0]
    width  = frame_real.shape[1]

    #cv2.rectangle(frame,(0,0),(1240,280),(0,0,0),-1)
    #cv2.rectangle(frame,(0,600),(1240,1080),(0,0,0),-1)
    imgray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    imgray = cv2.medianBlur(imgray,3)   



    
#    ret,thresh = cv2.threshold(imgray,230,255,cv2.THRESH_TOZERO)
    


    #ret,thresh = cv2.threshold(imgray,200,255,cv2.THRESH_TOZERO) #Adjust the first digit after imgray to increase sensitivity to white boards.  Default 230.

    ret, thresh = cv2.threshold(imgray,127,255,0)
    mask = np.zeros(thresh.shape,np.uint8)

    #contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) 
    
    #kernel = np.ones((7,7),np.uint8)
    #thresh = cv2.erode(thresh,kernel,iterations = 1)

    contours, hier = cv2.findContours(thresh,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)
    for cnt in contours:
        if 200<cv2.contourArea(cnt)<5000:
            ellipse = cv2.fitEllipse(cnt)
            im = cv2.ellipse(cv_image,ellipse,(0,255,0),2)

            #cv2.drawContours(cv_image,[cnt],0,(0,255,0),2)
            cv2.drawContours(mask,[cnt],0,255,-1)


    try:
      self.contours.publish(self.bridge.cv2_to_imgmsg(cv_image, "rgb8"))
      self.mask.publish(self.bridge.cv2_to_imgmsg(mask, "8UC1"))    
      #self.image_pub.publish(self.bridge.cv2_to_imgmsg(thresh, "8UC1"))
    except CvBridgeError, e:
      print e

def main(args):
  rospy.init_node('finding_white_signs')

  i = 0
  ic = image_converter()

  try:
    rospy.spin()
  except KeyboardInterrupt:
    print "Shutting down"
  cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)
