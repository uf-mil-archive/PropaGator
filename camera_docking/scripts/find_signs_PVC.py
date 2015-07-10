#!/usr/bin/env python

import roslib

#roslib.load_manifest('camera_docking')

import sys
import rospy
import cv2
import numpy as np
import math
import cv2.cv as cv
import colorsys
import time

from std_msgs.msg import Header
from std_msgs.msg import Bool, Int16, String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
from camera_docking.msg import Circle, Triangle, Cross, Sign

font = cv2.FONT_HERSHEY_SIMPLEX

#Hough_circle values
p1 = 100 #100 #First method-specific parameter. In case of CV_HOUGH_GRADIENT , it is the higher threshold of the two passed to the Canny() edge detector (the lower one is twice smaller). default 100
p2 = 13   #Second method-specific parameter. In case of CV_HOUGH_GRADIENT , it is the accumulator threshold for the circle centers at the detection stage. The smaller it is, the more false circles may be detected. Circles, corresponding to the larger accumulator values, will be returned first. default 15
nr = 20   #Minimum circle radius. default 20
mr = 150  #Maximum circle radius. default 150
#*******************

#blur Blank_gray images
kern1 = 7
kern2 = 7

#zeroing counters used for loops etc...
i = 0  
j = 0
corner = []

distance_from_center = 35.0 #28.0 distance used to ensure no overlapping contours are used

