#!/usr/bin/env python

import sys
import argparse
import numpy as np

import cv2
import rospy
import roslib
from sensor_msgs.msg import Image
from vision_sandbox.msg import Buoy, Buoys
from cv_bridge import CvBridge, CvBridgeError

roslib.load_manifest('vision_sandbox')

'''
    PROPAGATOR BUOY DETECTOR
        This script implements a 22-stage cascade classifier used to
        detect buoys. The training data is available, and consists of
        approx. 1.2k positive images, and +2k negative images. LBP features
        were detected, and ADA-boost weak learners were used for the cascade.

        This code will serve as a skeleton that surrounds the classifier. Color
        identification and other things will be handled from here.

        *-* Usage Information *-*
            For the sake of flexibility this code will allow the user to pass it a
            file path for any openCV generated cascade classifier. As previously
            mentioned, a pre-trained classifier is provided for your use.

        *-* Buoy Publisher Info *-*
            Buoy Message will contain the following information:
                - Color of the buoy
                - x and y location of the centroid of the largest buoy currently
                  visible in the FOV.
                - Size of the ROI containing the buoy
'''

# User-parsing for classifier xml file (This will be handled by ROSlaunch)
parser = argparse.ArgumentParser()
parser.add_argument('-c', '--classifier', help='Input XML classifier file location', required=False)
args = parser.parse_args()

# Imports the pre-trained cascade classifier from path & creates classifer
# cascade_path = os.path.join(os.path.expanduser('~'), 'Documents', 'Development', 'Classifiers', 'cascade.xml')
#cascade_path = args.classifier
cascade_path = 'Documents/Development/Classifiers/cascade.xml'
buoy_cascade = cv2.CascadeClassifier(cascade_path)

buoys_in_fov = Buoys()

def convert_bgr2greyscale(cv_image):
    return cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)

def convert_bgr2hsv(cv_image):
    return cv2.cvtColor(cv_image, cv2.COLOR_BGR2HSV)

class image_converter:
    # Object that is used to convert images from the ROS format to the OpenCV format
    def __init__(self):
        self.bridge = CvBridge()
        #self.image_pub = rospy.Publisher("/camera/cv2_cascade_out", Image)
        self.image_sub = rospy.Subscriber("/camera/image_raw", Image, self.callback)
        self.buoy_pub = rospy.Publisher("/buoy_info", Buoys)

    def callback(self,data):
        try:
            cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
        except CvBridgeError, e:
            print e

        final_img = image_process(cv_image)

        try:
            #self.image_pub.publish(self.bridge.cv2_to_imgmsg(final_img, "bgr8"))
            self.buoy_pub.publish(buoys_in_fov)
        except CvBridgeError, e:
            print e

class color_tools():
    def __init__(self):
        # These values correspond to the hue portion of the HSV color-space
        self.red_min = np.array([165], np.uint8)
        self.red_max = np.array([180], np.uint8)

        self.orange_min = np.array([15], np.uint8)
        self.orange_max = np.array([19], np.uint8)

        self.yellow_min = np.array([20], np.uint8)
        self.yellow_max = np.array([39], np.uint8)

        self.green_min = np.array([40], np.uint8)
        #self.green_max = np.array([70], np.uint8)
        self.green_max = np.array([64], np.uint8)

        #self.cyan_min = np.array([71], np.uint8)
        self.cyan_min = np.array([65], np.uint8)
        self.cyan_max = np.array([94], np.uint8)

        self.blue_min = np.array([95], np.uint8)
        self.blue_max = np.array([127], np.uint8)

        self.violet_min = np.array([128], np.uint8)
        self.violet_max = np.array([142], np.uint8)

        self.magenta_min = np.array([143], np.uint8)
        self.magenta_max = np.array([164], np.uint8)

        # This value corresponds to the value portion of the HSV color-space.
        self.white_min = np.array([0],np.uint8)
        self.white_max = np.array([40],np.uint8)

        self.black_min = np.array([200],np.uint8)
        self.black_max = np.array([255],np.uint8)

    def in_range(self, input, low, high):
        # Stupid but necessary function...
        return input >= low and input <= high

    def fetch_color(self, hue_val=None, value_val=None):
        # We simply pass it one of the other value. Or we
        # could simply send over all of the values and work from there
        print 'Hue Value: ', hue_val
        print 'Value Value: ', value_val

        if self.in_range([hue_val],self.red_min, self.red_max):
            return 'Red'
        elif self.in_range([hue_val],self.orange_min,self.orange_max):
            # This is actually orange, but we will simply output red
            return 'Red'
        elif self.in_range([hue_val],self.yellow_min,self.yellow_max):
            return 'Yellow'
        elif self.in_range([hue_val],self.green_min,self.green_max):
            return 'Green'
        elif self.in_range([hue_val],self.cyan_min,self.cyan_max):
            # This is actually cyan, but we will assume this is white
            return 'White'
        elif self.in_range([hue_val],self.blue_min,self.blue_max):
            return 'Blue'
        elif self.in_range([hue_val],self.violet_min,self.violet_max):
            # This might very well end up corresponding to a black buoy
            return 'Black'
        elif self.in_range([hue_val],self.magenta_min,self.magenta_max):
            return 'Magenta'
        else:
            return 'Blue'

