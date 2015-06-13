#!/usr/bin/env python
import roslib
roslib.load_manifest('start_gate_vision')
import sys
import rospy
import cv2
from std_msgs.msg import Float64
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import numpy as np
class image_converter:

  def __init__(self):
    self.angle_pub = rospy.Publisher("start_gate_vision", Float64, queue_size=1)

    cv2.namedWindow("Image window", 1)
    self.bridge = CvBridge()
    self.image_sub = rospy.Subscriber("/camera/image_raw", Image, self.callback)

  
  
  def callback(self,data):
    try:
      img = self.bridge.imgmsg_to_cv2(data, "bgr8")
    except CvBridgeError, e:
      print e

    img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)#HSV is good for color detection with lighting flux  

    #thresholding and building heat mask
    h1 = 66
    s1 = 126
    v1 = 130
    h2 = 179
    s2 = 220
    v2 = 255
    lower_value = np.array([h1, s1, v1], np.uint8)
    upper_value = np.array([h2, s2, v2], np.uint8)
    #endthresholding
    temp = np.array(0)
    mask = cv2.inRange(img, lower_value, upper_value)
    bImg = cv2.bitwise_or(mask, temp)
    #kernel = np.ones((150,150),np.uint8)
    #bImg = cv2.morphologyEx(bImg, cv2.MORPH_CLOSE, kernel)#messed something up was included in a tutorial not entirely required here
    ret, thresh = cv2.threshold(bImg, 127, 255, 0)
    #end thresholding and building heat mask
    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    def contour_area_quick_sort(l):
      length = len(l)
      if length <=1:
          return l
      else:
          pivot = l.pop(int(length/2))
          less, more = [], []
          for x in l:
              if cv2.contourArea(x)>=cv2.contourArea(pivot):
                  less.append(x)
              else:
                  more.append(x)
          return contour_area_quick_sort(less) + [pivot] + contour_area_quick_sort(more)    
    contours = contour_area_quick_sort(contours)
    moment0 = cv2.moments(contours[0])
    cx0 = int(moment0['m10']/moment0['m00'])
    img = cv2.line(img,(cx0,0),(cx0,10000),(0,0,0),5)
    moment1 = cv2.moments(contours[1])
    cx1 = int(moment1['m10']/moment1['m00'])
    img = cv2.line(img,(cx1,0),(cx1,10000),(0,0,0),5)
    #contours = cv2.findContours(thresh, 1, 2)#findimg contours really just masses of colors
    #img = cv2.drawContours(img, contours, 0, (0,0,0), 3)
    #img = cv2.drawContours(img, contours, 1, (0,0,0), 3)
    collums_midpoint = (cx0+cx1)/2
    angle = collums_midpoint*((np.pi/2)/1000)#1000 pixels over pi/2 radians view
    #cnt = contours[0]                        #selecting main contour    
    #x,y,w,h = cv2.boundingRect(cnt)
    #cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),2)
    #cv2.imshow("Gates",img)
    #cv2.waitKey()
    #cv2.destroyAllWindows()
        
    self.angle_pub.publish(Float64(data=angle))
  
def main(args):
  ic = image_converter()
  try:
    rospy.spin()
  except KeyboardInterrupt:
    print "Shutting down"
  cv2.destroyAllWindows()

if __name__ == '__main__':
    rospy.init_node('start_gate_vision')
    main(sys.argv)