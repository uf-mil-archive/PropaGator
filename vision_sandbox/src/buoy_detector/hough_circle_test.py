__author__ = 'Ralph F. Leyva'

import roslib
import sys
import rospy
import cv2
import cv2.cv as cv

from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

import numpy as np

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
        img = detect_bouys(cv_image)
        try:
            self.image_pub.publish(self.bridge.cv2_to_imgmsg(img, "bgr8"))
        except CvBridgeError, e:
            print e

## Utility Functions
def convert_bgr2hsv(cv_image):
    return cv2.cvtColor(cv_image, cv2.COLOR_BGR2HSV)

def convert_bgr2greyscale(cv_image):
    return cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)

def convert_greyscale2bgr(cv_image):
    return cv2.cvtColor(cv_image, cv2.COLOR_GRAY2BGR)

def detect_bouys(cv_image):
    gray_img = convert_bgr2greyscale(cv_image)
    blur = cv2.bilateralFilter(gray_img,10,50,200)

    kernel = np.array([[1,1,1,1,1],[1,1,1,1,1],[1,1,1,1,1],[1,1,1,1,1],[1,1,1,1,1]],np.uint8)
    eroded_masked_img = cv2.erode(blur, kernel, iterations = 1)
    dilatated_masked_img = cv2.dilate(eroded_masked_img,kernel,iterations = 2)

    #cv2.imshow('image',blur)
    #cv2.waitKey(0)

    circles = cv2.HoughCircles(dilatated_masked_img, cv.CV_HOUGH_GRADIENT,1, 20, param1=40, param2=24, minRadius=10, maxRadius=40)
    if circles is None:
        pass
    else:
        circles = np.uint16(np.around(circles))
        for i in circles[0,:]:
            # draw the outer circle
            cv2.circle(cv_image,(i[0],i[1]),i[2],(0,255,0),2)
            # draw the center of the circle
            cv2.circle(cv_image,(i[0],i[1]),2,(0,0,255),3)
    return cv_image

# Main Function
def main(args):
    ic = image_converter()
    rospy.init_node('image_converter', anonymous=True)
    cv2.namedWindow('image', cv2.WINDOW_NORMAL)

    try:
        rospy.spin()
    except KeyboardInterrupt:
        print "Shutting down"

    cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)
