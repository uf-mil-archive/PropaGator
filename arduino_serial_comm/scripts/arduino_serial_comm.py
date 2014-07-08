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
## New MSG protocol
##     | Header | ID  | Data  | * |
##	   |01234567| 012 | 34567 | * |
##
##     |MTR_CONF| 002 | ##### | * | is write out to starboard
##	   |MTR_CONF| 003 | ##### | * |is write out to port
##
##	   |M
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
##
## Causes of error
##  * Unconecting serial port while node is running will cause the node to read invalid data

import rospy
import serial
from std_msgs.msg import String
from std_msgs.msg import Int32          ##Size of arduino uno long
from std_msgs.msg import Int16          ##Size of arduino int
import time
from motor_control.msg import thrusterPWM

PORT_ID = 3
STARBOARD_ID = 2

PWM_ZERO = 1500

##message protocol
HEADER_SIZE = 8;
HEADER_LOCATION = 0;
ID_SIZE = 2;
ID_LOCATION = 8;
DATA_SIZE = 5;
DATA_LOCATION = 10;
MSG_DELIM = '*'
DELIM_LOCATION = 15
MSG_SIZE = 16;

HEADER_MOTOR_CONFIG = "MTR_CONF"
HEADER_MOTOR_STATUS = "MTR_STAT"
HEADER_VOLTAGE 		= "BAT_STAT"
HEADER_ERROR		= "GEN_EROR"

DATA_ERROR = "ERROR"
waiting_for_valid_header = False;
valid_header_buffer = "Eight888*"

READ_TIMEOUT = rospy.Duration(0.500)    #Wait for 500ms durring read time
message_buffer = ""

#Keeps count of how many config msgs are in waiting
#inc every time a conf msg is sent
#dec every time a conf msg is recieved
num_config_msg = 0
#Max number of tollerable config fails
MAX_FAILED_CONFIG_MSGS = 10

#Watchdog for replies from arduino
MAX_TIME_BETWEEN_READS = rospy.Duration(1)

## Enumerators directly from the thruster_arduino.ino file
##
##Constants for data request
##enum request_id
##{
##  INVALID_REQUEST = 0,
##  REPORT_VOLTAGE,
##  REPORT_BAUD,
##  WRITE_THRUST,
##  READ_THRUSTls
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
#Maps request strings to the start or all of the required arduino message
#request_ids = {"get_voltage"         : "+1",
#               "get_baudrate"        : "+2",
#               "set_port_pwm"        : "+3_1",
#               "set_starboard_pwm"   : "+3_0",
#               "get_port_pwm"        : "+4_1",
#               "get_starboard_pwm"   : "+4_0"}
#
##Error codes
#error_codes = {0 : "Okay",
#               1 : "Invalid request",
#               2 : "Invalid thruster ID",
#               3 : "Invalid thruster request",
#               4 : "Invalid thrust",
#              -1 : "Unknown error"}

#Publishers
thruster_pwm_status_pub = rospy.Publisher('thruster_pwm_status', thrusterPWM, queue_size=10)
voltage_status_pub = rospy.Publisher('voltage_status', Int16, queue_size=10)


#Define serial vars
#WARNING: you'll need permissions to access /dev/ttyACM0 file, or chmod it
#Baud rate = 9600 about as fast as the arduino will go
#Port is /dev/ttyACM0 which is the default for the arduino to attach to
#
ser = serial.Serial(baudrate=115200, timeout=0.01)
last_publish_time = None
MAX_PUBLISH_RATE = None

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
#def GenerateArduinoMsg(request, data=None):
#    #Get request
#    try:
#        msg = request_ids[request]
#    except KeyError as e:
#        rospy.logerr("Invalid request to arduino: %s", e)
#        return
#
#    #Append data
#    if data is not None:
#        if type(data) == type([]):
#            for element in data:
#                msg += '_'
#                msg += str(element)
#        else:
#            msg += '_'
#            msg += str(data)
#    
#    #Append terminator
#    msg += ':'
#    return msg
    


#
# ReadDataFrom arduino
#
# ReadThrusterPWM
# input: request
# output: pulse_width : a int32 representing the pulse width of the servo
# Description: helper function for PubThrusterPWM
#               Reads a the servo from the arduino
#def ReadDataFromArduino(request):
#    #Write request to arduino    
#    WriteDataToArduino(request)
#   
#    response = ""
#    #Wait for a reply, stop after ten tries
#    for i in range(1,10):
#        #Wait till we can read
#        while ser.readable() == False: pass
#        #while ser.inWaiting() < 1: pass
#        response = ser.readline()   #Read in the data
#        if response != "":          #Readline gives "" on failure
#            break
#
#    #Check if we didn't succesfully read
#    if response == "":
#        rospy.logwarn("Failed to read from arduino, no reply from arduino:\n\tThis always happens on starting the node\n\tTODO: figure out why this occurs on node startup");
#        return None
#    
#    #Split the ints
#    values = response.split('_')
#    try:
#        error_code = error_codes[int(values[0])]
#    except KeyError as e:
#        rospy.logwarn("Unkown error code from arduino while reading a servo PWM: " + str(values[0]))
#        error_code = error_codes[-1];
#
#    if error_code == "Okay":
#        values.pop(0)
#        return values
#    else:
#        rospy.logwarn("Failed to read servo pwm, arduino threw error: " + error_code);
#    return None

