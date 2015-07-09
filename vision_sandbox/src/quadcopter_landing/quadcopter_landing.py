__author__ = 'Ralph F. Leyva'

import cv2
import rospy
import roslib
import argparse
import numpy as np

from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

'''
    PROPAGATOR Quadcopter Challenge
        This script will implement simple thresholding along with
        hough-transforms for circle detection to demo quadcopter landing.
        An attempt at determining the height of the quadcopt off of the
        landing pad will also be made.

    Algorithm Overview:
        For this algorithm we will first attempt to find the two largest contours
        that correspond to the magenta mask. We will also use some proportions to make
        sure that the targets are sufficiently far enough. Once we have the regions
        of interests we will begin to run cam/meanshift algorithms in order to begin
        tracking the two landing targets. This same methodology will be used to find
        the bounding rectangle for the images
'''

parser = argparse.ArgumentParser()
parser.add_argument('-v', '--video', help='Enter path if you are providing a video file', required=False)
args = parser.parse_args()

video_stream = cv2.VideoCapture(str(args.video))

magenta_min = np.array([160,95,95], np.uint8)     # Works like a champ!
magenta_max = np.array([167,255,255], np.uint8)

green_min = np.array([40, 40, 0], np.uint8)
green_max = np.array([54, 200, 255], np.uint8)

def convert_bgr2greyscale(cv_image):
    return cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)

def convert_bgr2hsv(cv_image):
    # Converts the image from BGR colorspace to HSV inorder to threshold
    return cv2.cvtColor(cv_image, cv2.COLOR_BGR2HSV)

def convert_hsv2bgr(cv_image):
    return cv2.cvtColor(cv_image, cv2.COLOR_HSV2BGR)

def mask_image(cv_image, mask):
    # Carries out a bit-wise AND operation to only allow
    # pixels allowed by our mask1
    return cv2.bitwise_and(cv_image, cv_image, mask=mask)

def process_image(cv_image):
    # Thresholds image, generates masks, and performs open and closing operations
    # on the incoming frame. This will serve as the basis for the image processing
    kernel = np.ones((7,7),np.uint8)
    hsv_cv_image = convert_bgr2hsv(cv_image)
    blur_mask = cv2.medianBlur(hsv_cv_image, 5)
    eroded_img = cv2.erode(hsv_cv_image, kernel, iterations = 3)
    return cv2.dilate(eroded_img,kernel,iterations = 9)

def filter_green_markers(processed_cv_image):
    green_mask = cv2.inRange(processed_cv_image, green_min, green_max)
    thresh  = cv2.threshold(green_mask, 127, 255, cv2.THRESH_BINARY)[1]
    return mask_image(processed_cv_image, thresh)

def filter_magenta_markers(processed_cv_image):
    # Carries out the actual masking of the incoming image, allowing ONLY the
    # magenta landing markers to be visible in the image. This is the first
    # step in carrying out the cam/meanshift operations for tracking
    magenta_mask = cv2.inRange(processed_cv_image, magenta_min, magenta_max)
    thresh = cv2.threshold(magenta_mask, 100, 255, cv2.THRESH_BINARY)[1]
    return mask_image(processed_cv_image, thresh)

def track_magenta(masked):
    pass

