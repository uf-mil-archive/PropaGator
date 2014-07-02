#!/usr/bin/env python

##
##                          arduino_serial_comm node
##
## Purpose:
##  This Script controls all communications with the onboard arduino Uno
##
## Configuration parameters:
##  
##
## Trouble shooting:
##

import rospy
import serial
from std_msgs.msg import String


#Publishers
#motors_pub = rospy.Publisher('arduino_thruster_status', thrusterStatus, queue_size=10)


#Define serial vars
#WARNING: you'll need permissions to access /dev/ttyACM0 file, or chmod it
#Baud rate = 57600 about as fast as the arduino will go
#Port is /dev/ttyACM0 which is the default for the arduino to attach to
#
ser = serial.Serial(baudrate=9600, timeout=0.01) 

# writeCallback
# Input: std_msgs.msg.String
# Output: None
# Descrition: Writes the string out to the arduino
def writeCallback(msg):
    ser.write(msg.data)

# pubStatus
# Input: event (not actually used)
# Output: none
# Description: Periodically outputs the thruster values
#               This is the callback function to a rospytimer
#               (motor_pub_timer) defined in main
#               The output rate is determined by PUB_RATE
#def pubStatus(event):
#    thruster = thrusterStatus(STARBOARD_THRUSTER, starboard_current)
#    pub.publish(thruster)
#    thruster = thrusterStatus(PORT_THRUSTER, port_current)
#    pub.publish(thruster)

#   arduino_serial_comm
# Input: None
# Output: None
# Description: This is the main code
#               Sets up ROS enviorment an exicutes functionality
#               as described at top of this script
def arduino_serial_comm():
    global port_current
    global starboard_current
    
    #Setup ros
    rospy.init_node('arduino_serial_comm', anonymous=True)
    rospy.Subscriber("arduino_write", String, writeCallback)
    r = rospy.Rate(200)          #1000 hz(1ms Period)... I think
    #pub_timer = rospy.Timer(PUB_RATE, pubStatus)

    #Get params
    #Port that the arduino is attached to defaults to /dev/ttyACM0
    ser.port = rospy.get_param('arduino_port', '/dev/ttyACM0') 
    
    #Setup Serial
    try:
        #Attempt to open serial port
        ser.open()
    except serial.serialutil.SerialException as e:
        #Catch serial errors (Generally dne or permission denied)
        #If error is Errno 2 (No such file or directory) we could
        #attempt to increment through ttyACM0 - to ttyACMX
        rospy.logerr("Arduino serial error: \n\t%s\n Try changing the arduino_port parameter or check permisions on %s", e.__str__(), ser.port)        
        raise(e)    #We didn't actually handel the error so send it on up the chain (this will probably just terminate the program)

    #Main loop
    while not rospy.is_shutdown():
        ser.flush()
        rospy.loginfo(ser.readline())
        #Wait till next cycle
        r.sleep()
    
    #Clean up
    ser.close()             #Close serial port

if __name__ == '__main__':
    try:
        arduino_serial_comm()
    except rospy.ROSInterruptException: pass
