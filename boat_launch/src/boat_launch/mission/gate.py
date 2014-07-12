from __future__ import division

import numpy
import math

from txros import util

import boat_scripting


@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)
    
    @util.cancellableInlineCallbacks
    def go(x):
      
      while True:
        print 'a'
        scan = yield boat.get_scan()
        best = None
        angles = numpy.linspace(scan.angle_min, scan.angle_max, len(scan.ranges))
        pos = lambda i: numpy.array([math.cos(angles[i]), math.sin(angles[i]), 0]) * scan.ranges[i]
        for i in xrange(len(scan.ranges)//4, len(scan.ranges)*3//4):
            if not (scan.range_min <= scan.ranges[i] <= scan.range_max):
                continue
            for j in xrange(i+1, len(scan.ranges)*3//4):
                if not (scan.range_min <= scan.ranges[j] <= scan.range_max):
                    continue
                a = (pos(i) + pos(j))/2
                vec = pos(j) - pos(i)
                score = 1e6-a[0]
                if abs(vec[1])/numpy.linalg.norm(vec) < .5: score = 0
                if numpy.linalg.norm(vec) < 1.5: score = 0
                if numpy.linalg.norm(vec) > 7: score = 0
                if a[0] < abs(a[1])*1.5: score = 0
                if best is None or score > best_score:
                    best = a
                    best_score = score
        print best
        df = boat.move.relative(best).forward(x).go()
        if numpy.linalg.norm(x) < 3.5:
            yield df
            return

    yield go(0)
    yield go(+3)