############  function used to determine the shape inside of the white sign. ##############
def find_shape(orig_img, blank_img, p1, p2, nr, mr, dst_frm_cnt):
    symbol_type = 'none'
    imgray = cv2.cvtColor(orig_img,cv2.COLOR_BGR2GRAY)

    kernel = np.ones((3,3),np.float32)

    height = imgray.shape[0]

    width  = imgray.shape[1]
    
    width_cent = width/2
    height_cent = height/2

    thresh = cv2.GaussianBlur(imgray,(1,1),1)

    thresh = cv2.adaptiveThreshold(imgray,255,1,1,5,2)
   
    thresh2 = thresh

 

    mask = np.zeros(thresh.shape,np.uint8)
    mask2 = np.zeros(thresh.shape,np.uint8)

    contours, hier = cv2.findContours(thresh,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)
    for cnt in contours:
        x,y,w,h = cv2.boundingRect(cnt)
        aspect_ratio = float(w)/h

        if 200<cv2.contourArea(cnt)<50000 and aspect_ratio > .6 and aspect_ratio < 1.4:

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

    contours, hier = cv2.findContours(mask2,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)

    biggest_area = 0
    biggest_cnt = [0]
    biggest_M = [0]
    biggest_approx = [0]
    cxmax = 0
    cymax = 0

    for cnt in contours:

        approx = cv2.approxPolyDP(cnt,0.1*cv2.arcLength(cnt,True),True)

        M = cv2.moments(approx)

        if M['m00'] != 0.0:

            area = cv2.contourArea(approx)	
            cx = int(M['m10']/M['m00'])
            cy = int(M['m01']/M['m00'])
            #print area
            d = math.sqrt((width_cent-cx)*(width_cent-cx) + (height_cent-cy)*(height_cent-cy))
            
            #if area >= 1000:

            #print d 

            if area >= biggest_area and d < distance_from_center:
                biggest_area = area
                biggest_cnt[0] = cnt
                cxmax = cx
                cymax = cy
                #print biggest_area
    if biggest_area != 0:
        blank_gray = cv2.cvtColor(blank_img,cv2.COLOR_BGR2GRAY)

        for cnt in contours:
            if cnt != None: 
                x,y,w,h = cv2.boundingRect(cnt) 
                crop = mask2[y:y+h, x:x+w]
                hold = crop

                contours, hier = cv2.findContours(crop,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)
                for cnt in contours:

                    area = cv2.contourArea(cnt)
                    hull = cv2.convexHull(cnt)
                    hull_area = cv2.contourArea(hull)

                    circles = cv2.HoughCircles(mask,cv.CV_HOUGH_GRADIENT,1, width, param1=p1,param2=p2,minRadius=nr,maxRadius=mr)   #p1,p2,nr,mr defined at top of program 
                    
                    if hull_area != 0:                    
                        solidity = float(area)/hull_area

                        if hull_area > 0 and solidity < .75 and solidity > .5:
                            symbol_type = 'cruciform'  ##################################  cruciform definition  #######################################

                        elif circles != None: # or solidity > .9999:
                            symbol_type = 'circle' ##################################  circle definition  #######################################

                        else:
                            thresh = cv2.adaptiveThreshold(imgray,255,1,1,5,2)  
                            blank_image = np.zeros((imgray.shape[0],imgray.shape[1],3), np.uint8) 
                            blank_image[::] = 255 
                            dst = cv2.goodFeaturesToTrack(imgray,3,.05,50) #10, .15, 10

                            if dst != None:
                                dst = np.int0(dst)

                                for i in dst:
                                    x,y = i.ravel()
                                    cv2.circle(blank_image,(x,y),4,0,-1)                                    

                                #cv2.imshow('thresh', blank_image)
                                #cv2.waitKey(1) 

                                if len(dst) == 3:
                                    symbol_type = 'triangle'
                                    #print 'triangle'
                                else:
                                    #print len(dst)
                                    '''else:
                                        blank_img[0:height,0:width] = 255
                                        ret,thresh = cv2.threshold(imgray_triangle,230,255,cv2.THRESH_BINARY_INV+cv2.THRESH_OTSU)
                                                        
                                        contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

                                        biggest_area = 0
                                        biggest_cnt = [0]
                                        biggest_M = [0]
                                        biggest_approx = [0]
                                        cxmax = 0
                                        cymax = 0



                                        for cnt in contours:
                                            approx = cv2.approxPolyDP(cnt,0.1*cv2.arcLength(cnt,True),True)

                                            M = cv2.moments(approx)

                                            if M['m00'] != 0.0:
                                                area = cv2.contourArea(approx)  

                                                #print area

                                                cx = int(M['m10']/M['m00'])
                                                cy = int(M['m01']/M['m00'])

                                                d = math.sqrt((width_cent-cx)*(width_cent-cx) + (height_cent-cy)*(height_cent-cy))

                                                #if area >= 600:

                                                    #print d                        

                                                if area >= biggest_area and d < distance_from_center:
                                                    biggest_area = area
                                                    biggest_cnt[0] = cnt
                                                    cxmax = cx
                                                    cymax = cy
                                        if biggest_area != 0:

                                            cv2.drawContours(blank_img,[biggest_cnt[0]],0,(0,0,255),1)

                                            blank_gray = cv2.cvtColor(blank_img,cv2.COLOR_BGR2GRAY)

                                            dst = cv2.goodFeaturesToTrack(blank_gray,25,0.15,10)
                                            dst = np.int0(dst)
                                        


                                            for i in dst:
                                                x,y = i.ravel()
                                                cv2.circle(blank_gray,(x,y),4,0,-1)

                                            if len(dst) == 3:
                                                symbol_type = 'triangle'
                                            else:
                                                symbol_type = 'circle'
                                        else:
                                            symbol_type = 'none'
                                            blank_gray = cv2.cvtColor(blank_img,cv2.COLOR_BGR2GRAY)'''                                        
    
                            '''leftmost = tuple(cnt[cnt[:,:,0].argmin()][0])
                            rightmost = tuple(cnt[cnt[:,:,0].argmax()][0])
                            topmost = tuple(cnt[cnt[:,:,1].argmin()][0])
                            bottommost = tuple(cnt[cnt[:,:,1].argmax()][0])'''
                            '''print "left to bottom"
                            print abs(leftmost[1]-bottommost[1]) 
                            print abs(rightmost[1]-bottommost[1])
                            print "right to bottom"
                            print "***************************"'''

                            '''if abs(leftmost[1]-bottommost[1]) <= 23 and abs(rightmost[1]-bottommost[1]) <= 23:
                                area_of_triangle = abs((leftmost[0]*(topmost[1]-rightmost[1]) + topmost[0]*(rightmost[1] - leftmost[1]) + rightmost[0]*(leftmost[0]-topmost[0]))/2)

                                #print area_of_triangle
                                if area_of_triangle >= 80:# and solidity < .99:
                                    symbol_type = 'triangle'  ##################################  triangle definition  #######################################

                                    #print solidity'''
                    else:
                        print 'hull'

    else:
        symbol_type = 'none'
        

    if symbol_type != 'none':

        for h,cnt in enumerate(biggest_cnt[0]):
            mask = np.zeros(imgray.shape,np.uint8)
            cv2.drawContours(mask,[biggest_cnt[0]],0,255,-1)
            mean = cv2.mean(orig_img,mask = mask)

        mean = colorsys.rgb_to_hsv(mean[2]/255.0, mean[1]/255.0, mean[0]/255.0) #color finding portion, converts HSV to 0.0-1.0 scale
        hsv = list(mean)
        hsv[0] = hsv[0]*360 #Hue to 360 values

        if hsv[2] < 0.1 or (hsv[1] < .29 and hsv[2] > .1 and hsv[2] < .65):
            color = 'black' 
        elif (hsv[0]<11 or hsv[0]>324) and hsv[1]>.43 and hsv[2]>.1:
            color = 'red'
        elif (hsv[0]>64 and hsv[0]<172) and hsv[1]>.10 and hsv[2]>.1:
            color = 'green'
        elif (hsv[0]>180 and hsv[0]<255) and hsv[1]>.15 and hsv[2]>.1:
            color = 'blue'
        else:     
            color = 'can\'t find'

    if symbol_type == 'none':
        blank_gray = cv2.cvtColor(blank_img,cv2.COLOR_BGR2GRAY)
        cxmax = width_cent
        cymax = height_cent

        mask = np.zeros(imgray.shape, np.uint8)
        cv2.circle(mask,(cxmax,cymax), 5, 255,-1)
        mean = cv2.mean(orig_img,mask = mask)

        mean = colorsys.rgb_to_hsv(mean[2]/255, mean[1]/255, mean[0]/255)
        hsv = list(mean)
        hsv[0] = hsv[0]*3603

        if hsv[2] < 0.1 or (hsv[1] < .37 and hsv[2] > .1 and hsv[2] < .65):
            color = 'black' 
        elif (hsv[0]<11 or hsv[0]>324) and hsv[1]>.43 and hsv[2]>.1:
            color = 'red'
        elif (hsv[0]>64 and hsv[0]<164) and hsv[1]>.10 and hsv[2]>.1:
            color = 'green'
        elif (hsv[0]>180 and hsv[0]<255) and hsv[1]>.15 and hsv[2]>.1:
            color = 'blue'
        else:     
            color = 'can\'t find'
   
    return (symbol_type, blank_gray, cxmax, cymax, color, hsv[0], hsv[1], hsv[2])
    
    #symbol_type: 'circle' 'cruciform' 'triangle' 'none'
    #blank_gray: Cropped image used to transfer color image to find_shape function. Doesn't look like it's used 
    #cxmax, cymax: Center of mass, in x and y, for the detected shape
    #color: 'black' 'red' 'green' 'blue'
    #hsv[0,1,2]: HSV values for determining unknown colors

