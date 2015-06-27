import numpy
import math
from txros import util
import cv2
import boat_scripting

@util.cancellableInlineCallbacks
def main(nh, boat=None):
    print "Q"
    if boat is None:
        boat = yield boat_scripting.get_boat(nh)
    
    while True:
        
        print 'find gates'
        angle = yield boat.get_start_gate_vision()    
        print "raw angle", angle

        distance = 0
        
        #yield boat.move.as_MoveToGoal([3,0,0],angle).go()

        # ADDED BBY ZACH -- Measures the distance the boat has gone forward to tell when done
        # Breaks and returns true so that we know we actually got done. 
        # Not a great implimentation but works for now
        # Change depending on how far apart the bouys are 
        if distance > 30: 
            print "Exiting Gates"
            break

        if abs(angle) < (numpy.pi/12):
            print "forward"
            yield boat.move.forward(6).go()
            yield util.sleep(0.5)                       
            print "forward command sent"
            distance += 6
        elif angle < 0:
            print "turn_left: ", angle/5
            yield boat.move.turn_left(abs(angle/5)).go()
            yield util.sleep(0.5)
        elif angle > 0:
            print "turn_right: ", angle/5
            yield boat.move.turn_right(angle/5).go()
            yield util.sleep(0.5)

        

        '''
        print 'left'
        yield boat.move.turn_left(abs(numpy.pi/3)).go()

        print 'right'
        yield boat.move.turn_right(numpy.pi/3).go()
        '''