def gen_circle_mask(roi,x,y):
    # Generates a circular mask centered in the already pre-selected ROI chosen by
    # the cascade classifier. This is used to better approximate the color of the
    # 'buoy' that the classifier has selected.
    mask = np.zeros((roi.shape[1],roi.shape[0]), np.uint8)
    cv2.circle(mask,(x,y),int(0.3*(0.5*(x+y))),(255,255,255),-1)
    return mask

# def find_circle_roi(cv_image, roi):
#     gray_roi = convert_bgr2greyscale(roi)
#     blur = cv2.bilateralFilter(gray_roi,10,50,200)
#     kernel = np.array([[1,1,1,1,1],[1,1,1,1,1],[1,1,1,1,1],[1,1,1,1,1],[1,1,1,1,1]],np.uint8)
#     eroded_roi = cv2.erode(blur, kernel, iterations = 1)
#     dilatated_roi = cv2.dilate(eroded_roi, kernel, iterations = 2)
#     circles = cv2.HoughCircles(dilatated_roi, cv2.cv.CV_HOUGH_GRADIENT,1, 20, param1=40, param2=24, minRadius=10, maxRadius=40)
#
#     if circles is None:
#         pass
#     else:
#         circles = np.uint16(np.around(circles))
#         for i in circles[0,:]:
#             cv2.circle(roi,(i[0],i[1]),i[2],(0,255,0),2)
#             # draw the center of the circle
#             cv2.circle(roi,(i[0],i[1]),2,(0,0,255),3)

def place_text(image, coords, string, color):
    # Used to place text onto the image
    cv2.putText(image, string, coords, cv2.FONT_HERSHEY_PLAIN, 1, color, 1, cv2.CV_AA)
    cv2.putText(image, str(coords), (coords[0], coords[1] + 13), cv2.FONT_HERSHEY_PLAIN, 1, color, 1, cv2.CV_AA)
    return image

def find_center(roi):
    # Finds the center of an ROI and returns it as a tuple
    cy = int(roi.shape[0]/2)
    cx = int(roi.shape[1]/2)
    return cy, cx

def find_max_contour(contours):
    # Returns the largest contour in the images (We're assuming that this is the
    # number that we're looking for)
    areas = [cv2.contourArea(c) for c in contours]
    max_index = np.argmax(areas)
    return contours[max_index]

def find_roi_contour(roi, cv_image):
    # Function is used to return the largest contour in the current
    # area of interest. We will then use this to determine the average color of the
    # detected object.
    kernel = np.ones((5,5),np.uint8)
    roi = cv2.erode(roi,kernel,iterations=2)
    roi = cv2.dilate(roi,kernel,iterations=1)
    edges = cv2.Canny(roi, 50, 150)
    contours, hierarchy = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    cv2.drawContours(roi,contours,-1,(0,255,0),2)
    return roi

def detect_color(roi):
    color_finder = color_tools()
    x,y = find_center(roi)
    circle_mask = gen_circle_mask(roi,x,y)
    roi = cv2.bitwise_and(roi, roi, mask=circle_mask)

    hsv_roi = convert_bgr2hsv(roi)
    mean = cv2.mean(hsv_roi, mask=circle_mask)
    (h,s,v) = mean[0], mean[1], mean[2]
    color = color_finder.fetch_color(int(h), int(s))
    print color
    return color

def image_process(cv_image):
    # Passes each frame through the cascade classifier and places bounding
    # rectangles on the ROIs.
    global buoys_in_fov
    cv_grey = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)
    buoys = buoy_cascade.detectMultiScale(cv_grey, 1.3, 20)
    buoy_count = 0
    buoy_list = []

    for (x,y,w,h) in buoys:
        if (x*y) > 2000:
            # Calculating size of ROI
            current_buoy = Buoy()
            cv2.rectangle(cv_image,(x,y),(x+w,y+h),(255,0,0),2)
            buoy_count += 1
            roi = cv_image[y:y+h, x:x+w]

            buoy_color = detect_color(roi)
            cv_image = place_text(cv_image, (x, y-20), buoy_color + " Buoy", (255, 255, 255))

            current_buoy.color = buoy_color
            current_buoy.area = int(w*h)
            current_buoy.x_coord = int(x + 0.5*(w))
            current_buoy.y_coord = int(y + 0.5*(h))
            buoy_list.append(current_buoy)

    # Buoys in the current FOV are sorted from largest to smallest.
    # Because of this we can assume that the first bouy from the subscribed message
    # is the largest one that we are currently detecting.
    buoy_list = sorted(buoy_list, key = lambda buoy:buoy.area, reverse=True)
    buoys_in_fov.buoys = [buoy for buoy in buoy_list]

    #cv2.putText(cv_image, "Number of Buoys: " + str(buoy_count), (10, 410), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2, cv2.CV_AA)  # Places Object type on image
    #cv2.putText(cv_image, "Colors Visible: "+ " ".join(buoy_color_list), (10,440), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2, cv2.CV_AA)
    return cv_image

# Main Function
def main(args):
    ic = image_converter()
    rospy.init_node('image_converter', anonymous=True)
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print "Shutting down"

if __name__ == '__main__':
    main(sys.argv)