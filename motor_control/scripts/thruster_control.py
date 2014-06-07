#!/usr/bin/env python

##
##
##

import rospy
import serial
from motor_control.msg import thrusterConfig
from motor_control.msg import thrusterStatus

#Define some constants
#Note: these should be replaced with ros Params
STARBOARD_THRUSTER = 2
PORT_THRUSTER = 3

#    PWM values, 20 ms duty cycle with pulse width of
## Direction:       Reverse  Stop    Forward
## Output value:    45       90      145
## Pulse width:     1.04ms   1.49ms  2.07ms
ZERO = 90
FULL_FORWARD = 145
FULL_REVERSE = 45

## Conversion constants
MAX_NEWTONS = 100.0       #Just guessing
FORWARD_CONV = (FULL_FORWARD - ZERO) / MAX_NEWTONS
REVERSE_CONV = (ZERO - FULL_REVERSE) / MAX_NEWTONS

#define some global vars
starboard_thruster_value = 0
port_thruster_value = 0

#Define serial vars
#WARNING: you'll need permissions to access this file, or chmod it
ser = serial.Serial('/dev/ttyACM0')          

def stopThrusters():
    ser.write(str(STARBOARD_THRUSTER)+","+str(int(ZERO))+":")
    ser.write(str(PORT_THRUSTER)+","+str(int(ZERO))+":")


def motorConfigCallback(config):
    global starboard_thruster_value
    global port_thruster_value

    #Added : to prevent writing to messages in a row i.e. 1,234:2,65:
    ser.write(str(config.id)+","+str(int(convertNewtonsToDuty(config.thrust)))+":")
    if config.id == STARBOARD_THRUSTER:
        starboard_thruster_value = config.thrust
    else:
        port_thruster_value = config.thrust

def convertNewtonsToDuty(newtons):
    print(newtons)
    #Temporary Conversion
    if newtons < 0:
        newtons = ZERO + newtons * REVERSE_CONV
    else:
        print(FORWARD_CONV)
        newtons = ZERO + newtons * FORWARD_CONV

    #Make sure its not outta bounds
    if newtons > 145:
        newtons = 145
    elif newtons < 45:
        newtons = 45

    print(newtons)
    
    return newtons

def motorDirCtrl():
    
    #Setup ros
    rospy.init_node('thruster_control', anonymous=True)
    pub = rospy.Publisher('thruster_status', thrusterStatus, queue_size=10)
    rospy.Subscriber("thruster_config", thrusterConfig, motorConfigCallback)
    r = rospy.Rate(5)          #50 hz... I think
    
    #Initilize the motors to 0
    stopThrusters()

    #Main loop
    while not rospy.is_shutdown():
        #Periodically publish staus
        thruster = thrusterValue(STARBOARD_THRUSTER, starboard_thruster_value)
        pub.publish(thruster)
        thruster = thrusterValue(PORT_THRUSTER, port_thruster_value)
        pub.publish(thruster)
        
        #Wait till next cycle
        r.sleep()
    
    #Clean up
    ser.close()

if __name__ == '__main__':
    try:
        motorDirCtrl()
    except rospy.ROSInterruptException: pass