#Adaptive histogram for colored images.  Not currently used.  Was hoping it could even out colors during bad image bags.  Made image worse.
def adaptive_histogram_eq(img):
    b,g,r = cv2.split(img)
    clahe = cv2.createCLAHE(clipLimit=.1, tileGridSize=(8,8))
    b = clahe.apply(b)
    g = clahe.apply(g)
    r = clahe.apply(r)
    return cv2.merge([b,g,r])

class image_converter:

  def __init__(self):
    self.image_pub = rospy.Publisher("docking_camera_out",Image, queue_size = 1)
    self.image_white = rospy.Publisher("finding_white",Image, queue_size = 1)

    #Messages below publish x-pos and color for each recognized shape
    self.circle = rospy.Publisher('Circle_Sign', Circle, queue_size = 1)
    self.triangle = rospy.Publisher('Triangle_Sign', Triangle, queue_size = 1)
    self.cross = rospy.Publisher('Cross_Sign', Cross, queue_size = 1)
    self.signx = rospy.Publisher('Sign', Sign, queue_size = 1)



    self.bridge = CvBridge()
    self.image_sub = rospy.Subscriber("/camera/image_raw",Image,self.callback)

  def callback(self,data):
   
    try:
      vid = self.bridge.imgmsg_to_cv2(data, "bgr8")
    except CvBridgeError, e:
      print e    

    crcl = Circle()
    trgl = Triangle()
    crss = Cross()
    sgn = Sign()
    
