//Victor 888 

#include <Servo.h>

/*
 *  thruster_arduino.ino
 *    This program handels communication to and from the PropaGator computer to the arduino uno
 *    A request_ID is sent in the format of +{request_ID}_{Data}: or for request with no data {request_ID}:
 *      + is a message statrt, : is a terminator, {data} is a , seperated list, {request_ID} is an integer defined in the request_id enum below
 *    Data is returned in the format of {error_code}_{data}\n
 *      {error_code} is an int defined in the error_code enum (it should normally be OKAY), {data} is a , seperated list of values
 *    Baud rate is set to 9600 for reliablity
 *
 *    TODO:
 *      Make special and start characters into constants for easy adjustment
 *      On failiure toInt() returns 0, 0 is a valid thruster, therfore there is no way to tell if error or 0
 */

//Constants for data request
enum request_id
{
  INVALID_REQUEST = 0,
  REPORT_VOLTAGE,
  REPORT_BAUD,
  WRITE_THRUST,
  READ_THRUST
};

//Error codes
enum error_code
{
  E_OKAY = 0,
  E_INVALID_REQUEST,
  E_INVALID_THRUSTER_ID,
  E_INVALID_THRUSTER_REQUEST,
  E_INVALID_THRUST,
  E_UKNOWN_FAIL = -1
};

/*
 * State Codes
 *  keeps track of the current state
 */
enum state_code
{
  SETUP,
  WAIT_FOR_START,
  WAIT_FOR_REQUEST,
  WAIT_FOR_THRUSTER_ID,
  WAIT_FOR_THRUST
};

state_code current_state = SETUP;

//        Constants

//Constant for baudrate
const int BAUDRATE = 9600;
//Constants for voltage monitor
const int VOLTAGE_PIN = A0;

//Servo handels, Starboard is index 0 port is 1
//In ROS world Starboard is 2 and port is 3
//THe arduino node must take this into account and subtract two
//from each before sending the id to the arduino
const int NUM_SERVOS = 2;
Servo servos[NUM_SERVOS];
const int STARBOARD_ID = 0;
const int PORT_ID = 1;
int current_thruster;

// String to hold inputs
String in;

//Holds the value of the current request
int current_request;

/*
 * Write voltage
 * Analog input from VOLTAGE_PIN
 */
void WriteVoltage()
{
  //Serial.println("WriteVoltage");
  
  Serial.print(E_OKAY);
  Serial.print('_');
  Serial.println(analogRead(VOLTAGE_PIN));
  
  current_state = WAIT_FOR_START;
}

/*
 * Write baudrate
 * Writes out the baud rate
 */
void WriteBaud()
{
  //Serial.println("WriteBaud");
  
  Serial.print(E_OKAY);
  Serial.print('_');
  Serial.println(BAUDRATE);
  
  current_state = WAIT_FOR_START;
}

/*
 * Start Request
 *  Starts a request
 *  Goes to WAIT_FOR_REQUEST state
 */
void StartRequest()
{
  //Serial.println("StartRequest");
  
  current_state = WAIT_FOR_REQUEST;
}

/*
 * Request Made
 *  Switches the state appropriatly
 *  Performs appropriate function
 */
void RequestMade(long request)
{
  //Serial.println("RequestMade");
  
  current_request = request;
  switch(current_request)
  {
    case WRITE_THRUST:
      current_state = WAIT_FOR_THRUSTER_ID;//Switch to wait for thrusters id state
      break;
    case READ_THRUST:
      current_state = WAIT_FOR_THRUSTER_ID;//Switch to wait for thrusters id state
      break;
    case REPORT_VOLTAGE:
      WriteVoltage();                    //Write voltage
      break;
    case REPORT_BAUD:
      WriteBaud();                        //Write Baud
      break;
    case INVALID_REQUEST:
    default:
      Serial.println(E_INVALID_REQUEST);  //Print an error code and leave current_state as waiting for request
      break;
  }
}

/*
 * Record thruster id
 *  Records the thruster id in a global variable
 *  Reads the thruster and prints it out if we are requesting a read 
 *  Sets the state to WAIT_FOR_THRUST or WAIT_FOR_START depending on request
 */
