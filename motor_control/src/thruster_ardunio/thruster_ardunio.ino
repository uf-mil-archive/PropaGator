//Victor 888 

#include <Servo.h>

//Constants for motor configuration
const int STARBOARD_ID = 2;
const int PORT_ID = 3;

//Constants for voltage monitor
const int VOLTAGE_PIN = A0;

//Constant publish rate ms
const unsigned long WRITE_RATE = 100;
unsigned long previous_time = 0;

Servo port_servo;
Servo starboard_servo;

void setup() {
  port_servo.attach(9);      //Port
  starboard_servo.attach(6); //Starboard
  Serial.begin(115200);      //Maximum baud rate
  starboard_servo.write(91); //Go to stop on start
  port_servo.write(91);      //Go to stop on start
  previous_time = millis();
}


void loop() {
  //Update time
  unsigned long current_time = millis();
  if(current_time - previous_time > WRITE_RATE)
  {
    /* Read the voltage of the batteries and write it out
    Serial.print("Voltage:");
    Serial.println(analogRead(VOLTAGE_PIN));
    
    //Read the servos
    Serial.print("Thruster ID:");
    Serial.print(STARBOARD_ID);
    Serial.print("\tValue:");
    Serial.println(starboard_servo.read());
    
    Serial.print("Thruster ID:");
    Serial.print(PORT_ID);
    Serial.print("\tValue:");
    Serial.println(port_servo.read());
    */
    //Reset time
    previous_time = millis();
    
  }
  
    //Wait for serial to be avaliable
  if(Serial.available() > 0){
    //Messages will be in the format of #,#:
    int servoID = Serial.parseInt();        //Get Servo ID
    int thrust = Serial.parseInt();         //Get thrust
    Serial.read();                          //Remove :
    
    
    if(servoID == STARBOARD_ID){
      starboard_servo.write((int)thrust);   //Write the thrust out
  }
    else if (servoID == PORT_ID){
      port_servo.write((int)thrust);        //Write the thrust out
    }
    //else {
      //Serial.println("Invalid servo ID");
    //}
  }
}
