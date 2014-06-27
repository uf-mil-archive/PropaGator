//Victor 888 

#include <Servo.h>

//Constants for motor configuration
const int STARBOARD = 2;
const int PORT = 3;

Servo port_servo;
Servo starboard_servo;

void setup() {
  port_servo.attach(9);      //Port
  starboard_servo.attach(6); //Starboard
  Serial.begin(115200);      //Maximum baud rate
  starboard_servo.write(91); //Go to stop on start
  port_servo.write(91);      //Go to stop on start
}


void loop() {

    //Wait for serial to be avaliable
  if(Serial.available() > 0){
    //Messages will be in the format of #,#:
    int servoID = Serial.parseInt();        //Get Servo ID
    int thrust = Serial.parseInt();         //Get thrust
    Serial.read();                          //Remove :

    if(servoID == STARBOARD){
      starboard_servo.write((int)thrust);   //Write the thrust out
    }
    else if (servoID == PORT){
      port_servo.write((int)thrust);        //Write the thrust out
    }
    //else {
      //Serial.println("Invalid servo ID");
    //}
  }
}
