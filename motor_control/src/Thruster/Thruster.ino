//Victor 888 

#include <Servo.h>

const int STARBOARD = 2;
const int PORT = 3;

Servo port_servo;
Servo starboard_servo;

float port_setpoint = 91;
float starboard_setpoint = 91;

float port_current = 91;
float starboard_current = 91;

const float RAMP_RATE = 0.90;  //Degrees per ms

int servoID,thrust=91;

void setup() {
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);  
  port_servo.attach(9);  //Port
  starboard_servo.attach(6); //Starboard
  Serial.begin(9600);
  starboard_servo.write(thrust);
  port_servo.write(thrust);
  //Serial.println("Enter servo# (2 or 3) and thrust (45-145): ");
}


void loop() {

    if(Serial.available() > 0){

      servoID = Serial.parseInt();
      thrust = Serial.parseInt();
      Serial.read();

      /*Serial.print("Servo ID: ");
       //Serial.println(servoID);
       
       //Serial.print("thrust: ");
       //Serial.println(thrust);
       starboard_servo.write(110);
       delay(40);
       starboard_servo.write(145);
       delay(2000);
       starboard_servo.write(91);
       delay(40);
       starboard_servo.write();
       delay(40);
       
       starboard_servo.write(45);
       delay(1000);
       starboard_servo.write(91);
       delay(1000);
       */

      if(servoID == STARBOARD){
        //Serial.println("starboard_servo");
        //starboard_servo.write(thrust);
         starboard_setpoint = thrust;

        //delay(15);
      }
      else if (servoID == PORT){
        //port_servo.write(thrust);
        port_setpoint = thrust;

        //delay(15);
      }
      else {
        //Serial.println("Invalid servo ID");
      }



    }

    //Will ossilate...
    starboard_current += RAMP_RATE * (starboard_current > starboard_setpoint ? -1 : 1);
    port_current += RAMP_RATE * (port_current > port_setpoint ? -1 : 1);

    starboard_servo.write((int)starboard_current);
    port_servo.write((int)port_current);
    delay(1);

  }


