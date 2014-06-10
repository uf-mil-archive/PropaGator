//Victor 888 

#include <Servo.h>

const int STARBOARD = 2;
const int PORT = 3;

Servo port_servo;
Servo starboard_servo;

void setup() {
  port_servo.attach(9);      //Port
  starboard_servo.attach(6); //Starboard
  Serial.begin(115200);
  starboard_servo.write(91);
  port_servo.write(91);
}


void loop() {

  if(Serial.available() > 0){

    int servoID = Serial.parseInt();
    int thrust = Serial.parseInt();
    Serial.read();
    
    //Serial.print(servoID);
    //Serial.print(" : ");
    //Serial.println(thrust);

    if(servoID == STARBOARD){
      //Serial.println("starboard_servo");
      starboard_servo.write((int)thrust);

      //delay(15);
    }
    else if (servoID == PORT){
      port_servo.write((int)thrust);
    }
    else {
      //Serial.println("Invalid servo ID");
    }
  }
}



