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
SCALE = 1.

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

def ray(display, start, angle, length, scale=0.1, color=(255, 0, 0), width=1):
    unit_v = np.array([np.sin(angle), np.cos(angle)])
    _start = round_point(start)
    _end = round_point(start + (unit_v * length * scale))

    pygame.draw.line(display, color,
        _start,
        _end,
        width
    )


def draw_spline(display, spline, iteration=10, color=(255, 0, 0), draw_slopes=False, has_theta=False):
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

        if has_theta:
            ray(display, spline[ind][:2], angle=spline[ind][2], length=25, width=3)


def visualize_spline(spline, title="Spline pathing visualization", animate=True, iteration_speed=1, lasts=0, end_point=None,
                     has_theta=False, vel_markers=False):
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
            pygame.draw.circle(display, (255, 0, 0,), round_point(end_point), 3)

        draw_spline(display, spline, iteration, has_theta=has_theta, draw_slopes=vel_markers)

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


def draw_rect(display, corner, size):
    _size = size * SCALE
    # pygame.draw.rect(display, (255, 255, 0), (point[0], point[1], size[0] * SCALE, size[1] * SCALE))
    pts = [
        round_point(corner),
        round_point(corner + [_size[0], 0.0]),
        round_point(corner + _size),
        round_point(corner + [0.0, _size[1]]),
    ]

    pygame.draw.polygon(
        display, 
        (255, 255, 0), 
        pts,
        0
    )

def visualize_tree(tree, end_point=None, animate=False, title='RRT Visualization'):
    display = pygame.display.set_mode(SCREEN_DIM)
    pygame.display.set_caption(title)

    clock = pygame.time.Clock()
    iteration = 0.0

    start_time = time.time()
    num_edges = len(tree.edges)

    if animate: 
        k = 0
    else:
        k = num_edges

    while not rospy.is_shutdown():

        for obstacle in tree.obstacles:
            draw_rect(display, obstacle[:2], obstacle[2:])

        if end_point is not None:
            pygame.draw.circle(display, (0, 255, 0,), round_point(end_point), 6)

        for edge_num in xrange(k):
            edge = tree.edges[edge_num]
            pygame.draw.line(display, (255, 0, 0), round_point(edge[0]), round_point(edge[1]))

        if animate:
            k += 1
            if k >= num_edges:
                k = 0

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return

            if event.type == pygame.KEYDOWN:
                if (event.key == pygame.K_ESCAPE) or (event.key == pygame.K_q):
                    return

        pygame.display.update()
        clock.tick(20)
        display.fill((0, 0, 0))


def main():
    '''Core draw loop'''
    print 'Randomized Spline Visualization Demo; Not intended to be run as main'
    from spline.spline import Hermite, Waypoint

    display = pygame.display.set_mode(SCREEN_DIM)
    pygame.display.set_caption("Spline pathing visualization")

    clock = pygame.time.Clock()

    iteration = 0.0
    start_pt = np.array([0.0, 0.0])
    start_m = np.array([10.0, 10.0])    
    end_pt = np.array([30.0, 30.0])
    end_m = np.array([-10.0, -10.0])

    spline = Hermite.spline(Waypoint(start_pt, start_m), Waypoint(end_pt, end_m))

    while not rospy.is_shutdown():

        iteration += 1
        if iteration >= len(spline):
            print "Resetting splines"
            start_m = np.random.uniform(-1, 1, 2) * 30
            end_pt = np.random.uniform(-1, 1, 2) * 30
            end_m = np.random.uniform(-1, 1, 2) * 100
            spline = Hermite.spline(Waypoint(start_pt, start_m), Waypoint(end_pt, end_m))
            print "Raw path computed (red)"
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