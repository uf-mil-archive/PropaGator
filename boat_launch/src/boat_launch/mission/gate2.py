from __future__ import division

import math

import numpy

from txros import util

import boat_scripting


@util.cancellableInlineCallbacks
def main(nh, direction):
    print 1
    boat = yield boat_scripting.get_boat(nh)
    print 2
    
    assert direction in ['left', 'right']
    
    @util.cancellableInlineCallbacks
    def go():
        print 'a'
        last_best = None
        while True:
            scan = yield boat.get_scan()
            pose = boat.odom
            best = None
            best2 = None
            
            angles = numpy.linspace(scan.angle_min, scan.angle_max, len(scan.ranges))
            pos = lambda i: scan.ranges[i]*numpy.array([math.cos(angles[i]), math.sin(angles[i]), 0])
            for i in xrange(len(scan.ranges)//6, len(scan.ranges)*5//6):
                if not (scan.range_min <= scan.ranges[i] <= scan.range_max):
                    continue
                for j in xrange(i, len(scan.ranges)*5//6):
                    if not (scan.range_min <= scan.ranges[j] <= scan.range_max):
                        continue
                    center = (pos(i) + pos(j))/2
                    vec = pos(j) - pos(i)
                    
                    if numpy.linalg.norm(vec) < 1 or numpy.linalg.norm(vec) > 5:
                        continue
                    
                    bad = False
                    for k in xrange(int(i*.75+j*.25), int(i*.25+j*.75)):
                        if not (scan.range_min <= scan.ranges[k] <= scan.range_max):
                            continue
                        if numpy.linalg.norm(pos(k) - center) < 5:
                            bad = True
                            break
                    if bad: continue
                    
                    score = center[1] if direction == 'left' else -center[1]
                    
                    if best is None or score > best_score:
                        best = center
                        best_score = score
                        best2 = i, j, center, vec
            
            print best, best2
            print scan.ranges[best2[0]:best2[1]+3]
            
            if best is not None:
                last_best = pose.relative(best).position
            
            if last_best is None:
                df = boat.move.forward(5).go()
            else:
                df = boat.move.set_position(last_best).go()
                if numpy.linalg.norm(last_best - boat.pose.position) < 5:
                    print 'waiting'
                    yield df
                    break
            
            yield util.sleep(.1)
    
    yield go()
    
    yield boat.move.forward(2).go()
    
    yield go()
    yield boat.move.forward(2).go()
