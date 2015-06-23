from __future__ import division
import pygame
import numpy as np
import sys
from scipy.spatial import KDTree
from visualize import visualize_tree
from time import time

class RRT(object):
    sys.setrecursionlimit(10000)
    '''  
    1. Start with initial configuration
    2. Generate a random point, pt_rand
    3. Find the closest point, pt_close to that random point
    4. Attempt to reach toward pt_rand from pt_close
    '''
    def __init__(self, start_pt, end_pt, max_dist=5, max_iterations=1000):
        self.end_pt = end_pt
        self.tree = KDTree(np.array([start_pt]))
        self.points = np.array([start_pt])
        self.edges = []
        self.max_dist = max_dist
        self.max_iterations = max_iterations
        self.obstacles = []


    def compute(self):
        for k in xrange(self.max_iterations):
            done = self.explore()
            if done is True:
                print '--Converged--'
                break
        # --> Return just the path to the target
        # return self.tree


    def check_valid(self, point):
        for x, y, xlen, ylen in self.obstacles:
            if x < point[0] < x + xlen:
                if y < point[1] < y + ylen:
                    return False
        return True


    def extend(self, start_point, end_point):
        self.edges.append((start_point, end_point))
        self.points = np.vstack([self.points, end_point])
        self.tree = KDTree(self.points)

    def explore(self):
        '''Attempt to generate a path to the end point'''
        if np.random.random() < 0.1:
            random_pt = self.end_pt
        else:
            random_pt = (np.random.random(2) - 0.5) * 800

        dist, index = self.tree.query(random_pt)
        closest_pt = self.tree.data[index]

        difference = random_pt - closest_pt
        distance = np.linalg.norm(difference)

        growth_distance = min(distance, self.max_dist)
        new_pt = (growth_distance * self.unit_vector(difference)) + closest_pt

        if self.check_valid(new_pt):
            self.extend(closest_pt, new_pt)
        else:
            return False

        if np.linalg.norm(new_pt - self.end_pt) < (0.1 * self.max_dist):
            return True

        return False

    def unit_vector(self, vector):
        return vector / np.linalg.norm(vector)


if __name__ == '__main__':
    start = np.array([-80.0, 100.0])
    end = np.array([220.0, 300.0])
    tic = time()
    rrt = RRT(start, end, max_dist=15)
    rrt.obstacles.append(np.array((50., 50., 100., 100.)))
    rrt.obstacles.append(np.array((50., 200., 100., 300.)))

    rrt.compute()
    toc = time() - tic
    print 'RRT Generated, took {} seconds'.format(toc)
    visualize_tree(rrt, end_point=end, animate=False)