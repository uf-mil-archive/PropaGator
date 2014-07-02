#!/usr/bin/env python

##
##                          arduino_serial_comm node
##
## Purpose:
##  This Script controls all communications with the onboard arduino Uno
##  A request_ID is sent in the format of +{request_ID}_{Data}: or for request with no data {request_ID}:
##      + is a message statrt, : is a terminator, {data} is a , seperated list, {request_ID} is an integer defined in the request_id enum below
##  Data is returned in the format of {error_code}_{data}\n
##      {error_code} is an int defined in the error_code enum (it should normally be OKAY), {data} is a , seperated list of values
##  Baud rate is set to 9600 for reliablity
##
## Configuration parameters:
##  voltage_status_publish_rate, default = 1Hz, purpose controls the rate at which data is published on the voltage_staus topic
##  thruster_pwm_status_publish_rate, default = 10Hz, purpose controls the rate at which data is published on the thruster_pwm_status topic
##  arduino_port, default = /dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_55332333130351803192-if00,
##      Purpose to define the connected device (if we use a different arduino this needs to change)
## 
## Warnings:
##  Timers suck they introduce more threading!!!
##
## Trouble shooting:
##
##
## TODO:
##  *           Item                        Status
##  * Add publish rate parameters           Done    
##  * Set up publish timers                 Done
##  * Make build msg function               Done
##  * Publish the data                      Done
##  * Make subscriber to thruster_config    Done
##  * Set up CMake for msgs                 Done
##  * Set up XML for msgs                   Done
##  * Add dictionaries for                  Done
##      * Errors                            Done
##      * Data request                      Done
##  * Add error ouputs/warnings
##  * Get servo id's from rosparam      
##

import rospy
import serial
from std_msgs.msg import String
from std_msgs.msg import Int32          ##Size of arduino uno long
from std_msgs.msg import Int16          ##Size of arduino int
from motor_control.msg import thrusterPWM

## Enumerators directly from the thruster_arduino.ino file
##
##Constants for data request
##enum request_id
##{
##  INVALID_REQUEST = 0,
##  REPORT_VOLTAGE,
##  REPORT_BAUD,
##  WRITE_THRUST,
##  READ_THRUST
##};
##
##//Error codes
##enum error_code
##{
##  E_OKAY = 0,
##  E_INVALID_REQUEST,
##  E_INVALID_THRUSTER_ID,
##  E_INVALID_THRUSTER_REQUEST,
##  E_INVALID_THRUST,
##  E_UKNOWN_FAIL = -1
##};
##
## Python implimentation
#request id's
#Maps request strings to the start or all of the required arduino mesage
request_ids = {"get_voltage"         : "+1",
               "get_baudrate"        : "+2",
               "set_port_pwm"        : "+3_1",
               "set_starboard_pwm"   : "+3_0",
               "get_port_pwm"        : "+4_1",
               "get_starboard_pwm"   : "+4_0"}

#Error codes
error_codes = {0 : "Okay",
               1 : "Invalid request",
               2 : "Invalid thruster ID",
               3 : "Invalid thruster request",
               4 : "Invalid thrust",
              -1 : "Unknown error"}

#Publishers
thruster_pwm_status_pub = rospy.Publisher('thruster_pwm_status', thrusterPWM, queue_size=10)
voltage_status_pub = rospy.Publisher('voltage_status', Int16, queue_size=10)


#Define serial vars
#WARNING: you'll need permissions to access /dev/ttyACM0 file, or chmod it
#Baud rate = 9600 about as fast as the arduino will go
#Port is /dev/ttyACM0 which is the default for the arduino to attach to
#
ser = serial.Serial(baudrate=9600, timeout=0.01) 

# Generate arduino msgs
# input: request : a request_ids key
#        data    : an array of data to write out
# Output: A string formated for output to the arduino output
#           On error the string is ""
# Description: Formats a msg that the arduino will understand,
#               it uses the request_ids dictionary for most applications
#               but it will apend the additional data in the second argument
#               The msg format is defined at the top of this script and in the
#               ino file
def GenerateArduinoMsg(request, data=None):
    #Get request
    try:
        msg = request_ids[request]
    except KeyError as e:
        rospy.logerr("Invalid request to arduino: %s", e)
        return

    #Append data
    if data is not None:
        if type(data) == type([]):
            for element in data:
                msg += '_'
                msg += str(element)
        else:
            msg += '_'
            msg += str(data)
    
    #Append terminator
    msg += ':'
    return msg
    