void RecordThrusterID(long thruster)
{
  //Serial.println("RecordThrusterID");
  
  current_thruster = thruster;

  //Check for valid servo  
  if(current_thruster < 0 || current_thruster >= NUM_SERVOS)  
  {
    Serial.println(E_INVALID_THRUSTER_ID);    //print error_code
    current_state = WAIT_FOR_START;
    return;
  }
  
  //Valid servo
  if(current_request == WRITE_THRUST)    //Wait for thrust input
    current_state = WAIT_FOR_THRUST;
  else if(current_request == READ_THRUST)  //Send thrust to ROS
  {
    Serial.print(E_OKAY);
    Serial.print('_');
    Serial.println(servos[current_thruster].readMicroseconds());
    current_state = WAIT_FOR_START;
  }
  else        //Something went wrong
  {
    Serial.println(E_INVALID_THRUSTER_REQUEST);
    current_state = WAIT_FOR_START;
  }
    
    
}

/*
 * Write Thruster
 *   Sends the thrust to the appropriate thruster
 *   Switches state back to WAIT_FOR_START
 */
void WriteThruster(long thrust)
{
  //Serial.println("WriteThruster");
  
  if(thrust < 400 || thrust > 2500)
  {
    //Values must be between 400 and 2500
    Serial.println(E_INVALID_THRUST);
  }
  else
  {
    //Serial.println("Writing thruster");
    if(thrust < 1500 && servos[current_thruster].readMicroseconds() > 1500)
    {
      //Serial.println("Reverse");
      servos[current_thruster].writeMicroseconds(1500);
      //Serial.println(servos[current_thruster].readMicroseconds());
      delay(3000);
      //Serial.println("-20");
      //servos[current_thruster].writeMicroseconds(1308);
      //delay(1000);
      //Serial.println(-10);
      //servos[current_thruster].writeMicroseconds(1404);
      //delay(1000);
    }
    //Serial.println("Actual value");
    servos[current_thruster].writeMicroseconds(thrust);
    //Serial.println("Actuall Thrust");
  }
  current_state = WAIT_FOR_START;
}
 
/* 
 * This function is called on startup and reset
 *    It initilizes the thrusters to a stoped state
 *    It initilzes baudrate to 115200
 */
void setup() {
  //If we add anymore loop this
  servos[PORT_ID].attach(9);                    //Attach port to pin 9
  servos[PORT_ID].writeMicroseconds(1500);      //Zero Port
  servos[STARBOARD_ID].attach(6);               //Attach starboard to pin 6
  servos[STARBOARD_ID].writeMicroseconds(1500); //Zero Starboard
  
  Serial.begin(BAUDRATE);                                            //Maximum baud rate
  current_state = WAIT_FOR_START;                               //Switch states
}

/*
 * Main loop
 */
void loop() {

  //When Serial is avaliable we find the request id and then wait for the data
  //Data is seperated by ','s and terminated by ':'  
  //Only get new serial if we are looking for it.
  while(Serial.available() > 0 /*&& (current_state == WAIT_FOR_REQUEST || current_state == WAIT_FOR_THRUSTER_ID || current_state == WAIT_FOR_THRUST)*/)
  {
    
    //temporary values
    char current_char = Serial.read();  //Current serial input
    boolean special = false;            //special charater detected
    boolean start = false;              //Start character detected
    long data;                          //Data from input
    
    //Get the next character and see if its a special character or start character
    if(current_char == '_' || current_char == ':' || current_char == ',')
    {
      special = true;
      //This will need to be changed if we ever send anything other than ints
      data = in.toInt();
      in = "";  //Clear in
    }
    else if(current_char == '+')
    {
      start = true;
      in = "";
    }
    else //TODO: make sure its an int
    {
      //conocate the value
      in += current_char; 
    }
    
    //Take actions based on the state
    switch(current_state)
    {
    case WAIT_FOR_START:
    //Wait for start
      if(start)
        StartRequest();
      break;
    case WAIT_FOR_REQUEST:
    //Wait for a complete request
      if(special)
        RequestMade(data);    //request finished
      break;
    case WAIT_FOR_THRUSTER_ID:
    //Wait for entire ID
      if(special)
         RecordThrusterID(data);  //ID imputed
      break;
    case WAIT_FOR_THRUST:
    //Wait for entire thrust value
      if(special)
         WriteThruster(data);      //Send it out to ESC's
      break;
      
//Should never be in any other state
    default:
      current_state = WAIT_FOR_START;    //Send it back to waiting for request
      break;
    }   
  }
}