# Read from the arduino_port
def ReadFromArduino():
    global message_buffer
    
#    print "Bytes to read: %i" % ser.inWaiting()
    if(ser.inWaiting() == 0):
	  return None
    
    #Read in only full msgs
    raw_data = ser.read(ser.inWaiting()).split(MSG_DELIM)
#    print("Read: " + str(raw_data))
    #Check for msg skew
    #This won't work if we read less than a full msg across two calls to this function
    #I assume that at least one full message read has occured
    first_msg_size = len(raw_data[0])
    if first_msg_size != MSG_SIZE - 1:
#      print("Imcomplete start msg")
	  #We didn't start reading at the begining of a msg
      #Make sure something didn't go wrong i.e. serial buffer overflow
      if len(message_buffer) + first_msg_size == MSG_SIZE - 1:
		#Data is good reconstruct msg and put it back into the list
        #It would be a good idea at this point to check if we built a good msg
        raw_data[0] = message_buffer + raw_data[0]
#        print "Repaired message: %s" % raw_data[0]
      else:
#        print "Bad data at start"
		#Something is wrong so tell the user
        rospy.logwarn("Droping bad message because the last buffer + new msg does not equal MSG size\n\tBuffer from last read: %s\n\tFirst msg from this read: %s\n\tDid the serial buffer overflow or did you just start the node?", message_buffer, raw_data[0])
        raw_data.pop(0)		#Remove bad data
    #Always pop buffer because:
    #	If it finished transmition the * at the end would have caused an additional "" added to the end of raw_data list
    #		So we need to remove it and set the buffer to "" so that we don't add anything to the next msgs
    #	If it is was somewhere in the middle of transition then we need to remove the invalid msg and put it in the buffer
    #If there was only one message and it was bad this pop won't be able to pop anything!
    if len(raw_data) !=0:
	  message_buffer = raw_data.pop()
    
    #Make sure the poped data wasn't the only thing read
    if len(raw_data) == 0:
        return None
    
    return raw_data
    
def ProcessMessages(msgs):
  global num_config_msg
  #Loop through all msgs
  for msg in msgs:
#	print(msg)
	#look for valid header
	valid_header = False
	header = msg[0:(HEADER_SIZE)]
#	print("Looking for \"%s\"") % header
	for key, action in HEADER_VALUES.items():
	  if key == header:
		valid_header = True
		data = msg[DATA_LOCATION:DATA_LOCATION+DATA_SIZE]
#		print "Found a \"%s\" msg with id %s and data %s" % (header, msg[ID_LOCATION:ID_LOCATION+ID_SIZE], data) 
		if data != DATA_ERROR:
		  action(msg[ID_LOCATION:ID_LOCATION+ID_SIZE], msg[DATA_LOCATION:DATA_LOCATION+DATA_SIZE])
		else:
		  rospy.logerr("Recieved data error from arduino: %s", msg)
		  num_config_msg -= 1
		break
	if not valid_header:
	  rospy.logerr("Recieved a bad header from arduino: %s", msg)

def MsgMtrConfig(id, thrust):
  global num_config_msg
  num_config_msg -= 1
#  print(num_config_msg)
  if num_config_msg < 0:
	rospy.logerr("Recieved more thrust configuration confirmations than sent thrust configuration msgs by %i", num_config_msg * -1)

def MsgError(id, error):
  rospy.logerr("Arduino threw a general error: ID: %s, Error: ", id, error)
# PubThrusterPWM
# Input: event (not actually used)
# Output: none
# Description: Periodically outputs the thruster values
#               This is the callback function to a rospytimer
#               (thruster_pub_timer) defined in main
#               The output rate is determined by thruster_pwm_status_publish_rate
#def PubThrusterPWM():
#    #Request port PWM
#    msg = thrusterPWM();
#    data = ReadDataFromArduino(GenerateArduinoMsg("get_port_pwm"))
#    if data == None:
#        return
#    msg.id = 3;
#    msg.pulse_width = int(data[0])      #Do some error checking l8r
#    thruster_pwm_status_pub.publish(msg)
#
#    data = ReadDataFromArduino(GenerateArduinoMsg("get_starboard_pwm"))
#    if data == None:
#        return
#    msg.id = 2;
#    msg.pulse_width = int(data[0])      #Do some error checking l8r
#    thruster_pwm_status_pub.publish(msg)

def PubThrusterPWM(id, thrust):
  msg = thrusterPWM(int(id), int(thrust))
  thruster_pwm_status_pub.publish(msg)

