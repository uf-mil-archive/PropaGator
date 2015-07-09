__author__ = 'Ralph F. Leyva'

import os
import cv2
import errno
import rospy
import random
import roslib
import argparse
import subprocess
import numpy as np

import shutil
from std_msgs.msg import String
from server_interaction.msg import images_info

'''
    PROPAGATOR OCR
        Implements rudimentary OCR with the aid of SSOCR,
        an independently made application for carrying out text
        recognition on 7-segment displays. This scripts serves as a medium
        between this application and the ROS/JSON interface.

        In order to run this script SSOCR, must be installed and is available
        at https://www.unix-ag.uni-kl.de/~auerswal/ssocr/ This application
        depends on libimlib2-dev, and it should be installed in the default
        /usr/bin directory
'''

# Use for standalone use, sans ROS
# Add in catches for when ROS is not running and no argument is passed
parser = argparse.ArgumentParser()
parser.add_argument('-p', '--path', help='Input file directory', required=False)
args = parser.parse_args()

JSON_file_path = ''
detected_digits = []

# Ideally all ROS topics should be setup as a service that we can poll
# for information regarding file paths, and other things instead of having a constant
# stream of info.
def gather_parameters(images_info):
    global JSON_file_path
    JSON_file_path = str(images_info.file_path)
    args.path = JSON_file_path
    detect_digits(args)

def ros_json_interface():
    # ROS publishers and subscribers
    propagator_ocr = rospy.Publisher("/propagator_ocr/detected_text", String, queue_size=10)
    image_path = rospy.Subscriber("/server_images_info", images_info, gather_parameters)
    rospy.init_node('propagator_ocr', anonymous=True)
    rospy.spin()

def convert_bgr2greyscale(cv_image):
    # Converts the image from BGR colorspace to greyscale
    return cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)

def convert_bgr2hsv(cv_image):
    # Converts the image from BGR colorspace to HSV inorder to threshold
    return cv2.cvtColor(cv_image, cv2.COLOR_BGR2HSV)

def resize_image(image):
    # Resizes the image...
    r = 600.0/image.shape[0]
    dim = (int(image.shape[1] * r), 600)
    return cv2.resize(image, dim, interpolation = cv2.INTER_AREA)

def edge_detect(image):
    # Returns the edges currently in the image along with the hierarchy.
    # We only need the contours in this case since we will not be
    # interested in inner-contours of numbers.
    edges = cv2.Canny(image, 100, 255)
    contours, hierarchy = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    return contours, hierarchy

def find_max_contour(contours):
    # Returns the largest contour in the images (We're assuming that this is the
    # number that we're looking for)
    areas = [cv2.contourArea(c) for c in contours]
    max_index = np.argmax(areas)
    return contours[max_index]

def generate_roi(cv_image, contour):
    cv2.drawContours(cv_image, contour, -1, (0,255,0), 3)
    cv2.imshow('', cv_image)
    cv2.waitKey(0)

def transform_image(image):
    # Carries out the appropriate transforms on the given input image. Here
    # we carry out thresholding for orange, and attempt to blur each of the
    # segments for the image to make one continuous blob that we can
    # treat as a hex-digit.
    resized_image = resize_image(image)
    hsv_cv_image = convert_bgr2hsv(resized_image)
    orange_mask_hsv = cv2.inRange(hsv_cv_image, orange_min_hsv, orange_max_hsv)
    mask_blur = cv2.blur(orange_mask_hsv,(25,25))
    return cv2.threshold(mask_blur, 50, 255, cv2.THRESH_BINARY_INV)[1]

def rand_pick():
    # Used for cases where SSOCR cannot agree on what character is currently
    # in the image
    val = random.randint(0,15)
    if val == 10:
        return 'a'
    elif val == 11:
        return 'b'
    elif val == 12:
        return 'c'
    elif val == 13:
        return 'd'
    elif val == 14:
        return 'e'
    elif val == 15:
        return 'f'
    else:
        return str(val)

def json_prep(val):
    # AUVSI's JSON server is expecting the values to be of a certain format.
    # For example, instead of '1', the server is expecting 'one'
    if val == 0:
        return 'zero'
    elif val == 1:
        return 'one'
    elif val == 2:
        return 'two'
    elif val == 3:
        return 'three'
    elif val == 4:
        return 'four'
    elif val == 5:
        return 'five'
    elif val == 6:
        return 'six'
    elif val == 7:
        return 'seven'
    elif val == 8:
        return 'eight'
    elif val == 9:
        return 'nine'
    else:
        # Cases for 0x0A through 0x0F which should already be a string
        return str(val)

# HSV color-ranges used to threshold out the hexadecimal value
orange_min_hsv = np.array([0, 50, 50],np.uint8)
orange_max_hsv = np.array([10, 255, 255],np.uint8)

def detect_digits(args):
    # Creating temporary directory to save treated CV images inorder to run SSOCR
    # This is for cases where we are NOT using ROS
    try:
        os.makedirs(args.path + '/' + 'tmp_path')
    except OSError as exception:
        if exception.errno != errno.EEXIST:
            raise

    working_path = args.path + '/' + 'tmp_path'
    relevant_extensions = ['jpg','JPG','jpeg','Jpeg','JPEG','png','Png','PNG']

    # listing = os.listdir(working_path)
    listing = [fn for fn in os.listdir(args.path) if any([fn.endswith(ext) for ext in relevant_extensions])]

    # Image processing
    for infile in listing:
        if os.path.isdir(args.path + '/' + infile):
            pass
        else:
            print "Current file is: " + infile
            cv_image = cv2.imread(args.path + '/' + infile)
            cv_image = transform_image(cv_image)

            cv2.imshow('', cv_image)
            cv2.waitKey(0)

            if np.mean(cv_image) == 255:
                # Images with everything thresholded out is
                print 'CV2-Image is Empty: ', infile
            else:
                contours, hierarchy = edge_detect(cv_image)
                max_contour = find_max_contour(contours)
                # Generate bounding rectangle which contains our region of interest
                [x, y, w, h] = cv2.boundingRect(max_contour)
                cv2.rectangle(cv_image, (x,y), (x+w, y+y+h), (0,255,0), 2)
                roi = cv_image[y:y+h, x:x+w]

                # Seems to work well enough for most characters
                roi_small_sqr = cv2.resize(roi,(30,30))
                cv2.imwrite(working_path + '/' + str(infile) + '.jpg', roi_small_sqr)

    #listing = os.listdir(working_path)
    listing = [fn for fn in os.listdir(working_path) if any([fn.endswith(ext) for ext in relevant_extensions])]
    accepted_ans = ['0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f']

    for infile in listing:
        try:
            text = str(subprocess.check_output(["ssocr","-d","-1", str(working_path + '/' + infile)]))
            if text is None:
                print "No digit found in " + str(infile)
            elif any([text == letters for letters in accepted_ans]):
                print "Probable digit found, but unrecognized..."
            else:
                print "Digit found in ", str(infile), ": " + str(text)
        except subprocess.CalledProcessError:
            print "Exception occured, we are randomly picking value: ", rand_pick()

    print "Removing temporary work path"
    shutil.rmtree(working_path)
    rospy.signal_shutdown('Digit detection has finished')

if __name__ == '__main__':
    if args.path is None:
        print 'No argument provided, subscribing to /server_images_info'
        ros_json_interface()
    else:
        detect_digits(args)
