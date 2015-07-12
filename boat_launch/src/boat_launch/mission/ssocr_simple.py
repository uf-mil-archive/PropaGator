#!/usr/bin/env python

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

def convert_bgr2greyscale(cv_image):
    # Converts the image from BGR colorspace to greyscale
    return cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)

def convert_bgr2hsv(cv_image):
    # Converts the image from BGR colorspace to HSV inorder to threshold
    return cv2.cvtColor(cv_image, cv2.COLOR_BGR2HSV)

def resize_image(image,max_val):
    # Resizes the image...
    r = float(float(max_val)/image.shape[0])
    print r
    dim = (int(image.shape[1] * r),(max_val))
    print dim
    return cv2.resize(image, dim, interpolation = cv2.INTER_AREA)

def transform_image(image):
    # Carries out the appropriate transforms on the given input image. Here
    # we carry out thresholding for orange, and attempt to blur each of the
    # segments for the image to make one continuous blob that we can
    # treat as a hex-digit.
    max_size = 600
    resized_image = resize_image(image,max_size)
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
    val = int(val)
    # AUVSI's JSON server is expecting the values to be of a certain format.
    # For example, instead of '1', the server is expecting 'one'
    if val == 0:
        return 'ZERO'
    elif val == 1:
        return 'ONE'
    elif val == 2:
        return 'TWO'
    elif val == 3:
        return 'THREE'
    elif val == 4:
        return 'FOUR'
    elif val == 5:
        return 'FIVE'
    elif val == 6:
        return 'SIX'
    elif val == 7:
        return 'SEVEN'
    elif val == 8:
        return 'EIGHT'
    elif val == 9:
        return 'NINE'
    else:
        # Cases for 0x0A through 0x0F which should already be a string
        return str(val)


# HSV color-ranges used to threshold out the hexadecimal value
orange_min_hsv = np.array([0, 50, 50],np.uint8)
orange_max_hsv = np.array([10, 255, 255],np.uint8)

def detect_digits(args):

    working_path = args
    relevant_extensions = ['jpg','JPG','jpeg','Jpeg','JPEG','png','Png','PNG']

    # listing = os.listdir(working_path)
    listing = [fn for fn in os.listdir(args) if any([fn.endswith(ext) for ext in relevant_extensions])]

    # Image processing
    for infile in listing:
        if os.path.isdir(args + '/' + infile):
            pass
        else:
            print "Current file is: " + infile
            cv_image = cv2.imread(args + '/' + infile)

            #cv2.imshow('', cv_image)
            #cv2.waitKey(0)

            cv_image = transform_image(cv_image)

            #cv2.imshow('', cv_image)
            #cv2.waitKey(0)

            if np.mean(cv_image) == 255:
                # Images with everything thresholded out is
                # This image contains no value
                print 'CV2-Image is Empty: ', infile
            else:
                roi_small = resize_image(cv_image, 40)
                #cv2.imshow('',roi_small)
                #cv2.waitKey(0)

                cv2.imwrite(working_path + '/' + str(infile) + '.jpg', roi_small)

    listing = [fn for fn in os.listdir(working_path) if any([fn.endswith(ext) for ext in relevant_extensions])]
    accepted_ans = ['0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f']

    best_values = []
    all_values = []

    for infile in listing:
        try:
            text = str(subprocess.check_output(["ssocr","-d","-1", str(working_path + '/' + infile)]))
            if text is None:
                pass
            elif any([text == letters for letters in accepted_ans]):
                pass
            elif len(text) > 1 and text[1] != '.': 
            	print "Digit found in ", str(infile), ": " + str(text[0])
                all_values.append(text[0])
                all_values.append(str(working_path + infile))
            elif len(text) > 1 and text[1] == '.':
                best_values.append(text[0])
                best_values.append(str(working_path + infile))
        except subprocess.CalledProcessError:
            print "Exception occured, we are randomly picking value: ", rand_pick()

    # 0 index is number, 1 is path to file
    if len(best_values) == 0:
        best_values.append(all_values[0])
        best_values.append(all_values[1])

    best_values[0] = json_prep(best_values[0])

    return best_values

def main(image_path):
    to_send = ['a','a']
    to_send = detect_digits(image_path)
    return to_send