#**************** while(cap.isOpened()): *************************************************************
    counter = 0
    sign1_sum = []
    sign2_sum = []
    sign3_sum = []

    #frame = vid

    #Rotation of video
    rows1 = vid.shape[0]
    cols1 = vid.shape[1]
    #M = cv2.getRotationMatrix2D((cols1/2,rows1/2),180,1)
    #vid = cv2.warpAffine(vid, M, (cols1,rows1))

    frame_real = vid
    frame = vid
   
    height = frame_real.shape[0]
    width  = frame_real.shape[1]

    imgray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    imgray = cv2.GaussianBlur(imgray,(1,1),1)
    ret,thresh = cv2.threshold(imgray,145,255,cv2.THRESH_TOZERO) #Adjust the first digit after imgray to increase sensitivity to white boards. Used 230 for UF pool testing. 

    kernel = np.ones((7,7),np.uint8)
    thresh = cv2.erode(thresh,kernel,iterations = 1)
    
    thresh = cv2.dilate(thresh,kernel,iterations = 1)


    thresh = cv2.adaptiveThreshold(thresh,255,1,1,5,2)

    #cv2.imshow('thresh', thresh)
    #cv2.waitKey(1)


    #cv2.rectangle(thresh,(0,300),(1240,1080),(0,0,0),-1)  #Empty blinding rectangle.  Ensures that no false positives will come from the water.

    protect = thresh

    contours, hierarchy = cv2.findContours(protect, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)   
    
    i = 0

    sign = [0,0,0,0,0,0]
    cnt_hold = [0,0,0,0,0,0]

    while i < len(contours): #while loop to find the largest contour in the image (white bordered images).  Finds the biggest six.
        
        cnt = contours[i]

        M = cv2.moments(cnt)
        area = cv2.contourArea(cnt)

        if area >= sign[0]:
            sign[5] = sign[4]
            cnt_hold[5] = cnt_hold[4]
            sign[4] = sign[3]
            cnt_hold[4] = cnt_hold[3]
            sign[3] = sign[2]
            cnt_hold[3] = cnt_hold[2]
            sign[2] = sign[1]
            cnt_hold[2] = cnt_hold[1]
            sign[1] = sign[0]
            cnt_hold[1] = cnt_hold[0]
            sign[0] = area
            cnt_hold[0] = cnt
        elif area >= sign[1]:
            sign[5] = sign[4]
            cnt_hold[5] = cnt_hold[4]
            sign[4] = sign[3]
            cnt_hold[4] = cnt_hold[3]
            sign[3] = sign[2]
            cnt_hold[3] = cnt_hold[2]
            sign[2] = sign[1]
            cnt_hold[2] = cnt_hold[1]
            sign[1] = area
            cnt_hold[1] = cnt
        elif area >= sign[2]:
            sign[5] = sign[4]
            cnt_hold[5] = cnt_hold[4]
            sign[4] = sign[3]
            cnt_hold[4] = cnt_hold[3]
            sign[3] = sign[2]
            cnt_hold[3] = cnt_hold[2]
            sign[2] = area
            cnt_hold[2] = cnt
        elif area >= sign[3]:
            sign[5] = sign[4]
            cnt_hold[5] = cnt_hold[4]
            sign[4] = sign[3]
            cnt_hold[4] = cnt_hold[3]
            sign[3] = area
            cnt_hold[3] = cnt
        elif area >= sign[4]:
            sign[5] = sign[4]
            cnt_hold[5] = cnt_hold[4]
            sign[4] = area
            cnt_hold[4] = cnt
        elif area >= sign[5]:
            sign[5] = area
            cnt_hold[5] = cnt
	
        i += 1

    i = 0
    corner = []    
    no_of_signs = 0
    avg_area = np.mean(sign)

    no_of_signs = 0
    coordsx = []
    coordsy = []
    z = 0
    j = 0

    for i in xrange(6):

        #check if centroids of moments are close to each other
        #************************************************************************************
        same_moment = 0
        M = cv2.moments(cnt_hold[i])

        if (M['m00'] != 0.0):
            cx = int(M['m10']/M['m00'])
            cy = int(M['m01']/M['m00'])
        else:
            cx = 0
            cy = 0

        d = []

        numbers = len(coordsx)

        for z in xrange(numbers):
            d.append(int(math.sqrt((coordsx[z-1]-cx)*(coordsx[z-1]-cx) + (coordsy[z-1]-cy)*(coordsy[z-1]-cy))))
        
        for j in d:
            if j < 60:
                same_moment += 1
        #************************************************************************************
        if (M['m00'] != 0.0):  
            epsilon = 0.1*cv2.arcLength(cnt_hold[i],True)
            approx = cv2.approxPolyDP(cnt_hold[i],epsilon,True)
            area = cv2.contourArea(approx)
            #print area
            x,y,w,h = cv2.boundingRect(approx)
            
            if y < (height/2 + 150) and y > (height/2 - 225) and same_moment == 0 and area >= 5000 and area <= 100000.0:  #1400 #y values viewing area, greater values = more search area
            #if same_moment == 0 and area >= 3000.0 and area <= 70000.0:  #y values viewing area, greater values = more search area
                coordsx.append(cx) #append for centroid distance checking
                coordsy.append(cy) #append for centroid distance checking       
                x = x - 12
                y = y - 12
                
                if x < 0:
                    x = 0
                if y < 0:
                    y = 0
    
                w = w + 24
                h = h + 24
                corner.append(x)
                corner.append(y)
                corner.append(w)
                corner.append(h)

                sgn.xpixel = (x + (w / 2)) - 500

                sgn.header = Header(
                        stamp = rospy.get_rostime(),
                        frame_id = '/camera'
                    )
                self.signx.publish(sgn)

                #cv2.rectangle(frame_real,(x,y),(x+w,y+h),(0,255,0),2)
                cv2.circle(frame_real,(x,y), 2, (128,0,0),-1)
                no_of_signs += 1
    
    #if there are no signs or less than three signs visible
    if no_of_signs == 2:
            x,y,w,h = width,height,0,0
            corner.append(x-4)
            corner.append(y-4)
            corner.append(w+4)
            corner.append(h+4)
    elif no_of_signs == 1:
            x,y,w,h = width,height,0,0
            corner.append(x-4)
            corner.append(y-4)
            corner.append(w+4)
            corner.append(h+4)
            x,y,w,h = width,height,0,0
            corner.append(x-4)
            corner.append(y-4)
            corner.append(w+4)
            corner.append(h+4)
    elif no_of_signs == 0:
            x,y,w,h = width,height,0,0
            corner.append(x-4)
            corner.append(y-4)
            corner.append(w+4)
            corner.append(h+4)
            x,y,w,h = width,height,0,0
            corner.append(x-4)
            corner.append(y-4)
            corner.append(w+4)
            corner.append(h+4)
            x,y,w,h = width,height,0,0
            corner.append(x-4)
            corner.append(y-4)
            corner.append(w+4)
            corner.append(h+4)


