#include <Servo.h>

const int BATTERY_PIN=A0;

const int NUM_SERVOS = 4;
Servo servos[NUM_SERVOS];
const int STARBOARD_ID = 2;
const int PORT_ID = 3;
//          byte| 0 1 2 3 4 5 6 7 | 8 9 10 | 11 12 13 14 15|
// messages are |-----------------|--------|---------------|
//              |      header     |   id   |      value    |
const unsigned char PACKET_SIZE=16; // there are 16 bytes in a message packet
const int HEADER_LOCATION=0; // start of the header in the message
const int HEADER_SIZE=8;
const int ID_LOCATION=8; // start of the id feild in the message
const int ID_SIZE=3; // the length of the id feild
const int VALUE_LOCATION=11;
const int VALUE_SIZE=5;

const char THRUST_CONFIG_HEADER[HEADER_SIZE]={'M','T','R','_','C','O','N','F'};// the header for a motor config (a.k.a. thrust config) message- abbrivated to 8 bytes
const char THRUST_STATUS_HEADER[HEADER_SIZE]={'M','T','R','_','S','T','A','T'};
const char BATTERY_STATUS_HEADER[HEADER_SIZE]={'B','A','T','_','S','T','A','T'};
const char ERROR_HEADER[HEADER_SIZE]={'G','E','N','_','E','R','O','R'};

const char CONFIG_ERROR[VALUE_SIZE]={'E','R','R','O','R'};

const int THRUST_ZERO=1500;
const int THRUST_FORWARD_MAX=2250;
const int THRUST_REVERSE_MAX=700;
const int THRUST_REVERSE_MIN_TURN_ON=1308;

const int BAUDRATE = 9600;

//#define DEBUG

void setup() {
  // only if the packet size is correct should we attach on the port
  if((HEADER_SIZE+ID_SIZE+VALUE_SIZE==PACKET_SIZE))
  {
    servos[PORT_ID].attach(9);                    //Attach port to pin 9
    servos[PORT_ID].writeMicroseconds(1500);      //Zero Port
    servos[STARBOARD_ID].attach(6);               //Attach starboard to pin 6
    servos[STARBOARD_ID].writeMicroseconds(1500); //Zero Starboard
  }
  
  Serial.begin(BAUDRATE);                                            //Maximum baud rate                              //Switch states
}

int atoi(char input)
{
  // We want strict control over the ascii to integer conversion.
  // Note: that there is no null in the messages that are sent
  int ret_val=0;
  if(input=='1')
  {
    ret_val=1;
  }
  else if(input=='2')
  {
    ret_val=2;
  }
  else if(input=='3')
  {
    ret_val=3;
  }
  else if(input=='4')
  {
    ret_val=4;
  }
  else if(input=='5')
  {
    ret_val=5;
  }
  else if(input=='6')
  {
    ret_val=6;
  }
  else if(input=='7')
  {
    ret_val=7;
  }
  else if(input=='8')
  {
    ret_val=8;
  }
  else if(input=='9')
  {
    ret_val=9;
  }
  return ret_val;
}
char* uitoa(int value, char array[], int percision)
{
  // we want strict control of how things are output so this is our custom unsigned integer to assci that is padded with 0's
  int base=10;
  
  if(percision<=0)
  {
    // dont touch the array
    return array;
  }
  // since the aurdino is c/c++ ish we will do this to verify the size of the arry (instead of templateing)
  
  if(value==0)
  {
    for(int i=0;i<percision;i++)
    {
      array[i]='0';
    }
    return array;
  }
  
  // Process individual digits
  // NOTE: we explicitly go until i = percision to pad the front with 0's. (instead of going until value != 0)
  for (int i=0; i < percision;i++)
  {
    int remainder = value % base;
    value = (int)(value/base);// effectively the  quotient
    array[i] = (remainder > 9)? (remainder-10) + 'A' : remainder + '0'; // this allows for base to be configurable in the future to hex
  }
  
  // note that we filled the array in reverse order so fix it.
  //      ex: value=32;
  //          value=3=32/10 remainder=array[0]=2
  //          value=0=3/10  remainder=array[1]=3
  for(int i=0;i<(int)(percision/2);i++)
  {
    // NOTE: im not doing xor tricks for clarity
    // Note again the integer division wich takes care of odd (e.g. 2n-1) and even (2n) length arrays.
    char temp=array[i];
    array[i]=array[percision-(i+1)];
    array[percision-(i+1)]=temp;
  }
  /*
  *Serial.print("received: ");
  *Serial.print(value);
  *Serial.print(" with percsision: ");
  *Serial.print(percision);
  *Serial.print(" converted to");
  *for(int i=0;i<percision;i++)
  *{
  *  Serial.print(array[i]);
  *}
  *Serial.println();
  */
  return array;
}