# PubThrusterPWM
# Input: event (not actually used)
# Output: none
# Description: Periodically outputs the thruster values
#               This is the callback function to a rospytimer
#               (thruster_pub_timer) defined in main
#               The output rate is determined by thruster_pwm_status_publish_rate
def PubThrusterPWM():
    #Request port PWM
    msg = thrusterPWM();
    data = ReadDataFromArduino(GenerateArduinoMsg("get_port_pwm"))
    if data == None:
        return
    msg.id = 3;
    msg.pulse_width = int(data[0])      #Do some error checking l8r
    thruster_pwm_status_pub.publish(msg)

    data = ReadDataFromArduino(GenerateArduinoMsg("get_starboard_pwm"))
    if data == None:
        return
    msg.id = 2;
    msg.pulse_width = int(data[0])      #Do some error checking l8r
    thruster_pwm_status_pub.publish(msg)


# PubVoltage
# Input: event (not actually used)
# Output: none
# Description: Periodically outputs the voltage values
#               This is the callback function to a rospytimer
#               (voltage_pub_timer) defined in main
#               The output rate is determined by voltage_status_publish_rate
def PubVoltage():
    msg = Int16();
    data = ReadDataFromArduino(GenerateArduinoMsg("get_voltage"))
    if data == None:
        return
    msg.data = int(data[0])
    voltage_status_pub.publish(msg)

#
# ReadDataFrom arduino
#
# ReadThrusterPWM
# input: request
# output: pulse_width : a int32 representing the pulse width of the servo
# Description: helper function for PubThrusterPWM
#               Reads a the servo from the arduino
def ReadDataFromArduino(request):
    #Write request to arduino    
    WriteDataToArduino(request)
   
    response = ""
    #Wait for a reply, stop after ten tries
    for i in range(1,10):
        #Wait till we can read
        while ser.readable() == False: pass
        #while ser.inWaiting() < 1: pass
        response = ser.readline()   #Read in the data
        if response != "":          #Readline gives "" on failure
            break

    #Check if we didn't succesfully read
    if response == "":
        rospy.logwarn("Failed to read from arduino, no reply from arduino:\n\tThis always happens on starting the node\n\tTODO: figure out why this occurs on node startup");
        return None
    
    #Split the ints
    values = response.split('_')
    try:
        error_code = error_codes[int(values[0])]
    except KeyError as e:
        rospy.logwarn("Unkown error code from arduino while reading a servo PWM: " + str(values[0]))
        error_code = error_codes[-1];

    if error_code == "Okay":
        values.pop(0)
        return values
    else:
        rospy.logwarn("Failed to read servo pwm, arduino threw error: " + error_code);
    return None

def WriteDataToArduino(cmd):
    #Wait to write
    while ser.writable() == False: pass
    #Write the request
    ser.write(cmd)

def WriteThruster(msg):
    cmd = ""
    if msg.id == 2:
        cmd = GenerateArduinoMsg("set_starboard_pwm", int(msg.pulse_width))
    elif msg.id == 3:
        cmd = GenerateArduinoMsg("set_port_pwm", int(msg.pulse_width))
    else:
        rospy.logerr("Invalid thruster ID in arduino_serial_comm node: " + str(msg.id))
        return

    WriteDataToArduino(cmd)


#
#   arduino_serial_comm
# Input: None
# Output: None
# Description: This is the main code
#               Sets up ROS enviorment an exicutes functionality
#               as described at top of this script
def arduino_serial_comm():    
    #Setup ros
    rospy.init_node('arduino_serial_comm', anonymous=True)
    rospy.Subscriber("thruster_pwm_config", thrusterPWM, WriteThruster)
    r = rospy.Rate(100)          #Length of time per cycle

    ##Get params
    #Port that the arduino is attached to defaults to /dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_55332333130351803192-if00
    ser.port = rospy.get_param('~arduino_port', '/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_55332333130351803192-if00')

    
    ##Timing vars
    #Publish rate for voltage status default is 1Hz    
    pub_freq = rospy.get_param('~voltage_status_publish_rate', 1);
    voltage_pub_period = rospy.Duration(pub_freq**-1)
    voltage_pub_last_time = rospy.get_rostime()
    #Publish rate for thruster status default is 10Hz
    pub_freq = rospy.get_param('~thruster_pwm_status_publish_rate', 10);
    thruster_pub_period = rospy.Duration(pub_freq**-1)
    thruster_pub_last_time = rospy.get_rostime()
    
    
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
        now = rospy.get_rostime()
        if (now - voltage_pub_last_time) > voltage_pub_period:
            #reset time
            voltage_pub_last_time = now
            #Write out voltage
            PubVoltage()

        if (now - thruster_pub_last_time) > thruster_pub_period:
            #reset time
            thruster_pub_last_time = now
            #Write out thrust
            PubThrusterPWM()
        r.sleep()
    
    #Clean up
    ser.close()             #Close serial port

if __name__ == '__main__':
    try:
        arduino_serial_comm()
    except rospy.ROSInterruptException: pass