#########################################################  Begin Display and Message update  ####################################################################    
#################  The nine sections below apply text to the opencv image displays and update the x-pos and color messages for each shape  ######################
#################################################################################################################################################################

    if (corner[0] < corner[4] and corner[0] < corner[8] and corner[0] != width):
        one_a = frame[corner[1]:corner[1]+corner[3], corner[0]:corner[0]+corner[2]]
        blank_one = np.zeros((corner[3],corner[2],3), np.uint8)
        blank_one[:,0:corner[2]] = (255,255,255)  
        sign1 = find_shape(one_a, blank_one, p1, p2, nr, mr, distance_from_center)  

        if sign1[0] != 'none':        
            cv2.circle(frame_real,(sign1[2]+corner[0],sign1[3]+corner[1]), 2, (255,0,0),-1) 

            if sign1[4] == 'can\'t find':
                cv2.putText(frame_real, "%s (%d, %1.2f, %1.2f)" % (sign1[4], sign1[5], sign1[6], sign1[7]) , (sign1[2]+corner[0], sign1[3]+corner[1] - 15), font, .6,(255,0,255),1, cv2.CV_AA)
                cv2.putText(frame_real, "%s" % sign1[0], (sign1[2]+corner[0], sign1[3]+corner[1] + 15), font, .6,(255,0,255),1, cv2.CV_AA)
            elif sign1[4] != 'red':            
                cv2.putText(frame_real, sign1[4] , (sign1[2]+corner[0], sign1[3]+corner[1] - 15), font, .6,(0,0,255),1, cv2.CV_AA)
                cv2.putText(frame_real, "%s" % sign1[0], (sign1[2]+corner[0], sign1[3]+corner[1] + 15), font, .6,(0,0,255),1, cv2.CV_AA)
            else:
                cv2.putText(frame_real, sign1[4] , (sign1[2]+corner[0], sign1[3]+corner[1] - 15), font, .6,(255,0,0),1, cv2.CV_AA)      
                cv2.putText(frame_real, "%s" % sign1[0], (sign1[2]+corner[0], sign1[3]+corner[1] + 15), font, .6,(255,0,0),1, cv2.CV_AA)

            if sign1[0] == 'circle':
                crcl.xpixel = sign1[2]+corner[0] - 500
                crcl.color = sign1[4]
            elif sign1[0] == 'triangle':
                trgl.xpixel = sign1[2]+corner[0] - 500
                trgl.color = sign1[4]
            elif sign1[0] == 'cruciform':
                crss.xpixel = sign1[2]+corner[0] - 500
                crss.color = sign1[4]

        cv2.putText(frame_real, "Sign1" , (corner[0], corner[1] - 5), font, .75,(255,0,255),2, cv2.CV_AA)
    elif (corner[0] > corner[4] and corner[0] > corner[8] and corner[0] != width):
        three_a = frame[corner[1]:corner[1]+corner[3], corner[0]:corner[0]+corner[2]]
        blank_three = np.zeros((corner[3],corner[2],3), np.uint8)
        blank_three[:,0:corner[2]] = (255,255,255)
        sign3 = find_shape(three_a, blank_three, p1, p2, nr, mr, distance_from_center)  

        if sign3[0] != 'none':        
            cv2.circle(frame_real,(sign3[2]+corner[0],sign3[3]+corner[1]), 2, (255,0,0),-1)

            if sign3[4] == 'can\'t find':
                cv2.putText(frame_real, "%s (%d, %1.2f, %1.2f)" % (sign3[0], sign3[5], sign3[6], sign3[7]), (sign3[2]+corner[0], sign3[3]+corner[1] + 15), font, .6,(255,0,255),1, cv2.CV_AA)
                cv2.putText(frame_real, sign3[4] , (sign3[2]+corner[0], sign3[3]+corner[1] - 15), font, .6,(255,0,255),1, cv2.CV_AA)
            elif sign3[4] != 'red':
                cv2.putText(frame_real, "%s" % sign3[0], (sign3[2]+corner[0], sign3[3]+corner[1] + 15), font, .6,(0,0,255),1, cv2.CV_AA)
                cv2.putText(frame_real, sign3[4] , (sign3[2]+corner[0], sign3[3]+corner[1] - 15), font, .6,(0,0,255),1, cv2.CV_AA)
            else:
                cv2.putText(frame_real, "%s" % sign3[0], (sign3[2]+corner[0], sign3[3]+corner[1] + 15), font, .6,(255,0,0),1, cv2.CV_AA)
                cv2.putText(frame_real, sign3[4] , (sign3[2]+corner[0], sign3[3]+corner[1] - 15), font, .6,(255,0,0),1, cv2.CV_AA)                
          
            if sign3[0] == 'circle':
                crcl.xpixel = sign3[2]+corner[0] - 500
                crcl.color = sign3[4]
            elif sign3[0] == 'triangle':
                trgl.xpixel = sign3[2]+corner[0] - 500
                trgl.color = sign3[4]
            elif sign3[0] == 'cruciform':
                crss.xpixel = sign3[2]+corner[0] - 500
                crss.color = sign3[4]

        cv2.putText(frame_real, "Sign3" , (corner[0], corner[1] - 5), font, .75,(255,0,255),2, cv2.CV_AA)
    elif (corner[0] != width):
        two_a = frame[corner[1]:corner[1]+corner[3], corner[0]:corner[0]+corner[2]]
        blank_two = np.zeros((corner[3],corner[2],3), np.uint8)
        blank_two[:,0:corner[2]] = (255,255,255) 
        sign2 =  find_shape(two_a, blank_two, p1, p2, nr, mr, distance_from_center)

        if sign2[0] != 'none':        
            cv2.circle(frame_real,(sign2[2]+corner[0],sign2[3]+corner[1]), 2, (255,0,0),-1) 
            if sign2[4] == 'can\'t find':
                cv2.putText(frame_real, "%s (%d, %1.2f, %1.2f)" % (sign2[0], sign2[5], sign2[6], sign2[7]), (sign2[2]+corner[0], sign2[3]+corner[1] + 15), font, .6,(255,0,255),1, cv2.CV_AA)
                cv2.putText(frame_real, sign2[4] , (sign2[2]+corner[0], sign2[3]+corner[1] - 15), font, .6,(255,0,255),1, cv2.CV_AA)
            elif sign2[4] != 'red':
                cv2.putText(frame_real, "%s" % sign2[0] , (sign2[2]+corner[0], sign2[3]+corner[1] + 15), font, .6,(0,0,255),1, cv2.CV_AA)
                cv2.putText(frame_real, sign2[4] , (sign2[2]+corner[0], sign2[3]+corner[1] - 15), font, .6,(0,0,255),1, cv2.CV_AA)
            else:
                cv2.putText(frame_real, "%s" % sign2[0] , (sign2[2]+corner[0], sign2[3]+corner[1] + 15), font, .6,(255,0,0),1, cv2.CV_AA)
                cv2.putText(frame_real, sign2[4] , (sign2[2]+corner[0], sign2[3]+corner[1] - 15), font, .6,(255,0,0),1, cv2.CV_AA)            

            if sign2[0] == 'circle':
                crcl.xpixel = sign2[2]+corner[0] - 500
                crcl.color = sign2[4]
            elif sign2[0] == 'triangle':
                trgl.xpixel = sign2[2]+corner[0] - 500
                trgl.color = sign2[4]
            elif sign2[0] == 'cruciform':
                crss.xpixel = sign2[2]+corner[0] - 500
                crss.color = sign2[4]

        cv2.putText(frame_real, "Sign2" , (corner[0], corner[1] - 5), font, .75,(255,0,255),2, cv2.CV_AA)
    if (corner[4] < corner[0] and corner[4] < corner [8] and corner[4] != width and corner[4] != width):
        one_a = frame[corner[5]:corner[5]+corner[7], corner[4]:corner[4]+corner[6]]
        blank_one = np.zeros((corner[7],corner[6],3), np.uint8)
        blank_one[:,0:corner[6]] = (255,255,255)
        sign1 =  find_shape(one_a, blank_one, p1, p2, nr, mr, distance_from_center) 

        if sign1[0] != 'none':        
            cv2.circle(frame_real,(sign1[2]+corner[4],sign1[3]+corner[5]), 2, (255,0,0),-1) 

            if sign1[4] == 'can\'t find':
                cv2.putText(frame_real, "%s (%d, %1.2f, %1.2f)" % (sign1[4], sign1[5], sign1[6], sign1[7]) , (sign1[2]+corner[4], sign1[3]+corner[5] + 15), font, .6,(255,0,255),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign1[4] , (sign1[2]+corner[4], sign1[3]+corner[5] - 15), font, .6,(255,0,255),1, cv2.CV_AA)
            elif sign1[4] != 'red':
                cv2.putText(frame_real, "%s" % sign1[0] , (sign1[2]+corner[4], sign1[3]+corner[5] + 15), font, .6,(0,0,255),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign1[4] , (sign1[2]+corner[4], sign1[3]+corner[5] - 15), font, .6,(0,0,255),1, cv2.CV_AA)
            else:
                cv2.putText(frame_real, "%s" % sign1[0] , (sign1[2]+corner[4], sign1[3]+corner[5] + 15), font, .6,(255,0,0),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign1[4] , (sign1[2]+corner[4], sign1[3]+corner[5] - 15), font, .6,(255,0,0),1, cv2.CV_AA)

            if sign1[0] == 'circle':
                crcl.xpixel = sign1[2]+corner[4] - 500
                crcl.color = sign1[4]
            elif sign1[0] == 'triangle':
                trgl.xpixel = sign1[2]+corner[4] - 500
                trgl.color = sign1[4]
            elif sign1[0] == 'cruciform':
                crss.xpixel = sign1[2]+corner[4] - 500
                crss.color = sign1[4]

        cv2.putText(frame_real, "Sign1" , (corner[4], corner[5] - 5), font, .75,(255,0,255),2, cv2.CV_AA)
    elif (corner[4] > corner[0] and corner[4] > corner[8] and corner[4] != width):
        three_a = frame[corner[5]:corner[5]+corner[7], corner[4]:corner[4]+corner[6]]
        blank_three = np.zeros((corner[7],corner[6],3), np.uint8)
        blank_three[:,0:corner[6]] = (255,255,255)
        sign3 =  find_shape(three_a, blank_three, p1, p2, nr, mr, distance_from_center) 

        if sign3[0] != 'none':        
            cv2.circle(frame_real,(sign3[2]+corner[4],sign3[3]+corner[5]), 2, (255,0,0),-1) 

            if sign3[4] == 'can\'t find':
                cv2.putText(frame_real, "%s (%d, %1.2f, %1.2f)" % (sign3[0], sign3[5], sign3[6], sign3[7]) , (sign3[2]+corner[4], sign3[3]+corner[5] + 15), font, .6,(255,0,255),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign3[4] , (sign3[2]+corner[4], sign3[3]+corner[5] - 15), font, .6,(255,0,255),1, cv2.CV_AA) 
            elif sign3[4] != 'red':
                cv2.putText(frame_real, "%s" % sign3[0] , (sign3[2]+corner[4], sign3[3]+corner[5] + 15), font, .6,(0,0,255),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign3[4] , (sign3[2]+corner[4], sign3[3]+corner[5] - 15), font, .6,(0,0,255),1, cv2.CV_AA)     
            else:
                cv2.putText(frame_real, "%s" % sign3[0] , (sign3[2]+corner[4], sign3[3]+corner[5] + 15), font, .6,(255,0,0),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign3[4] , (sign3[2]+corner[4], sign3[3]+corner[5] - 15), font, .6,(255,0,0),1, cv2.CV_AA)

            if sign3[0] == 'circle':
                crcl.xpixel = sign3[2]+corner[4] - 500
                crcl.color = sign3[4]
            elif sign3[0] == 'triangle':
                trgl.xpixel = sign3[2]+corner[4] - 500
                trgl.color = sign3[4]
            elif sign3[0] == 'cruciform':
                crss.xpixel = sign3[2]+corner[4] - 500
                crss.color = sign3[4]

        cv2.putText(frame_real, "Sign3" , (corner[4], corner[5] - 5), font, .75,(255,0,255),2, cv2.CV_AA)
    elif (corner[4] != width):
        two_a = frame[corner[5]:corner[5]+corner[7], corner[4]:corner[4]+corner[6]]
        blank_two = np.zeros((corner[7],corner[6],3), np.uint8)
        blank_two[:,0:corner[6]] = (255,255,255)    
        sign2 =  find_shape(two_a, blank_two, p1, p2, nr, mr, distance_from_center)    

        if sign2[0] != 'none':        
            cv2.circle(frame_real,(sign2[2]+corner[4],sign2[3]+corner[5]), 2, (255,0,0),-1) 
            if sign2[4] == 'can\'t find':
                cv2.putText(frame_real, "%s (%d, %1.2f, %1.2f)" % (sign2[0], sign2[5], sign2[6], sign2[7]), (sign2[2]+corner[4], sign2[3]+corner[5] + 15), font, .6,(255,0,255),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign2[4] , (sign2[2]+corner[4], sign2[3]+corner[5] - 15), font, .6,(255,0,255),1, cv2.CV_AA)
            elif sign2[4] != 'red':
                cv2.putText(frame_real, "%s" % sign2[0] , (sign2[2]+corner[4], sign2[3]+corner[5] + 15), font, .6,(0,0,255),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign2[4] , (sign2[2]+corner[4], sign2[3]+corner[5] - 15), font, .6,(0,0,255),1, cv2.CV_AA) 
            else:
                cv2.putText(frame_real, "%s" % sign2[0] , (sign2[2]+corner[4], sign2[3]+corner[5] + 15), font, .6,(255,0,0),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign2[4] , (sign2[2]+corner[4], sign2[3]+corner[5] - 15), font, .6,(255,0,0),1, cv2.CV_AA) 

            if sign2[0] == 'circle':
                crcl.xpixel = sign2[2]+corner[4] - 500
                crcl.color = sign2[4]
            elif sign2[0] == 'triangle':
                trgl.xpixel = sign2[2]+corner[4] - 500
                trgl.color = sign2[4]
            elif sign2[0] == 'cruciform':
                crss.xpixel = sign2[2]+corner[4] - 500
                crss.color = sign2[4]

        cv2.putText(frame_real, "Sign2" , (corner[4], corner[5] - 5), font, .75,(255,0,255),2, cv2.CV_AA)

    if (corner[8] < corner[4] and corner[8] < corner [0] and corner[8] != width):
        one_a = frame[corner[9]:corner[9]+corner[11], corner[8]:corner[8]+corner[10]]
        blank_one = np.zeros((corner[11],corner[10],3), np.uint8)
        blank_one[:,0:corner[10]] = (255,255,255)
        sign1 =  find_shape(one_a, blank_one, p1, p2, nr, mr, distance_from_center) 

        if sign1[0] != 'none':        
            cv2.circle(frame_real,(sign1[2]+corner[8],sign1[3]+corner[9]), 2, (255,0,0),-1) 

            if sign1[4] == 'can\'t find':
                cv2.putText(frame_real, "%s (%d, %1.2f, %1.2f)" % (sign1[4], sign1[5], sign1[6], sign1[7]) , (sign1[2]+corner[8], sign1[3]+corner[9] + 15), font, .6,(255,0,255),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign1[4] , (sign1[2]+corner[8], sign1[3]+corner[9] - 15), font, .6,(255,0,255),1, cv2.CV_AA)
            elif sign1[4] != 'red':
                cv2.putText(frame_real, "%s" % sign1[0] , (sign1[2]+corner[8], sign1[3]+corner[9] + 15), font, .6,(0,0,255),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign1[4] , (sign1[2]+corner[8], sign1[3]+corner[9] - 15), font, .6,(0,0,255),1, cv2.CV_AA)
            else:
                cv2.putText(frame_real, "%s" % sign1[0] , (sign1[2]+corner[8], sign1[3]+corner[9] + 15), font, .6,(255,0,0),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign1[4] , (sign1[2]+corner[8], sign1[3]+corner[9] - 15), font, .6,(255,0,0),1, cv2.CV_AA)    

            if sign1[0] == 'circle':
                crcl.xpixel = sign1[2]+corner[8] - 500
                crcl.color = sign1[4]
            elif sign1[0] == 'triangle':
                trgl.xpixel = sign1[2]+corner[8] - 500
                trgl.color = sign1[4]
            elif sign1[0] == 'cruciform':
                crss.xpixel = sign1[2]+corner[8] - 500
                crss.color = sign1[4]

        cv2.putText(frame_real, "Sign1" , (corner[8], corner[9] - 5), font, .75,(255,0,255),2, cv2.CV_AA)
    elif (corner[8] > corner[0] and corner[8] > corner[4] and corner[8] != width):
        three_a = frame[corner[9]:corner[9]+corner[11], corner[8]:corner[8]+corner[10]]
        blank_three = np.zeros((corner[11],corner[10],3), np.uint8)
        blank_three[:,0:corner[10]] = (255,255,255)
        sign3 =  find_shape(three_a, blank_three, p1, p2, nr, mr, distance_from_center)

        if sign3[0] != 'none':        
            cv2.circle(frame_real,(sign3[2]+corner[8],sign3[3]+corner[9]), 2, (255,0,0),-1) 

            if sign3[4] == 'can\'t find':
                cv2.putText(frame_real, "%s (%d, %1.2f, %1.2f)" % (sign3[0], sign3[5], sign3[6], sign3[7]) , (sign3[2]+corner[8], sign3[3]+corner[9] + 15), font, .6,(255,0,255),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign3[4] , (sign3[2]+corner[8], sign3[3]+corner[9] - 15), font, .6,(255,0,255),1, cv2.CV_AA)    
            elif sign3[4] != 'red':
                cv2.putText(frame_real, "%s" % sign3[0] , (sign3[2]+corner[8], sign3[3]+corner[9] + 15), font, .6,(0,0,255),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign3[4] , (sign3[2]+corner[8], sign3[3]+corner[9] - 15), font, .6,(0,0,255),1, cv2.CV_AA)
            else:
                cv2.putText(frame_real, "%s" % sign3[0] , (sign3[2]+corner[8], sign3[3]+corner[9] + 15), font, .6,(255,0,0),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign3[4] , (sign3[2]+corner[8], sign3[3]+corner[9] - 15), font, .6,(255,0,0),1, cv2.CV_AA)

            if sign3[0] == 'circle':
                crcl.xpixel = sign3[2]+corner[8] - 500
                crcl.color = sign3[4]
            elif sign3[0] == 'triangle':
                trgl.xpixel = sign3[2]+corner[8] - 500
                trgl.color = sign3[4]
            elif sign3[0] == 'cruciform':
                crss.xpixel = sign3[2]+corner[8] - 500
                crss.color = sign3[4]

        cv2.putText(frame_real, "Sign3" , (corner[8], corner[9] - 5), font, .75,(255,0,255),2, cv2.CV_AA)
    elif (corner[8] != width):
        two_a = frame[corner[9]:corner[9]+corner[11], corner[8]:corner[8]+corner[10]]
        blank_two = np.zeros((corner[11],corner[10],3), np.uint8)
        blank_two[:,0:corner[10]] = (255,255,255)
        sign2 =  find_shape(two_a, blank_two, p1, p2, nr, mr, distance_from_center)

        if sign2[0] != 'none':        
            cv2.circle(frame_real,(sign2[2]+corner[8],sign2[3]+corner[9]), 2, (255,0,0),-1) 
            if sign2[4] == 'can\'t find':
                cv2.putText(frame_real, "%s (%d, %1.2f, %1.2f)" % (sign2[0], sign2[5], sign2[6], sign2[7]), (sign2[2]+corner[8], sign2[3]+corner[9] + 15), font, .6,(255,0,255),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign2[4] , (sign2[2]+corner[8], sign2[3]+corner[9] - 15), font, .6,(255,0,255),1, cv2.CV_AA)            
            elif sign2[4] != 'red':
                cv2.putText(frame_real, "%s" % sign2[0], (sign2[2]+corner[8], sign2[3]+corner[9] + 15), font, .6,(0,0,255),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign2[4] , (sign2[2]+corner[8], sign2[3]+corner[9] - 15), font, .6,(0,0,255),1, cv2.CV_AA)
            else:
                cv2.putText(frame_real, "%s" % sign2[0], (sign2[2]+corner[8], sign2[3]+corner[9] + 15), font, .6,(255,0,0),1, cv2.CV_AA) 
                cv2.putText(frame_real, sign2[4] , (sign2[2]+corner[8], sign2[3]+corner[9] - 15), font, .6,(255,0,0),1, cv2.CV_AA)

            if sign2[0] == 'circle':
                crcl.xpixel = sign2[2]+corner[8] - 500
                crcl.color = sign2[4]
            elif sign2[0] == 'triangle':
                trgl.xpixel = sign2[2]+corner[8] - 500
                trgl.color = sign2[4]
            elif sign2[0] == 'cruciform':
                crss.xpixel = sign2[2]+corner[8] - 500
                crss.color = sign2[4]

        cv2.putText(frame_real, "Sign2" , (corner[8], corner[9] - 5), font, .75,(255,0,255),2, cv2.CV_AA)    

#########################################################  End Display and Message update  ####################################################################

# for message headers     
    crcl.header = Header(
            stamp = rospy.get_rostime(),
            frame_id = '/camera'
        )
    trgl.header = Header(
            stamp = rospy.get_rostime(),
            frame_id = '/camera'
        )
    crss.header = Header(
            stamp = rospy.get_rostime(),
            frame_id = '/camera'
        )
    
    self.circle.publish(crcl)
    self.triangle.publish(trgl)
    self.cross.publish(crss)

#*******************************************************************************************************    
    
    try:
      self.image_pub.publish(self.bridge.cv2_to_imgmsg(frame_real, "bgr8"))        
    
      #cv2.imshow('frame_real', frame_real)
      #cv2.waitKey(1)

      #cv2.moveWindow('frame_real', 20, 20)
      #cv2.moveWindow('thresh', 20, 550)

      #cv2.moveWindow('window', 1200, 500)

      #self.image_white.publish(self.bridge.cv2_to_imgmsg(thresh, "8UC1"))
    except CvBridgeError, e:
      print e

def main(args):
  rospy.init_node('docking_camera')
  ic = image_converter()

  try:
    rospy.spin()
  except KeyboardInterrupt:
    print "Shutting down"
  cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)