//Victor 888 

#include <Servo.h>

Servo servo1;
Servo servo2;

int servoID,thrust;

void setup() {
 servo3.attach(9);  //Port
 servo2.attach(6); //Starboard
 Serial.begin(9600);
 Serial.println("Enter servo# (2 or 3) and thrust (45-145): ");
}

void loop() {
  
  if(Serial.available() > 0){
    
    servoID = Serial.parseInt();
    thrust = Serial.parseInt();
    Serial.read();
    
    Serial.print("Servo ID: ");
    Serial.println(servoID);
    
    Serial.print("thrust: ");
    Serial.println(thrust);
    
   if(servoID == 2){
     servo2.write(thrust);
     delay(15);
   }
   else if (servoID == 3){
     servo3.write(thrust);
     delay(15);
   }
   else {
     Serial.println("Invalid servo ID");
   }
   
   

   }
   
   
   
    
  }

