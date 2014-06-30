#!/usr/bin/env python

##
##                          thruster_control node
##
## Purpose:
##  This script accepts ROS msgs of type thrusterConfig on topic /thruster_status
##  and converts the given newtons into a degree output sutiable for the arduino
##  servo class. A ramping function then moves the current value to the desired value
##  This value is ouput over serial each itteration to an arduino with the ino file in
##  the src folder of this same package, the arduino then generates the appropriate pwm
##  to send to the Victor 888 motor contolers which are attached to the thrusters
##
##  This node (thruster_control) also ouputs the current newton output of the particular
##  thruster periodically
##
## Trouble shooting:
##  If this node stops sending to the motors propely try decreasing RAMP_RATE by 0.1          
##  If that doesn't work try decreasing the ros rate (sleep time)
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

#    PWM and coresponding values, 20 ms duty cycle
## Direction:       Reverse  Stop    Forward    unit
## Output value:    45       91      145        Degrees
## Pulse width:     1.04    ~1.49    2.07       ms
## Newton input:   -23.65047 0       39.24135   N
ZERO_DEG = 91               #Zero in degrees
MAX_DEG = 145               #full forward in degrees
MIN_DEG = 45                #full reverse in degrees
MAX_NEWTONS = 39.24135      #Full forward in newtons
MIN_NEWTONS = -23.65047     #Full reverse in newtons

#These are the valuse that the thrusters 
# attempt to achieve in newtons
starboard_setpoint = 0.0
port_setpoint = 0.0
#Values that the thrusters are currently at
# in newtons
port_current = 0.0
starboard_current = 0.0

#Timing Variables
PUB_RATE = rospy.Duration(0.01)
UPDATE_RATE = 1000                      #Update every 1000 Hz
RAMP_RATE = 1.0 * UPDATE_RATE / 1000    #1 Degree * update_rate * (1s / 1000 ms) = [1 DEG/MS]

#Pub
pub = rospy.Publisher('thruster_status', thrusterStatus, queue_size=10)


#Define serial vars
#WARNING: you'll need permissions to access this file, or chmod it
ser = serial.Serial('/dev/ttyACM0', 115200) 

#       stopThrusters
# Input: none
# Output: none
# Description: Sets both thrusters to ZERO_DEG imediatly        
def stopThrusters():
    global starboard_setpoint
    global port_setpoint
    global starboard_current
    global port_current
    
    #Write zero to thrusters
    ser.write(str(STARBOARD_THRUSTER)+","+str(int(ZERO_DEG))+":")
    ser.write(str(PORT_THRUSTER)+","+str(int(ZERO_DEG))+":")

    #Zero internal varibles
    starboard_setpoint = 0.0;
    port_setpoint = 0.0;
    starboard_current = 0.0
    port_current = 0.0

#   motorConfigCallback
# input: thrusterConfig
# Output: none
# Description: This is the call back for config msgs to the thrusters
#               It checks to ensure the correct thrusters are being addressed
#               It checks to make sure the thrust settings are within bounds
#               It then sets the desired level for the ramp function in main
#               to work towards
def motorConfigCallback(config):
    global starboard_setpoint
    global port_setpoint

    #Check to ensure we are working with an appropriate thruster
    if (config.id != STARBOARD_THRUSTER and config.id != PORT_THRUSTER):
        rospy.logwarn("Id: %i, is not a known id", config.id);
        return
    
    thrust = config.thrust

    #Check if we are more than 0.25 newtons out of bounds
    if thrust > MAX_NEWTONS:
        if thrust > (MAX_NEWTONS + 0.25):
            rospy.logwarn("Attempted to set thruster %i to %f which is significantly above maximum (39.24135N) output thrust cliped to max. achivable value", config.id, thrust)
        thrust = MAX_NEWTONS;
    elif thrust < MIN_NEWTONS:
        if thrust < (MIN_NEWTONS - 0.25):
            rospy.logwarn("Attempted to set thruster %i to %f which is significantly below minimum (-23.65047N) output thrust cliped to min. achivable value", config.id, thrust)
        thrust = MIN_NEWTONS
    
    #Set the setpoints
    if config.id == STARBOARD_THRUSTER:
        starboard_setpoint = thrust
    elif config.id == PORT_THRUSTER:
        port_setpoint = thrust        

#   ConvertNewtonsToDuty
# Input: floating point (Newtons)
# Output: floating point (degrees)
# Description: Converts a value in newtons to a degree value suitable for the
#               arduino servo class. From testing a degree 3 polynomial 
#               Interpolation is used to convert newtons in the positive
#               and negative region to Degrees
def convertNewtonsToDuty(newtons):
    out = newtons
    # These equations were determined experimentally
    # Degree 3 polynomials were used to map newton inputs to
    # the coresponding forward and reverse deg representaitons:
    #   Forward_deg = 0.0016x^3 - 0.1027x^2 + 2.812X + 96.116
    #   Reverse_deg = 0.0055x^3 + 0.224X^2 + 3.9836x + 86.679
    # Where x is equal to the inputed newtons
    if out < 0:
        out = 0.0055*out**3 + 0.224*out**2 + 3.9836 * out + 86.679
    elif out > 0:
        out = 0.0016*out**3 - 0.1027*out**2 + 2.812*out + 96.116
    else:
        out = ZERO_DEG;

    #Bounds should be taken care of in the motor callback
    #Just in case we double check the bounds after conversion
    if out > MAX_DEG:
        out = MAX_DEG 
    elif out < MIN_DEG:
        out = MIN_DEG
        
    return out

# pubStatus
# Input: event (not actually used)
# Output: none
# Description: Periodically outputs the thruster values
#               This is the callback function to a rospytimer
#               (pub_timer) defined in main
#               The output rate is determined by PUB_RATE
def pubStatus(event):
    thruster = thrusterStatus(STARBOARD_THRUSTER, starboard_current)
    pub.publish(thruster)
    thruster = thrusterStatus(PORT_THRUSTER, port_current)
    pub.publish(thruster)

#   thrusterCtrl
# Input: None
# Output: None
# Description: This is the main code
#               Sets up ROS enviorment an exicutes functionality
#               as described at top of this script
def thrusterCtrl():
    global port_current
    global starboard_current
    
    #Setup ros
    rospy.init_node('thruster_control')
    rospy.Subscriber("thruster_config", thrusterConfig, motorConfigCallback)
    r = rospy.Rate(1000)          #1000 hz(1ms Period)... I think
    pub_timer = rospy.Timer(PUB_RATE, pubStatus)
    
    #Initilize the motors to 0
    stopThrusters()

    #Main loop
    while not rospy.is_shutdown():
        #Simple ramping function
        #If its less than the setpoint add RAMP_RATE
        #If its greater than the setpoint subtract RAMP_RATE
        #If we overshoot set thrust equal to the setpoint
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
        #Generate messages in the form of #,#:
        #Added : to prevent writing to messages in a row i.e. 1,234:2,65:
        ser.write(str(PORT_THRUSTER)+","+str(int(convertNewtonsToDuty(port_current)))+":")
        time.sleep(0.001)
        ser.write(str(STARBOARD_THRUSTER)+","+str(int(convertNewtonsToDuty(starboard_current)))+":")

        #Wait till next cycle
        r.sleep()
    
    #Clean up
    stopThrusters()         #Stop motors
    ser.close()             #Close serial port

if __name__ == '__main__':
    try:
        thrusterCtrl()
    except rospy.ROSInterruptException: pass