def find_contours(cv_image_processed):
    return cv2.findContours(cv_image_processed, cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

def find_largest_rectangle(contours):
    areas = [cv2.contourArea(c) for c in contours]
    max_index = np.argmax(areas)
    return contours[max_index]

def camshift_algo():
    pass

def finding_landing_pad(cv_image):
    pass

def find_circle(cv_image, cv_thresh):
    contours, hierarchy = find_contours(convert_bgr2greyscale(cv_thresh))
    if contours is None:
        pass
    else:
        for i in contours:
            (x,y),radius = cv2.minEnclosingCircle(i)
            center = (int(x), int(y))
            radius = int(radius)
            cv2.circle(cv_image, center, radius, (0,255,0),5)

def main(args):
    count = 0
    cap = cv2.VideoCapture(args.video)
    ret, frame = cap.read()
    track_window = None
    #frame = None

    # while count < 60:
    #     # Waits 60 frames before
    #     ret, frame = cap.read()
    #     processed_frame = process_image(frame)
    #     blur_processed = cv2.bilateralFilter(processed_frame,9,75,75)
    #     landing_pad = filter_green_markers(blur_processed)
    #     contours,hierarchy =  find_contours(convert_bgr2greyscale(landing_pad))
    #
    #     max_contour = find_largest_rectangle(contours)
    #
    #     x,y,w,h = cv2.boundingRect(max_contour)
    #
    #     # x = x - 5
    #     # y = y - 5
    #     # w = w + 10
    #     # h = h + 10
    #
    #     track_window = (x,y,w,h)
    #     roi = frame[y:y+h,x:x+w]
    #     #cv2.rectangle(frame,(x,y),(x+w,y+h),(0,0,255),10)
    #     #cv2.imshow('', roi)
    #     if cv2.waitKey(1) & 0xFF == ord('q'):
    #         break
    #     count += 1

    kernel = np.ones((8,8),np.uint8)
    eroded_img = cv2.erode(frame, kernel, iterations = 2)
    dilated_img = cv2.dilate(eroded_img,kernel,iterations = 5)

    hsv_roi = cv2.cvtColor(dilated_img, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv_roi, green_min, green_max)

    cv2.imshow('', mask)
    cv2.waitKey(0)

    roi_hist = cv2.calcHist([hsv_roi],[0],mask,[180],[0,180])
    cv2.normalize(roi_hist,roi_hist,0,255,cv2.NORM_MINMAX)
    term_crit = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 5, 1)

    # while True:
    #     ret, frame = cap.read()
    #     if ret == True:
    #         eroded_img = cv2.erode(frame, kernel, iterations = 3)
    #         dilated_img = cv2.dilate(eroded_img,kernel,iterations = 5)
    #
    #         hsv = cv2.cvtColor(dilated_img, cv2.COLOR_BGR2HSV)
    #         dst = cv2.calcBackProject([hsv],[0],roi_hist,[0,180],1)
    #
    #         ret, track_window = cv2.CamShift(dst,track_window,term_crit)
    #
    #         pts = cv2.cv.BoxPoints(ret)
    #         pts = np.int0(pts)
    #
    #         cv2.polylines(frame, [pts], True, (0,0,255),10)
    #         cv2.imshow('img2',frame)
    #
    #         k = cv2.waitKey(60) & 0xff
    #         if k == 27:
    #             break
    #     else:
    #         break

    while True:
        ret, frame = cap.read()
        processed_frame = process_image(frame)
        landing_pad = filter_green_markers(processed_frame)
        contours,hierarchy = find_contours(convert_bgr2greyscale(landing_pad))

        max_contour = find_largest_rectangle(contours)

        # Non-Rotating Rectangle
        #x,y,w,h = cv2.boundingRect(max_contour)
        #track_window = (x,y,w,h)
        #roi = frame[y:y+h,x:x+w]
        #cv2.rectangle(frame,(x,y),(x+w,y+h),(0,0,255),10)

        # Rotating Rectangle
        #rect = cv2.minAreaRect(max_contour)
        #box = cv2.cv.BoxPoints(rect)
        #box = np.int0(box)
        #im = cv2.drawContours(frame,[box],0,(0,0,255),10)

        # epsilon = 0.005*cv2.arcLength(max_contour,True)
        # approx = cv2.approxPolyDP(max_contour,epsilon,True)

        hull = cv2.convexHull(max_contour, returnPoints=False)
        defects = cv2.convexityDefects(max_contour,hull)

        for i in range(defects.shape[0]):
            s,e,f,d = defects[i,0]
            start = tuple(max_contour[s][0])
            end = tuple(max_contour[e][0])
            far = tuple(max_contour[f][0])
            cv2.line(frame,start,end,[0,0,255],10)
            cv2.circle(frame,far,5,[0,0,255],-1)

        #cv2.drawContours(frame, [approx], -1, (0,0,255), 3)

        cv2.imshow('', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main(args)