bool writeThrust(int input_id, int input_thrust)
{
  if(input_id!=STARBOARD_ID&&input_id!=PORT_ID)
  {
    return false;
  }
  else if(input_thrust>=THRUST_FORWARD_MAX)
  {
    return false;
  }
  else if (input_thrust<=THRUST_REVERSE_MAX)
  {
    return false;
  }
  
  int current_thrust=servos[input_id].readMicroseconds();
  if(current_thrust>=THRUST_ZERO&&input_thrust<THRUST_ZERO)
  {
    //reverse hack
    servos[input_id].writeMicroseconds(THRUST_ZERO);
    delay(3000);
    servos[input_id].writeMicroseconds(THRUST_REVERSE_MIN_TURN_ON);
    delay(1000);    
  }
  servos[input_id].writeMicroseconds(input_thrust);
  return true;
}

void loop() {
  bool new_packet_received=false;
  bool new_packet_successful=false;
  bool new_thrust_config_received=false;
  int id_to_config=0;
  int value_to_set=0;
  char bytes[PACKET_SIZE]={0};// init to 0
  char header_section[HEADER_SIZE]={'t','e','s','t'};
  char id_section[ID_SIZE]={'_','i','d'};
  char value_section[VALUE_SIZE]={0};
  
  if(Serial.available()>0)
  {
    //          byte| 0 1 2 3 4 5 6 7 | 8 9 10 | 11 12 13 14 15|
    // messages are |-----------------|--------|---------------|
    //              |      header     |   id   |      value    |
    // only read if you have an entire packet
    if(Serial.available()%PACKET_SIZE==0)
    {
      
      int num_bytes_read=Serial.readBytes(bytes,PACKET_SIZE);
      new_packet_received=true;
      // double check that it read ok
      if(num_bytes_read==PACKET_SIZE)
      {
#ifdef DEBUG
         Serial.println();
         Serial.print("New Packet: ");
         for(int i=0;i<PACKET_SIZE;i++)
         {
           Serial.print(bytes[i]);
         }
         Serial.println();
#endif
        new_packet_received=true;
        for(int i=0;i<HEADER_SIZE;i++)
        {
          header_section[i]=bytes[HEADER_LOCATION+i];
        }
        for(int i=0;i<ID_SIZE;i++)
        {
          id_section[i]=bytes[ID_LOCATION+i];
        }
        for(int i=0;i<VALUE_SIZE;i++)
        {
          value_section[i]=bytes[VALUE_LOCATION+i];
        }
        
#ifdef DEBUG
        Serial.print("ASCII PACKET HEADER: ");
        for(int i=0;i<HEADER_SIZE;i++)
        {
          Serial.print(header_section[i]);
        }
        Serial.print(" ID: ");
        for(int i=0;i<ID_SIZE;i++)
        {
          Serial.print(id_section[i]);
        }
        Serial.print(" VALUE: ");
        for(int i=0;i<VALUE_SIZE;i++)
        {
          Serial.print(value_section[i]);
        }
        Serial.println();
#endif
        
        
        
        
        // get the id from the message
        for(int i=0;i<ID_SIZE;i++)
        {
          id_to_config=id_to_config*10+atoi(id_section[i]);
        }
        // get the value to configure
        for(int i=0;i<VALUE_SIZE;i++)
        {
          value_to_set=value_to_set*10+atoi(value_section[i]);
        }
        
        // See if it's a thruster message
        bool valid_thrust_header=true;
        for(int i=0;i<HEADER_SIZE;i++)
        {
          if(header_section[i]!=THRUST_CONFIG_HEADER[i])
          {
            valid_thrust_header=false;
            break;
          }
        }
        bool valid_thruster_id=false;
        if(id_to_config==STARBOARD_ID)
        {
          valid_thruster_id=true;
        }
        else if(id_to_config==PORT_ID)
        {
          valid_thruster_id=true;
        }
        if(valid_thrust_header&&valid_thruster_id)
        {
          new_thrust_config_received=true;
#ifdef DEBUG
          Serial.print("thruster config for id: ");
          Serial.print(id_to_config);
          Serial.print(" value: ");
          Serial.println(value_to_set);
#endif
        }
        else if(valid_thrust_header==true&&valid_thruster_id==false)
        {
          new_packet_successful=false;
          for(int i=0;i<VALUE_SIZE;i++)
          {
            value_section[i]=CONFIG_ERROR[i];
          }
        }
        //check to see if it is any other messages we define in the future here
        //
        // code
        //
        
      }
      else
      {
#ifdef DEBUG
         Serial.println("read error");
#endif
        new_packet_successful=false;
        for(int i=0;i<HEADER_SIZE;i++)
        {
          header_section[i]=ERROR_HEADER[i];
        }
        for(int i=0;i<ID_SIZE;i++)
        {
          id_section[i]=0;
        }
        for(int i=VALUE_LOCATION;i<VALUE_SIZE;i++)
        {
          value_section[i]=0;
        }
      }
    }
  }
  
  
  
  // if we received a new thrust config message
  if(new_thrust_config_received==true)
  {
    if(writeThrust(id_to_config,value_to_set)==true)
    {
#ifdef DEBUG
         Serial.println("writeThrust success:");
#endif
      for(int i=0;i<HEADER_SIZE;i++)
      {
        Serial.print(header_section[i]);
      }
      for(int i=0;i<ID_SIZE;i++)
      {
        Serial.print(id_section[i]);
      }
      for(int i=0;i<VALUE_SIZE;i++)
      {
        Serial.print(value_section[i]);
      }
      new_packet_successful=true;
    }
    else
    {
#ifdef DEBUG
         Serial.println("writeThrust error");
#endif
      new_packet_successful=false;
      for(int i=0;i<VALUE_SIZE;i++)
      {
        value_section[i]=CONFIG_ERROR[i];
      }
    }
  }
  
  //send a message if the config failes  
  if(new_packet_received==true&&new_packet_successful==false)
  {
#ifdef DEBUG
         Serial.println("Packet Received but not successfull:");
#endif
    for(int i=0;i<HEADER_SIZE;i++)
    {
      Serial.print(header_section[i]);
    }
    for(int i=0;i<ID_SIZE;i++)
    {
      Serial.print(id_section[i]);
    }
    for(int i=0;i<VALUE_SIZE;i++)
    {
      Serial.print(value_section[i]);
    }
  }
  
  /*
  char status_value[VALUE_SIZE];
  char status_id[ID_SIZE];
  // now print the status messages
  // send the starboard thrust
  int current_starboard_thrust=servos[STARBOARD_ID].readMicroseconds();
  uitoa(current_starboard_thrust, status_value, VALUE_SIZE);
  uitoa(STARBOARD_ID,status_id,ID_SIZE);
  Serial.print("MTR_STAT");
  for(int i=0;i<ID_SIZE;i++)
  {
    Serial.print(status_id[i]);
    
  }
  for(int i=0;i<VALUE_SIZE;i++)
  {
    Serial.print(status_value[i]);
  }
  Serial.println();
  //send the port thrust
  int current_port_thrust=servos[PORT_ID].readMicroseconds();
  uitoa(current_port_thrust, status_value, VALUE_SIZE);
  uitoa(PORT_ID,status_id,ID_SIZE);
  Serial.print("MTR_STAT");
  for(int i=0;i<ID_SIZE;i++)
  {
    Serial.print(status_id[i]);
  }
  for(int i=0;i<VALUE_SIZE;i++)
  {
    Serial.print(status_value[i]);
  }
  Serial.println();
  //send the batery voltage
  */
  
  
  
  
  
  
  
  
  
  

}
