__author__ = 'Ralph F. Leyva'

import os
import sys
import cv2
import rospy
import roslib
import numpy as np

from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

'''
    PROPAGATOR BUOY DETECTOR
        This script implements a 20-stage cascade classifier used to
        detect buoys. The training data is available, and consists of
        approx. 1.2k positive images, and +2k negative images. HOG features
        were detected, and ADA-boost weak learners were used for the cascade.
'''

# Imports the pre-trained cascade classifier from path & creates classifer
cascade_path = os.path.join(os.path.expanduser('~'), 'Documents', 'Development', 'Classifiers', 'cascade.xml')
buoy_cascade = cv2.CascadeClassifier(cascade_path)

def convert_bgr2greyscale(cv_image):
    return cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)

class image_converter:
    # Object that is used to convert images from the ROS format to the OpenCV format
    def __init__(self):
        self.bridge = CvBridge()
        self.image_pub = rospy.Publisher("/camera/cv2_cascade_out", Image)
        self.image_sub = rospy.Subscriber("/camera/image_raw", Image, self.callback)
        self.buoy_fov = rospy.Publisher("/camera/buoy_img", String)

    def callback(self,data):
        try:
            cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
        except CvBridgeError, e:
            print e

        final_img = image_process(cv_image)

        try:
            self.image_pub.publish(self.bridge.cv2_to_imgmsg(final_img, "bgr8"))
        except CvBridgeError, e:
            print e

class color_mask():
    def __init__(self):
        # HSV color-ranges
        self.red_min = np.array([165,0,0], np.uint8)
        self.red_max = np.array([14,255,255], np.uint8)

        self.orange_min = np.array([15,0,0], np.uint8)
        self.orange_max = np.array([22,255,255], np.uint8)

        self.yellow_min = np.array([23,0,0], np.uint8)
        self.yellow_max = np.array([37,255,255], np.uint8)

        self.green_min = np.array([38,0,0], np.uint8)
        self.green_max = np.array([75,255,255], np.uint8)

        self.cyan_min = np.array([76,0,0], np.uint8)
        self.cyan_max = np.array([97,255,255], np.uint8)

        self.blue_min = np.array([98,0,0], np.uint8)
        self.blue_max = np.array([127,255,255], np.uint8)

        self.violet_min = np.array([128,0,0], np.uint8)
        self.violet_max = np.array([142,255,255], np.uint8)

        self.magenta_min = np.array([143,0,0], np.uint8)
        self.magenta_max = np.array([164,255,255], np.uint8)

    def match_color(self, color_val):
        if self.in_range(color_val, self.red_min, self.red_max):
            return "Red"
        elif self.in_range(color_val, self.orange_min, self.orange_max):
            return "Orange"
        elif self.in_range(color_val, self.yellow_min, self.yellow_max):
            return "Yellow"
        elif self.in_range(color_val, self.green_min, self.green_max):
            return "Green"
        elif self.in_range(color_val, self.blue_min, self.blue_max):
            return "Blue"
        elif self.in_range(color_val, self.violet_min, self.violet_max):
            return "Violet"
        elif self.in_range(color_val, self.magenta_min, self.magenta_red_max):
            return "Magenta"
        else:
            return "Unknown"

def find_circle_roi(cv_image, roi):
    gray_roi = convert_bgr2greyscale(roi)
    blur = cv2.bilateralFilter(gray_roi,10,50,200)
    kernel = np.array([[1,1,1,1,1],[1,1,1,1,1],[1,1,1,1,1],[1,1,1,1,1],[1,1,1,1,1]],np.uint8)
    eroded_roi = cv2.erode(blur, kernel, iterations = 1)
    dilatated_roi = cv2.dilate(eroded_roi, kernel, iterations = 2)
    circles = cv2.HoughCircles(dilatated_roi, cv2.cv.CV_HOUGH_GRADIENT,1, 20, param1=40, param2=24, minRadius=10, maxRadius=40)

    if circles is None:
        pass
    else:
        circles = np.uint16(np.around(circles))
        for i in circles[0,:]:
            cv2.circle(roi,(i[0],i[1]),i[2],(0,255,0),2)
            # draw the center of the circle
            cv2.circle(roi,(i[0],i[1]),2,(0,0,255),3)

def place_text(image, coords, string, color):
    # Function that is used to place text onto the image
    cv2.putText(image, string, coords, cv2.FONT_HERSHEY_PLAIN, 1, color, 1, cv2.CV_AA)  # Places Object type on image
    cv2.putText(image, str(coords), (coords[0], coords[1]+13), cv2.FONT_HERSHEY_PLAIN, 1, color, 1, cv2.CV_AA)  # Places realtime coords on image
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
    find_color = color_mask()
    kernel = np.ones((5,5),np.uint8)
    roi = cv2.erode(roi,kernel,iterations=1)
    roi = cv2.dilate(roi,kernel,iterations=1)
    edges = cv2.Canny(roi, 50, 150)
    contours, hierarchy = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    cv2.drawContours(roi,contours,-1,(0,255,0),2)
    return roi

def detect_color(cv_image,y,x):
    # Used to detect the colors of the buoys in the current ROI
    poi_rgb = cv_image[y,x,0:3]
    x=[[[poi_rgb[0], poi_rgb[1], poi_rgb[2]]]]
    poi_hsv = cv2.cvtColor(np.asarray(x), cv2.COLOR_BGR2HSV)
    h = np.reshape(poi_hsv, (1,3))
    h = h[0][0]

    if h > 70 and h < 80:     # Green
        return "Green"
    elif h > 25 and h < 33:   # Blue
        return "Yellow"
    elif h > 175 and h < 179: # Red
        return "Red"
    elif h > 110 and h < 130: # Yellow
        return "Blue"
    else:                     # Unknown Color
        return "Unknown Color"

def image_process(cv_image):
    # Passes each frame through the cascade classifiers and places bounding
    # rectangles on the ROIs.
    color = color_mask()
    cv_grey = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)
    buoys = buoy_cascade.detectMultiScale(cv_grey, 1.3, 20)
    buoy_count = 0
    buoy_color_list = []

    for (x,y,w,h) in buoys:
        cv2.rectangle(cv_image,(x,y),(x+w,y+h),(255,0,0),2)
        buoy_count += 1
        roi = cv_image[y:y+h, x:x+w]
        print(roi.shape)
        #roi_center = find_roi_contour(roi, cv_image)
        #color.find_max_color(roi_center)

        find_circle_roi(roi, cv_image)

        poi = find_center(roi)
        buoy_color = detect_color(roi, poi[0], poi[1])
        buoy_color_list.append(buoy_color)
        cv_image = place_text(cv_image, (x, y-20), buoy_color + " Buoy", (255, 255, 255))

    cv2.putText(cv_image, "Number of Buoys: " + str(buoy_count), (10, 410), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2, cv2.CV_AA)  # Places Object type on image
    cv2.putText(cv_image, "Colors Visible: "+ " ".join(buoy_color_list), (10,440), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2, cv2.CV_AA)
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
