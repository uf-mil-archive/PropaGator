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

SCREEN_DIM = (750, 750)
ORIGIN = np.array([SCREEN_DIM[0]/2.0, SCREEN_DIM[1]/2.0])
SCALE = 10.

def print_in(f):
    print("Defining " + f.func_name)
    def print_on_entry(*args, **kwargs):
        print("Executing " + f.func_name + " with arguments " + str(args) + " and kwargs " + str(kwargs))
        result = f(*args, **kwargs)
        print("Returning " + str(result))
        return(result)
    return(print_on_entry)


def round_point(pt):
    '''Round and change point to centered coordinate system'''
    x, y = pt[:2]
    return map(int, ((SCALE * x) + ORIGIN[0], -(SCALE * y) + ORIGIN[1]))


def unround_point((x, y)):
    '''Change center-origin coordinates to pygame coordinates'''
    return ((x - ORIGIN[0]) / SCALE, (-y + ORIGIN[1]) / SCALE)


def vector(display, start, direction, scale=0.1, color=(0, 255, 0)):
    pygame.draw.line(display, color,
        round_point(start),
        round_point(np.array(start) + (np.array(direction) * scale)),
    )


def draw_spline(display, spline, iteration=10, color=(255, 0, 0), draw_slopes=False):
    speed = np.diff(spline, 1, 0) / 0.01
    acc = np.diff(spline, 2, 0) / 0.01

    if len(spline) > 2:
        prev_pt = spline[0]
        for ind, pt in enumerate(spline):
            if ind >= iteration:
                break

            pygame.draw.line(display, color, round_point(pt), round_point(prev_pt))
            prev_pt = pt
            if ind < len(speed) and draw_slopes:
                vector(display, spline[ind], speed[ind], 0.1, color=(255, 200, 0))

        if ind > 2:
            # if Hermite.spline_max_q(spline[max(0, ind - 10):ind]) >= Hermite.spline_max_q(spline):
                # dot_color = (0, 0, 255)
            # else:
                # dot_color = (0, 255, 0)
            dot_color = (0, 255, 0)
            pygame.draw.circle(display, dot_color, round_point(spline[ind]), 3)

        if ind < len(acc):
            vector(display, spline[ind], speed[ind], 0.1, color=(255, 255, 0))
            vector(display, spline[ind], acc[ind], 0.5, color=(0, 255, 255))


def visualize_spline(spline, title="Spline pathing visualization", animate=True, iteration_speed=1, lasts=0, end_point=None):
    '''visualize_spline(spline, title="Spline pathing visualization", animate=True, iteration_speed=1, lasts=0):
    Visualize a sequence of points
        title: String that will set the window title
        animate: Animate the motion of the particle along the spline. If False, will draw the whole spline in one frame
        iteration_speed: Higher is faster
        lasts: Number of seconds to hold the window open. 0 or inf will keep it open until the user exits

    Exit the window with q or the exit button

    '''
    display = pygame.display.set_mode(SCREEN_DIM)
    pygame.display.set_caption(title)

    clock = pygame.time.Clock()
    iteration = 0.0

    start_time = time.time()
    while not rospy.is_shutdown():

        if animate:
            iteration += iteration_speed
            if iteration >= len(spline):
                iteration = 0
        else:
            iteration = len(spline) - 1

        if end_point is not None:
            pygame.draw.circle(display, (255, 0, 0,), round_point(end_point), 6)

        draw_spline(display, spline, iteration, draw_slopes=(not animate))

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return

            if event.type == pygame.KEYDOWN:
                if (event.key == pygame.K_ESCAPE) or (event.key == pygame.K_q):
                    return

        t = time.time()

        if (t - start_time > lasts) and (lasts > 0):
            return

        pygame.display.update()
        clock.tick(20)
        display.fill((0, 0, 0))


def main():
    '''Core draw loop'''
    from spline.spline import Hermite, Waypoint
    from policy_search import Policy_Search

    display = pygame.display.set_mode(SCREEN_DIM)
    pygame.display.set_caption("Spline pathing visualization")

    clock = pygame.time.Clock()

    iteration = 0.0
    start_pt = np.array([0.0, 0.0])
    start_m = np.array([10.0, 10.0])    
    end_pt = np.array([30.0, 30.0])
    end_m = np.array([-10.0, -10.0])

    spline = Hermite.spline(Waypoint(start_pt, start_m), Waypoint(end_pt, end_m))
    # opt_spline = Policy_Search.compute_policy(start_pt, end_pt, end_m)

    while not rospy.is_shutdown():

        iteration += 1
        if iteration >= len(spline):
            print "Resetting splines"
            start_m = np.random.uniform(-1, 1, 2) * 30
            end_pt = np.random.uniform(-1, 1, 2) * 30
            end_m = np.random.uniform(-1, 1, 2) * 100
            spline = Hermite.spline(Waypoint(start_pt, start_m), Waypoint(end_pt, end_m))
            print "Raw path computed (red)"
            # opt_spline = Policy_Search.compute_policy(start_pt, end_pt, end_m)
            print "Optimal path computed (blue)"
            iteration = 0

        draw_spline(display, spline, iteration)
        # draw_spline(display, opt_spline, iteration, color=(0, 0, 255))

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