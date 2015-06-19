#!/usr/bin/env python
import roslib
roslib.load_manifest('camera_docking')
import sys
import rospy
import cv2
import numpy as np
import math
import cv2.cv as cv
import colorsys
import time

from std_msgs.msg import Bool, Int16, String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

font = cv2.FONT_HERSHEY_SIMPLEX

#Hough_circle values
p1 = 100 #100
p2 = 13 #13
nr = 5 #20
mr = 5000 #30
#*******************

class image_converter:

  def __init__(self):

    self.mask = rospy.Publisher("mask",Image, queue_size = 1)
    self.contours = rospy.Publisher("contours",Image, queue_size = 1)
    self.cv_image = rospy.Publisher("cv_image",Image, queue_size = 1)

    self.circle_detected = rospy.Publisher("circle_detected", Bool, queue_size = 5)
    self.circle_x_pos = rospy.Publisher("circle_x_pos", Int16, queue_size = 5)

    self.cross_detected = rospy.Publisher("cross_detected", Bool, queue_size = 5)
    self.cross_x_pos = rospy.Publisher("cross_x_pos", Int16, queue_size = 5)

    self.triangle_detected = rospy.Publisher("triangle_detected", Bool, queue_size = 5)
    self.triangle_x_pos = rospy.Publisher("triangle_x_pos", Int16, queue_size = 5)

    self.bridge = CvBridge()
    self.image_sub = rospy.Subscriber("/camera/image_raw",Image,self.callback)

  def callback(self,data):
    try:
      cv_image = self.bridge.imgmsg_to_cv2(data, "rgb8")
    except CvBridgeError, e:
      print e

    (rows,cols,channels) = cv_image.shape

    #cv2.line(cv_image, (0,280), (1240,280), (255,0,0), 10) 
    #cv2.line(cv_image, (995,0), (995,300), (255,0,0), 10)
    #image size is 1000 pixels across


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

    #cv2.rectangle(frame,(0,0),(1240,50),(0,0,0),-1)
    cv2.rectangle(frame,(0,300),(1240,1080),(0,0,0),-1)
    imgray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    #imgray = cv2.medianBlur(imgray,3)   

    imgray2 = imgray

    imgray = cv2.GaussianBlur(imgray,(1,1),1)

    #imgray2 = cv2.equalizeHist(imgray2)
    #clahe = cv2.createCLAHE(clipLimit=4.0, tileGridSize=(11,11))
    #imgray2 = clahe.apply(imgray2)

    #self.mask.publish(self.bridge.cv2_to_imgmsg(imgray2, "8UC1")) 

    #cdf = hist.cumsum()
    #cdf_normalized = cdf * hist.max()/ cdf.max()

    
