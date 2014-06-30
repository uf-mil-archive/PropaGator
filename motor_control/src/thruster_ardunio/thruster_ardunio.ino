//Victor 888 

#include <Servo.h>

//Constants for motor configuration
const int STARBOARD_PIN = 2;
const int PORT_PIN = 3;

//Constants for voltage monitor
const int VOLTAGE_PIN = A0;

Servo port_servo;
Servo starboard_servo;

void setup() {
  port_servo.attach(9);      //Port
  starboard_servo.attach(6); //Starboard
  Serial.begin(9600);      //Maximum baud rate
  starboard_servo.write(91); //Go to stop on start
  port_servo.write(91);      //Go to stop on start
}


void loop() {
  
  // Read the voltage of the batteries and write it out
  Serial.println(analogRead(VOLTAGE_PIN));
  
    //Wait for serial to be avaliable
  if(Serial.available() > 0){
    //Messages will be in the format of #,#:
    int servoID = Serial.parseInt();        //Get Servo ID
    int thrust = Serial.parseInt();         //Get thrust
    Serial.read();                          //Remove :

    if(servoID == STARBOARD_PIN){
      starboard_servo.write((int)thrust);   //Write the thrust out
    }
    else if (servoID == PORT_PIN){
      port_servo.write((int)thrust);        //Write the thrust out
    }
    //else {
      //Serial.println("Invalid servo ID");
    //}
  }
}
