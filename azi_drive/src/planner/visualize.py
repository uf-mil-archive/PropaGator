#!/usr/bin/python
from __future__ import division
## Math
import numpy as np
## Display
import pygame
import time
## Ros
import rospy
from tf import transformations as tf_trans
## Ros Msgs
from std_msgs.msg import Header, Float64
from geometry_msgs.msg import Point, PointStamped, PoseStamped, Pose, Quaternion

from spline import Hermite


SCREEN_DIM = (750, 750)
ORIGIN = np.array([SCREEN_DIM[0]/2.0, SCREEN_DIM[1]/2.0])


def print_in(f):
    print("Defining " + f.func_name)
    def print_on_entry(*args, **kwargs):
        print("Executing " + f.func_name + " with arguments " + str(args) + " and kwargs " + str(kwargs))
        result = f(*args, **kwargs)
        print("Returning " + str(result))
        return(result)
    return(print_on_entry)


def round_point((x, y)):
    '''Round and change point to centered coordinate system'''
    return map(int, ((10 * x) + ORIGIN[0], -(10 * y) + ORIGIN[1]))


def unround_point((x, y)):
    '''Change center-origin coordinates to pygame coordinates'''
    return ((x - ORIGIN[0]) / 10.0, (-y + ORIGIN[1]) / 10.0)


def draw_spline(display, start_pt, start_m, end_pt, end_m, max_t):
    prev_pt = start_pt
    for iteration in np.arange(0, max_t, 0.01):
        pt = Hermite.hermite_interpolate(start_pt, start_m, end_pt, np.array([0.1, 0.1]), iteration)
        pygame.draw.line(display, (255, 0, 0), round_point(prev_pt), round_point(pt))
        prev_pt = pt


def main():
    '''Core draw loop'''

    display = pygame.display.set_mode(SCREEN_DIM)
    pygame.display.set_caption("Spline Simulation")

    clock = pygame.time.Clock()

    iteration = 0.0
    start_pt = np.array([0.0, 0.0])
    start_m = np.array([-19.0, 40.0])    
    end_pt = np.array([10.0, -30.0])

    while not rospy.is_shutdown():

        # for t in np.arange(0, 1.05, 0.05):
        iteration += 0.01
        if iteration >= 1.05:
            iteration = 0

        draw_spline(display, start_pt, start_m, end_pt, np.array([0.0, 0.0]), iteration)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return

            if event.type == pygame.KEYDOWN:
                if (event.key == pygame.K_ESCAPE) or (event.key == pygame.K_q):
                    return

        t = time.time()        
        pygame.display.update()
        clock.tick(20)
        display.fill((0, 0, 0))


if __name__ == '__main__':
    main()