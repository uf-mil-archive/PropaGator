#!/usr/bin/env python

##
## If this node stops sending to the motors propely try decreasing RAMP_RATE by
## 0.1          If that doesn't work try decreasing the ros rate (sleep time)
##

import rospy
import serial
from motor_control.msg import thrusterConfig
from motor_control.msg import thrusterStatus
import time

#Define some constants
#Note: these should be replaced with ros Params
STARBOARD_THRUSTER = 2
PORT_THRUSTER = 3

#    PWM values, 20 ms duty cycle with pulse width of
## Direction:       Reverse  Stop    Forward
## Output value:    45       91      145
## Pulse width:     1.04ms   ~1.49ms  2.07ms
ZERO = 91
FULL_FORWARD = 145
FULL_REVERSE = 45

## Conversion constants
#MAX_NEWTONS = 100.0       #Just guessing
#FORWARD_CONV = (FULL_FORWARD - ZERO) / MAX_NEWTONS
#REVERSE_CONV = (ZERO - FULL_REVERSE) / MAX_NEWTONS

#define some global vars
starboard_setpoint = 0.0
port_setpoint = 0.0

#Timing Variables
PUB_RATE = rospy.Duration(0.01)
RAMP_RATE = 1.0

#Pub
pub = rospy.Publisher('thruster_status', thrusterStatus, queue_size=10)


#Define serial vars
#WARNING: you'll need permissions to access this file, or chmod it
ser = serial.Serial('/dev/ttyACM0', 115200)  
        
def stopThrusters():
    ser.write(str(STARBOARD_THRUSTER)+","+str(int(ZERO))+":")
    ser.write(str(PORT_THRUSTER)+","+str(int(ZERO))+":")


def motorConfigCallback(config):
    global starboard_setpoint
    global port_setpoint
    
    if config.id == STARBOARD_THRUSTER:
        starboard_setpoint = config.thrust
    elif config.id == PORT_THRUSTER:
        port_setpoint = config.thrust
    else:
        rospy.logwarn("Id: %i, is not a known id", config.id);

def convertNewtonsToDuty(newtons):
    #Using a polynomial interpolation of power 3
    #These values were determined through experiment
    if newtons < 0:
        newtons*=-1
        newtons = -0.0055*newtons**3 + 0.224*newtons**2 - 3.9836 * newtons + 86.679
        rospy.loginfo("Negative: %i", newtons)
    elif newtons > 0:
        newtons = 0.0016*newtons**3 - 0.1027*newtons**2 + 2.812*newtons + 96.116
        rospy.loginfo("Positive: %i", newtons)
    else:
        newtons = ZERO;

    #Make sure its not outta bounds
    if newtons > FULL_FORWARD:
        newtons = FULL_FORWARD
    elif newtons < FULL_REVERSE:
        newtons = FULL_REVERSE
    
    return newtons

def pubStatus(event):
    #Periodically publish staus
    thruster = thrusterStatus(STARBOARD_THRUSTER, starboard_setpoint)
    pub.publish(thruster)
    thruster = thrusterStatus(PORT_THRUSTER, port_setpoint)
    pub.publish(thruster)

def motorDirCtrl():
    
    #Setup ros
    rospy.init_node('thruster_control', anonymous=True)
    rospy.Subscriber("thruster_config", thrusterConfig, motorConfigCallback)
    r = rospy.Rate(1000)          #1000 hz(1ms Period)... I think
    pub_timer = rospy.Timer(PUB_RATE, pubStatus)
    
    port_current = 0.0
    starboard_current = 0.0
    
    #Initilize the motors to 0
    stopThrusters()

    #Main loop
    while not rospy.is_shutdown():
        #update vlaues
        if port_current > port_setpoint:
            port_current -= RAMP_RATE
            if port_current < port_setpoint:
                port_current = port_setpoint
        elif port_current < port_setpoint:
            port_current += RAMP_RATE
            if port_current > port_setpoint:
                port_current = port_setpoint

        if starboard_current > starboard_setpoint:
            starboard_current -= RAMP_RATE
            if starboard_current < starboard_setpoint:
                starboard_current = starboard_setpoint
        elif starboard_current < starboard_setpoint:
            starboard_current += RAMP_RATE
            if starboard_current > starboard_setpoint:
                starboard_current = starboard_setpoint
        
        #Write to the serial bus
        #Added : to prevent writing to messages in a row i.e. 1,234:2,65:
        ser.write(str(PORT_THRUSTER)+","+str(int(convertNewtonsToDuty(port_current)))+":")
        ser.write(str(STARBOARD_THRUSTER)+","+str(int(convertNewtonsToDuty(starboard_current)))+":")
        
        #Wait till next cycle
        r.sleep()
    
    #Clean up
    stopThrusters()
    ser.close()

if __name__ == '__main__':
    try:
        motorDirCtrl()
    except rospy.ROSInterruptException: pass
