import numpy
import math
from txros import util
import cv2
import boat_scripting

@util.cancellableInlineCallbacks
def main(nh):
    print "Q"
    boat = yield boat_scripting.get_boat(nh)
    case = True
    while case == True:
        print 'find gates'
        #img = cv2.imread('Images/2014-4.jpg')
        angle = yield boat.get_start_gate_vision()    
        angle = angle
        if abs(angle) < (numpy.pi/10):
            print "forward"
            yield boat.move.forward(14).go()                       
        elif angle < 0:
            print "turn_left: ", angle
            yield boat.move.turn_left(abs(angle)).go()
        elif angle > 0:
            print "turn_right: ", angle
            yield boat.move.turn_right(angle).go()
