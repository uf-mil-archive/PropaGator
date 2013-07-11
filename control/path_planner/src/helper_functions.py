#!/usr/bin/env python

import math,time
import random,numpy,threading


def distance (p1,p2):
	return (math.sqrt((p2[1]-p1[1])**2 + (p2[0]-p1[0])**2))
	
def center_of_points((p1,p2)):
	return numpy.mean([p1,p2],0)
	
def find_pair_center(red,green):
        print 'dist',distance(red,green)
        if (distance(red,green) < channel_width and distance(red,green) > .3):
                return [True,center_of_points((red,green))]
        else:
                return [False,[0,0]]
def max_dist(x,ref):
        ans = x[0]
        for i in x:
                if (distance(i,ref) > ans):
                        i = ans
        return ans

def get_perp(p1,p2):
    x = p2[0]-p1[0]
    y = p2[1]-p1[1]
    vec = max_dist(([-y,x],[y,-x]),center_of_points((p1,p2))) 
    return vec/numpy.linalg.norm(vec)

def three_d(x):
	return numpy.array([x[0], x[1], 0])                                
			