# PubVoltage
# Input: event (not actually used)
# Output: none
# Description: Periodically outputs the voltage values
#               This is the callback function to a rospytimer
#               (voltage_pub_timer) defined in main
#               The output rate is determined by voltage_status_publish_rate
#def PubVoltage():
#    msg = Int16();
#    data = ReadDataFromArduino(GenerateArduinoMsg("get_voltage"))
#    if data == None:
#        return
#    msg.data = int(data[0])
#    voltage_status_pub.publish(msg)

def PubVoltage(id, voltage):
    msg = Int16(int(voltage))
    voltage_status_pub.publish(msg)
    
HEADER_VALUES = {HEADER_MOTOR_CONFIG	:	MsgMtrConfig,
				 HEADER_MOTOR_STATUS	:	PubThrusterPWM,
				 HEADER_VOLTAGE			:	PubVoltage,
				 HEADER_ERROR			:	MsgError}

def WriteDataToArduino(cmd):
    #Wait to write
    #Write the request
    print("Writing: " + cmd)
    ser.write(cmd)
#    time.sleep(0.07)		#As fast as I can let it go without it occuring massive amounts of bad data


def WriteThruster(id, pulse_width):
    global num_config_msg
    #Make sure pulse width is valid
    assert 0 <= pulse_width < 100000
    #Format msg
    cmd = '%s%02i%05i*' % (HEADER_MOTOR_CONFIG, id, pulse_width)    
    #Write
    #Wait till I can write
    print("Writing: %s" % cmd)
    WriteDataToArduino(cmd)
    #inc num_config_msg written
    num_config_msg += 1
    

    
#    if msg.id == 2:
    #    cmd = GenerateArduinoMsg("set_starboard_pwm", int(msg.pulse_width))
    
   # elif msg.id == 3:
    #    cmd = GenerateArduinoMsg("set_port_pwm", int(msg.pulse_width))
    #else:
    #    rospy.logerr("Invalid thruster ID in arduino_serial_comm node: " + str(msg.id))
    #    return

#    WriteDataToArduino(cmd)


thrusters = {}

#
#   arduino_serial_comm
# Input: None
# Output: None
# Description: This is the main code
#               Sets up ROS enviorment an exicutes functionality
#               as described at top of this script
def arduino_serial_comm():  
    global waiting_for_mtr_config_handshake  
    #Setup ros
    rospy.init_node('arduino_serial_comm', anonymous=True)
    r = rospy.Rate(20)          #Length of time per cycle
#
#    ##Get params
    #Port that the arduino is attached to defaults to /dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_55332333130351803192-if00
    ser.port = rospy.get_param('~arduino_port', '/dev/serial/by-id/usb'
		 '-Arduino__www.arduino.cc__0043_55332333130351803192-if00')
#
 #   
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
        rospy.logerr("Arduino serial error: \n\t%s\n Try changing the arduino_port" 
					 "parameter or check permisions on %s", e.__str__(), ser.port)        
        raise(e)    #We didn't actually handel the error so send it on up the chain (this will probably just terminate the program)
        

    def got_message(msg):
        thrusters[msg.id] = msg.pulse_width
    rospy.Subscriber("thruster_pwm_config", thrusterPWM, got_message, queue_size = 100)
    
	#Setup some buffers
    last_voltage = 0;
    last_port_pwm = PWM_ZERO;
    last_starboard_pwm = PWM_ZERO;
    
    #Zero motors on start up
    thrusters = {
      2: PWM_ZERO,
      3: PWM_ZERO,
    }

    last_recieve_time = rospy.get_rostime()    

#    #Main loop
    while not rospy.is_shutdown():
        #Watchdog timer
        time_since_last_recieve = rospy.get_rostime() - last_recieve_time
        if time_since_last_recieve > MAX_TIME_BETWEEN_READS:
            rospy.logerr("Arduino hasn't responded in %i seconds. Attepmting to close and reopen port", time_since_last_recieve.to_sec())
            ser.close()
            ser.open()
            #Wait for MAX_TIME_BETWEEN_READS
            last_recieve_time = rospy.get_rostime()   
       
        #Write thruster values
        for id_, pulse_width in dict(thrusters).iteritems():
            WriteThruster(id_, pulse_width)

        #Read and process messages
        msgs = ReadFromArduino()
        print(msgs)
        if msgs != None:
           #reset watchdog
           last_recieve_time = rospy.get_rostime()

           ProcessMessages(msgs)
        
        #Watch dog for droped mtr_conf messages
        #Currently this always drops
        #if num_config_msg > MAX_FAILED_CONFIG_MSGS:
           #rospy.logerr("Arduino is not has droped %i mtr configuration msgs", num_config_msg)
        
        #Use up the rest of the loop time
        r.sleep()
           
    #Clean up
    ser.close()             #Close serial port
#
if __name__ == '__main__':
    try:
        arduino_serial_comm()
    except rospy.ROSInterruptException: pass