#    ret,thresh = cv2.threshold(imgray,230,255,cv2.THRESH_TOZERO)

    #ret,thresh = cv2.threshold(imgray,200,255,cv2.THRESH_TOZERO) #Adjust the first digit after imgray to increase sensitivity to white boards.  Default 230.

    #ret, thresh = cv2.threshold(imgray,127,255,0)
    #ret, thresh = cv2.threshold(imgray,130,255,0)

    thresh = cv2.adaptiveThreshold(imgray,255,1,1,5,2)
    #thresh = cv2.adaptiveThreshold(imgray,255,1,1,7,2)
    
    thresh2 = thresh

    #kernel = np.ones((1,1),np.uint8)
    #thresh = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)

    mask = np.zeros(thresh.shape,np.uint8)
    mask2 = np.zeros(thresh.shape,np.uint8)


    #contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) 
    
    #kernel = np.ones((7,7),np.uint8)
    #thresh = cv2.erode(thresh,kernel,iterations = 1)

    contours, hier = cv2.findContours(thresh,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)
    for cnt in contours:
        x,y,w,h = cv2.boundingRect(cnt)
        aspect_ratio = float(w)/h

        if 200<cv2.contourArea(cnt)<50000 and aspect_ratio > .8 and aspect_ratio < 1.2:

            area = cv2.contourArea(cnt)
            x,y,w,h = cv2.boundingRect(cnt)            

            rect_area = w*h
            extent = float(area)/rect_area

            if extent >= .3:
                cv2.drawContours(mask,[cnt],0,255,-1)
                cv2.drawContours(mask2,[cnt],0,255,-1)   

    kernel = np.ones((7,7),np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
    mask2 = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)

    self.contours.publish(self.bridge.cv2_to_imgmsg(mask, "8UC1"))
   
    contours, hier = cv2.findContours(mask2,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)
    for cnt in contours:
        #print cnt
        #epsilon = 0.001*cv2.arcLength(cnt,True)
        #approx = cv2.approxPolyDP(cnt,epsilon,True)
        #cv2.drawContours(cv_image, approx, -1, (255,255,0), 3)

        #k = cv2.isContourConvex(cnt)
        #if k == True:
        #    cv2.drawContours(cv_image, contours, -1, (255,255,0), -1)


        #cv2.circle(cv_image,(rightmost),3,255,-1)
        #cv2.circle(cv_image,(topmost),3,255,-1)
        #cv2.circle(cv_image,(bottommost),3,255,-1)


        

        if cnt != None: 
            x,y,w,h = cv2.boundingRect(cnt) 
            crop = mask2[y:y+h, x:x+w]
            self.mask.publish(self.bridge.cv2_to_imgmsg(crop, "8UC1"))

            contours, hier = cv2.findContours(crop,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)
            for cnt in contours:

                area = cv2.contourArea(cnt)
                hull = cv2.convexHull(cnt)
                hull_area = cv2.contourArea(hull)

                if hull_area > 0:
                    solidity = float(area)/hull_area


                    if solidity < .67 and solidity > .5:
                        #print len(cnt)
                        
                        shift = np.array([x,y])
                        for c in contours:
                            c += shift
                        
                        cnt = contours[0]
                        M = cv2.moments(cnt)

                        if M['m00'] != 0:

                            cx = int(M['m10']/M['m00'])

                            x_pos = cx - 500
                            self.cross_detected.publish(1)
                            self.cross_x_pos.publish(x_pos)
                           
                            cv2.drawContours(cv_image, contours, -1, (0,255,0), -1)

                leftmost = tuple(cnt[cnt[:,:,0].argmin()][0])
                rightmost = tuple(cnt[cnt[:,:,0].argmax()][0])
                topmost = tuple(cnt[cnt[:,:,1].argmin()][0])
                bottommost = tuple(cnt[cnt[:,:,1].argmax()][0])

                if abs(leftmost[1]-bottommost[1]) <= 7 and abs(rightmost[1]-bottommost[1]) <= 7:
                    area_of_triangle = abs((leftmost[0]*(topmost[1]-rightmost[1]) + topmost[0]*(rightmost[1] - leftmost[1]) + rightmost[0]*(leftmost[0]-topmost[0]))/2)

                    #print area_of_triangle
                    if area_of_triangle >= 60:
                        cv2.line(cv_image, (leftmost[0]+x, leftmost[1]+y), (topmost[0]+x, topmost[1]+y), (255,255,0), 2)
                        cv2.line(cv_image, (rightmost[0]+x, rightmost[1]+y), (topmost[0]+x, topmost[1]+y), (255,255,0), 2)
                        cv2.line(cv_image, (leftmost[0]+x, leftmost[1]+y), (rightmost[0]+x, rightmost[1]+y), (255,255,0), 2)

                        x_pos = ((leftmost[0]+x)+(rightmost[0]+x))/2 - 500
                        self.triangle_detected.publish(1)
                        self.triangle_x_pos.publish(x_pos)

                    else:
                        self.triangle_detected.publish(0)

                        #cv2.drawContours(cv_image, contours, -1, (255,255,0), -1)


    #dst = cv2.goodFeaturesToTrack(mask,25,0.2,15)

    #if dst != None:
        #print len(dst)
        #corners = np.int0(dst)

        #for i in corners:
            #x,y = i.ravel()
            #cv2.circle(cv_image,(x,y),3,255,-1)




    circles = cv2.HoughCircles(mask,cv.CV_HOUGH_GRADIENT,1, width, param1=p1,param2=p2,minRadius=nr,maxRadius=mr)            
    if circles != None:
        a, b, c = circles.shape
        for i in range(b):
            self.circle_detected.publish(1)
            x_pos = circles[0][i][0] - 500
            self.circle_x_pos.publish(x_pos)
            cv2.circle(cv_image, (circles[0][i][0], circles[0][i][1]), circles[0][i][2], (0, 255, 0), 2)
    else:
        self.circle_detected.publish(0)

    try:
      self.cv_image.publish(self.bridge.cv2_to_imgmsg(cv_image, "rgb8"))

    
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
