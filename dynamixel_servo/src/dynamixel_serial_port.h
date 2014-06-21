/*
 * dynamixel_serial_port.h
 *
 *  Created on: Mar 7, 2014
 *      Author: stephen palecek
 */

#ifndef DYNAMIXEL_SERIAL_PORT_H_
#define DYNAMIXEL_SERIAL_PORT_H_

#include <stdio.h>//used for stderr
#include <unistd.h>//close, write, read uses this
#include <fcntl.h>//open uses this
#include <termios.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <sys/time.h>//gettimeofday uses this
#include <math.h>
#include <vector>
#include <string>

using namespace std;

#define DYNAMIXEL_SERIAL_PORT_DEBUG

static inline long getCurrentTime()
{
	// time value struct specified in sys/time.h. see: http://linux.die.net/man/2/gettimeofday
	struct timeval time_val;
	// get the systems time struct with no specific time zone specified (hence the NULL)
	gettimeofday (&time_val, NULL);
	// note: tv_sec and tv_usec are defined as __SYSCALL_SLONG_TYPE (and hence may be system specific)
	return (time_val.tv_sec * 1000 + time_val.tv_usec / 1000);
}

// Note: fprintf with stderr is used by this class specifically so that if any errors occur they can be redirected if so chosen.
class DynamixelSerialPort
{
public:
	// These constants are only defined above the standard public information because they are used in declaring the size of private variables
	//------------- Max number of parameters that a message can have Constants --------------
	static const uint8_t MAXNUM_NUM_TX_PARAMS=150; // maximum number of parameters that can be in a packet sent to the servo(s)
	static const uint8_t MAXNUM_NUM_RX_PARAMS=60; // maximum number of parameters that can be in a returned packet from the servo

protected:
	string port_name;//the port name on the the linux file system ex: /dev/ttyUSB3
	float baud_rate;//the baud rate that the port will use
	bool using_bus; // this is used in the Dynamixel sdk to indicate if the Dynamixel BUS is in use by a tx or rx command (since the port is specified as non blocking)
	int comm_status; // this is used in the Dynamixel sdk to indicate the state of the com port
	// These are constant parameters specified in the Dynamixel sdk for its tx/rx functions
	//----------- Com Port Status Flags ----------------
	static const int COMM_TXSUCCESS=0;
	static const int COMM_RXSUCCESS=1;
	static const int COMM_TXFAIL=2;
	static const int COMM_RXFAIL=3;
	static const int COMM_TXERROR=4;
	static const int COMM_RXWAITING=5;
	static const int COMM_RXTIMEOUT=6;
	static const int COMM_RXCORRUPT=7;
	static const int COMM_PORTCLOSED=8;
	//------------- Index specifiers for a given field in a packet ------------------------
	// Note: packets are represented as follows 0xFF,0xFF,ID,LENGTH, INSTRUCTION or ERRBIT, PARAMETER 1, ... , PARAMETER N, CHECKSUM
	static const uint8_t ID=2; // the value in the packet[2] indicates the id of the servo to talk with
	static const uint8_t LENGTH=3; // the value in the packet[3] indicates the number of bytes in the packet following this field. (so Instruction + N parameters + checksum)
	static const uint8_t INSTRUCTION=4; // for a instruction_packet the instruction code sits in instruction_packet[4]
	static const uint8_t ERRBIT=4; // for a status_packet the location of the error bits that might be set is status_packet[4]
	static const uint8_t PARAMETER=5; // Note: that for both the instruction_packet and status_packet the parameters of the packet start at index 5
	//------------ Default Return Delay Time -------------------------------------
	// NOTE: THIS VALUE IS ASSUMED TO BE THE DEFAULT RETURN DELAYTIME OF 0.5 msec SO THE FIRST READ/WRITE CAN OCCUR.
	// CHANGE THIS CONSTANT IF THE EEPROM ADRESS 0X05 IS NOT THE DEFAULT VALUE OF 0XFA.
	// See: http://support.robotis.com/en/product/dynamixel/mx_series/mx-64.htm#Actuator_Address_05
	static const float RETURN_DELAY_TIME=5.0f;

private:
	int socket_fd;//socket file descriptor that is used to talk on the serial port
	long transfer_start_time; // the time when the transfer started
	float byte_transfer_time; // the time it takes to transfer a byte at a given baud rate; it's used for timeout values
	float receive_wait_time; // how long to wait when receiving bytes
	// Note: The +10 ensures that enough space is cut out. Remember a packet is 0xFF,0xFF,ID,LENGTH, INSTRUCTION, PARAMETER 1, ... , PARAMETER N, CHECKSUM
	uint8_t instruction_packet[MAXNUM_NUM_TX_PARAMS+10]; // memory cut out to hold an instruction packet that can be sent to a servo
	uint8_t status_packet[MAXNUM_NUM_RX_PARAMS+10]; // memory cut out to hold a reply packet received from a servo
	// Note: receive_packet_total_length and num_packets_currently_received will be used in coordination with transfer_start_time and receive_wait_time to tell if there is any corruption/timeout
	uint8_t receive_packet_total_length; // the total number of bytes to be received for a given packet sent from a servo
	uint8_t num_packets_currently_received; // this is the number of packets that have currently been recived on the port
	template <typename T, unsigned int passed_array_size>
	unsigned int getChecksumLocation(const T (&packetArray) [passed_array_size]) const;
	template <typename T, unsigned int passed_array_size>
	string printPacket(const T (&packetArray) [passed_array_size]) const;
	template <typename T, unsigned int passed_array_size>
	string getStatusPacketErrors(const T (&packetArray) [passed_array_size]) const;
	void flushComPort(){tcflush(socket_fd, TCIFLUSH);};
	void setTimeOut(int number_bytes_to_receive);
	bool hasTimedOut();
	bool sendPacket();
	bool receivePacket();

public:
	DynamixelSerialPort();
	void setPortName(string input_name);
	void setBaudRate(uint8_t input_baud);
	bool openPort();
	void closePort();
	bool isOpen();
	string getPortName(){return port_name;};
	template <typename T, unsigned int passed_array_size>
	T calcPacketChecksum(const T (&packetArray) [passed_array_size]) const; // This is a templated function to allow for some magic
	bool createPacket(uint8_t id, uint8_t instruction_code, vector<uint8_t> paramaters);
	string sendAndReceive(uint8_t id, uint8_t instruction_code, vector<uint8_t> paramaters = std::vector<uint8_t>());
	string printSendPacket()const{return printPacket(instruction_packet);};
	string printReceivePacket()const{return printPacket(status_packet);};
	string printComStatus() const;
	string printInstructionType(const uint8_t &instruction_code) const;
	vector<uint8_t> getReturnPacketParamaters() const;
	bool comErrorPresent() const;
	//------------- Available Instruction Codes at instruction_packet[INSTRUCTION] --------------------
	static const uint8_t INST_PING=0x01; // Used for obtaining a Status Packet
	static const uint8_t INST_READ=0x02; // Reading values in the Control Table
	static const uint8_t INST_WRITE=0x03; // Writing values to the Control Table
	static const uint8_t INST_REG_WRITE=0x04; // Similar to WRITE_DATA, but the servo stays in standby mode until the ACION instruction is given
	static const uint8_t INST_ACTION=0x05; // Triggers the action registered by the REG_WRITE instruction
	static const uint8_t INST_RESET=0x06; // Changes the control table values of the Dynamixel servo to the Factory Default Value settings
	static const uint8_t INST_SYNC_WRITE=0x83; // Used for controlling many Dynamixel actuators at the same time
	static const uint8_t INST_DIGITAL_RESET=0x07; // Included for consistency with the sdk
	static const uint8_t INST_SYSTEM_READ=0x0C; // Included for consistency with the sdk
	static const uint8_t INST_SYSTEM_WRITE=0x0D; // Included for consistency with the sdk
	static const uint8_t INST_SYNC_REG_WRITE=0x84; // Included for consistency with the sdk
	//------------ Error Bit Constants -----------------
	static const uint8_t ERRBIT_VOLTAGE=0b00000001;
	static const uint8_t ERRBIT_ANGLE=0b00000010;
	static const uint8_t ERRBIT_OVERHEAT=0b00000100;
	static const uint8_t ERRBIT_RANGE=0b00001000;
	static const uint8_t ERRBIT_CHECKSUM=0b00010000;
	static const uint8_t ERRBIT_OVERLOAD=0b00100000;
	static const uint8_t ERRBIT_INSTRUCTION=0b01000000;
	//------------ Brodcast ID ---------------------------------------------------
	// The valid id(s) for a servo are between [0-252 (0xFC)]. The broadcast id is 0xFE (254); a message sent to this id will be sent to all the servo(s)
	static const uint8_t BROADCAST_ID=254;
	//------------ Default Parameters related to the serial ports operation ---------------------------------------------------
	static const string DEFAULT_SERIAL_PORT;
	static const uint8_t DEFAULT_BAUD_RATE=0x22; //this it the default baud rate of 57600 bps
	static const string DYNAMIXEL_COMUNICATION_SUCCESSFUL;
};
// A string is not an integral type and hence it must be defined outside the class.
const string DynamixelSerialPort::DEFAULT_SERIAL_PORT="/dev/serial/by-id/usb-FTDI_FT4232H_Device_FTWZVMOA-if03-port0";
const string DynamixelSerialPort::DYNAMIXEL_COMUNICATION_SUCCESSFUL="SUCCESS";

DynamixelSerialPort::DynamixelSerialPort()
{
        port_name="";
        // the min value that can be set per the documentation (data value is in between [0-249])
        // ex: baud_rate = 2000000/(Data + 1) = 2000000/(249+1)= 2000000/250=8000
        baud_rate=8000;
        // again per the documentation the corresponding byte_transfer_time(see setBaudRate for more info)
        byte_transfer_time=(float)((1000.0f / baud_rate) * 12.0f);
        comm_status=COMM_TXERROR;
        using_bus=false; // the bus is not in use yet
        socket_fd=-1;//note: -1 is used by many POSIX function to indicate a closed file handle
        transfer_start_time=0; //default value per the sdk in dxl_hal.c
        byte_transfer_time=0.0f; //default value per the sdk in dxl_hal.c
        receive_wait_time=0.0f; //default value per the sdk in dxl_hal.c
        // Note: an instruction_packet consists of uint8_t(s) which are bytes, and hence memset is (compiler) optimal. Remember things like integers occupy more than 1 byte (so don't use memset on them).
        // Note: sizeof() operator gives you the number of bytes a thing occupies in memory.
        memset(instruction_packet, 0, sizeof(instruction_packet)); // for details see: http://www.cplusplus.com/reference/cstring/memset/
        memset(status_packet, 0, sizeof(status_packet));
        receive_packet_total_length=0;
        num_packets_currently_received=0;
}

void DynamixelSerialPort::setPortName(string input_port_name)
{
        if(port_name==input_port_name)
        {
                //do nothing its the same port already specified, so we don't have to worry about any possible open file handles.
        }
        else if(socket_fd==-1)// the port/file handle is closed
        {
                port_name=input_port_name;//while technically slower it is easier to deal with a string instead of a char*
        }
        else //there must be no open file handles (i.e. connections on the port) before we try to switch to a new port
        {
                fprintf(stderr, "Port %s must be closed first before we switch to port %s; Once this is done call openPort to establish a connection on the new port.\n", port_name.c_str(), input_port_name.c_str());
        }
};

// Note: the input_baud is assumed to be valid and in the range of 0-249
void DynamixelSerialPort::setBaudRate(uint8_t input_baud_rate)
{
        if(socket_fd==-1)// the port is closed
        {
                // per the documentation the baud rate is calculated as follows for a given input_baud
                baud_rate=2000000.0f / (float)(input_baud_rate + 1);
                // per the Dynamixel SDK's C code (in dxl_hal.c) for the function dxl_hal_set_timeout
                // http://support.robotis.com/en/software/dynamixel_sdk/porting/dxl_hal_timeout.htm
                byte_transfer_time=(float)((1000.0f / baud_rate) * 12.0f);
        }
        else //the port must be closed first to set the baud rate on it
        {
                fprintf(stderr, "Port %s must be closed before the baud can be reset; Once this is done call openPort to establish a connection at the desired baud.\n", port_name.c_str());
        }
}

// This function returns true if it can acquire the handle on the com port, false otherwise
// Note: The port_name and baud_rate are expected to be set before calling this function. Also, any previously open ports will be closed
bool DynamixelSerialPort::openPort()
{
        if(port_name=="")
        {
                //the port_name must be set before a call can be made to open the port
                fprintf(stderr, "Error the port_name must NOT be empty;  The port must be specified with setPortName before a call to openPort is made\n");
                return false;
        }

        struct serial_struct serinfo; // full struct info can be seen in linux/serial.h
        struct termios terminal_io_control;// POSIX tty i/o control. see: http://linux.die.net/man/3/termios
        memset(&terminal_io_control, 0, sizeof(terminal_io_control)); // 0 out the struct

        //ensure the port is closed before we start manipulating values
        closePort();

        // note: in linux everything is either a file or a directory. Our serial port is just a system/special file
        // try to open the port (the file on the system that is the port)
        // http://linux.die.net/man/2/open
        // O_RDWR sets the mode to read write
        // O_NOCTTY since we are a terminal device, we don't want this to be the process's controlling terminal. see http://linux.die.net/man/4/tty
        // O_NONBLOCK set the mode to non-blocking so the calling process (i.e. the open function) does NOT have to wait for it to open
        // open returns the new file descriptor, or -1 if an error occurred
        if ((socket_fd = open(port_name.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0)
        {
                fprintf(stderr, "Device open error for port: %s\n", port_name.c_str());
                closePort();
                return false; // return false to indicate the port didnt open
        }

        // configure how the terminal is controlled
        // B38400 sets the initial bitrate so we can later change it with setserial. see: http://linux.die.net/man/8/setserial
        // CS8 set the character size mask to 8
        // CLOCAL ignore modem control lines
        // CREAD Enable the receiver
        terminal_io_control.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
        // IGNPAR ignore framing errors and parity errors.
        terminal_io_control.c_iflag = IGNPAR;
        // set the output mode to raw
        terminal_io_control.c_oflag = 0;
        // set no local mode flags. see: http://www.gnu.org/software/libc/manual/html_node/Local-Modes.html
        terminal_io_control.c_lflag = 0;
        // disable the Timeout in deciseconds for noncanonical read
        terminal_io_control.c_cc[VTIME] = 0;
        // Set the minimum number of characters for noncanonical read
        terminal_io_control.c_cc[VMIN] = 0;

        // if there is old output- still on the port- clear it since we are opening it as a new port
        tcflush(socket_fd, TCIFLUSH);
        //set all the attributes on the terminal we just configured.
        // 0 is returned if any of the attributes were set and if there is an error -1 is returned. see: http://linux.die.net/man/3/tcsetattr
        tcsetattr(socket_fd, TCSANOW, &terminal_io_control);
        if(socket_fd==-1)
        {
                fprintf(stderr, "ERROR setting attributes on port: %s\n", port_name.c_str());
                return false;
        }

        // The ioctl() function manipulates the underlying device parameters of special files.
        // since our serial port is a special file it basically performs a generic I/O operation to see if the port is now online
        // Note: Typically -1 is returned on an error but the behavior is not fully defined. see: http://linux.die.net/man/2/ioctl
        if (ioctl(socket_fd, TIOCGSERIAL, &serinfo) < 0)
        {
                fprintf(stderr, "Cannot get the serial information after setting the attributes on port: %s\n",port_name.c_str());
                closePort();
                return false;
        }

        // clear the ASYNC speed bit
        serinfo.flags &= ~ASYNC_SPD_MASK;
        // set the custom speed bit so we can customize the speed on the port
        serinfo.flags |= ASYNC_SPD_CUST;
        // set our custom baud rate on the port
        serinfo.custom_divisor = serinfo.baud_base / baud_rate;
        // make sure we were able to set our custom speed on the port
        if (ioctl(socket_fd, TIOCSSERIAL, &serinfo) < 0)
        {
                fprintf(stderr, "Cannot set a custom baud rate of: %f on port: %s\n", baud_rate, port_name.c_str());
                closePort();
                return false;
        }

        // The following code is a repeat of some of the code that is above, I assume it is to lock in the baud rate we just set.
        // Dynamixel SDK's C code (in dxl_hal.c) for the function dxl_hal_open
        closePort();
        memset(&terminal_io_control, 0, sizeof(terminal_io_control));
        closePort(); // while this seems redundant here the Dynamixel SDK closes the port again
        if((socket_fd = open(port_name.c_str(), O_RDWR|O_NOCTTY|O_NONBLOCK)) < 0)
        {
                fprintf(stderr, "Device open error for port: %s on second access.\n", port_name.c_str());
                closePort();
                return false;
        }
        terminal_io_control.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
        terminal_io_control.c_iflag = IGNPAR;
        terminal_io_control.c_oflag = 0;
        terminal_io_control.c_lflag = 0;
        terminal_io_control.c_cc[VTIME] = 0;
        terminal_io_control.c_cc[VMIN] = 0;
        tcflush(socket_fd, TCIFLUSH);
        tcsetattr(socket_fd, TCSANOW, &terminal_io_control);

        // socket_fd should now have a handle on the port allowing us to send/receive bytes.
        // Since we have a handle set the "comm_status" and "using_bus", then return true to indicate the function was successful.
        comm_status=COMM_RXSUCCESS;
        using_bus=false; // the bus is not in use yet
        return true;
}

void DynamixelSerialPort::closePort()
{
        if (socket_fd != -1)
        {
      	  // flush both data received but not read and data written but not transmitted that still sits on the port.
      	  tcflush(socket_fd, TCIOFLUSH);
                // http://linux.die.net/man/3/close
                // POSIX close function to close a handle on a file descriptor
                if( close(socket_fd)==0 )
                {
                        // success, no errors occurred closing the file handle
                        // set the file descriptor to -1 to indicate that its not open
                        socket_fd = -1;
                }
                else
                {
                        fprintf(stderr, "device close error on port: %s\n", port_name.c_str());
                }
        }
}

bool DynamixelSerialPort::isOpen()
{
	// the socket_fd can only bet set to -1 if it was closed or there was an error (consequently it was never opened).
	if (socket_fd != -1)
	{
		return true;
	}
	return false;
}

void DynamixelSerialPort::setTimeOut(int number_bytes_to_receive)
{
        transfer_start_time=getCurrentTime();
        // per the Dynamixel sdk in dxl_hal.c function dxl_hal_set_timeout receive_wait_time should be set to:
        receive_wait_time=(float)(byte_transfer_time*(float)number_bytes_to_receive + 5.0f);
};

// the Dynamixel sdk uses this function to time the com port to ensure we are within bounds since the port is in a custom raw mode
bool DynamixelSerialPort::hasTimedOut()
{
        long time_elapsed=getCurrentTime()-transfer_start_time;
        if(time_elapsed>receive_wait_time)
        {
                return true;
        }
        else if(time_elapsed<0)// if for some reason the transfer_start_time was set wrong, correct it
        {
                transfer_start_time=getCurrentTime();
        }
        return false;
};

// Note: the packet is already expected to be fully and properly constructed in the memory cut out for instruction_packet[] when this is called
// instruction_packet[0] and instruction_packet[1] will be over written with 0xFF, and instruction_packet[instruction_packet[LENGTH]+3] will be written with the checksum
bool DynamixelSerialPort::sendPacket()
{
        if(using_bus==true)
        {
                fprintf(stderr, "Can't send the Dynamixel packet, the bus is indicated to be in use\n");
                return false; // the bus is in use by something else and we can't send a packet at this time
        }
        else if(isOpen()==false)
        {
      	  // if the port isn't open we can't send anything.
      	  comm_status = COMM_PORTCLOSED; // indicate that there was an error sending the packet
      	  return false; // the bus is in use by something else and we can't send a packet at this time
        }
        using_bus=true;

        // We will now verify the various fields of the packet that will be sent. Remember the structure is as follows:
        // 0xFF, 0xFF, ID, LENGTH , INSTRUCTION , PARAMETER 1, ... , PARAMETER N , CHECKSUM
        // More info on all Dynamixel packets can be seen here (note RX series and MX series packets are the same): http://support.robotis.com/en/product/dynamixel/communication/dxl_packet.htm

        // The first two bytes of 0xFF, 0xFF indicates the start of a packet, so we will explicitly set them to be safe.
        instruction_packet[0]=0xFF;
        instruction_packet[1]=0xFF;

        // An ID of 0x00-0xFC will communicant directly with a single servo, and 0xFE will broadcast to all servo(s). An ID of 0xFF is invalid.
        if(instruction_packet[ID]==0xFF)
        {
                fprintf(stderr, "Invalid ID of %u specified\n", instruction_packet[ID]);
                comm_status = COMM_TXERROR; // indicate that there was an error sending the packet
                using_bus = false; // free up the bus
                return false; //return false indicating that the write failed
        }

        // Next check the length field in the packet to make sure that it is within the bound of a packets size
        // per the sdk The length is calculated as “the number of Parameters (N) + 2”, hence why we make sure it isn't past the maximum number of parameters we can have +2
        if(instruction_packet[LENGTH]> (MAXNUM_NUM_TX_PARAMS+2))
        {
                fprintf(stderr, "Packet length (%u) exceeds the the max length (%u)\n", instruction_packet[LENGTH], (MAXNUM_NUM_TX_PARAMS+2));
                comm_status = COMM_TXERROR; // indicate that there was an error sending the packet
                using_bus = false; // free up the bus
                return false; //return false indicating that the write failed
        }

        // Verify that the instruction is one of the allowed instructions
        if(             instruction_packet[INSTRUCTION] != INST_PING
                        && instruction_packet[INSTRUCTION] != INST_READ
                        && instruction_packet[INSTRUCTION] != INST_WRITE
                        && instruction_packet[INSTRUCTION] != INST_REG_WRITE
                        && instruction_packet[INSTRUCTION] != INST_ACTION
                        && instruction_packet[INSTRUCTION] != INST_RESET
                        && instruction_packet[INSTRUCTION] != INST_SYNC_WRITE )
        {
                // Since it isn't one of the defined instruction, we have an error
                fprintf(stderr, "Invalid Instruction specified: 0x%X\n", instruction_packet[INSTRUCTION]);
                comm_status = COMM_TXERROR;
                using_bus = false;
                return false;
        }

        // Now calculate the checksum and insert it into the packet
        // per the sdk The computation method for the ‘Check Sum’ is as follows (note instruction_packet[0] and instruction_packet[1] are not in the calculation):
        // Check Sum = ~ (ID value + Length value + Instruction value + Parameter1 value + ... Parameter N value)
        // Note: If the calculated value is larger than 255, the lower byte is defined as the checksum. (We defined checksum as an uint8, and thus only have the lower byte)
        // Since arrays are 0 indexed we compare it to instruction_packet[LENGTH]+1
        uint8_t checksum = 0x00;
        for( uint8_t i=0; i<(instruction_packet[LENGTH]+1); i++ )
        {
                // As mentioned instruction_packet[0] and instruction_packet[1] are not in the calculation, hence the +2
                checksum += instruction_packet[i+2];
        }
        // Remember that instruction_packet[LENGTH] has the number of bytes following this field. So since the array is 0 indexed and 3 bytes come before the length field, the checksum is at position instruction_packet[instruction_packet[LENGTH]+3]
        // Per the formula mentioned above, we also still have to negate the checksum.
        instruction_packet[instruction_packet[LENGTH]+3] = ~checksum;

        // Now that the packet is verified, start the process of sending it
        // If there is old data on the port that hasn't been flushed (due to something like a timeout), get rid of it
        if(comm_status == COMM_RXTIMEOUT || comm_status == COMM_RXCORRUPT)
        {
                flushComPort();
        }

        // Note that the write command will cast this as a size_t in the call
        uint8_t num_bytes_to_send=instruction_packet[LENGTH] + 4;
        // The POSIX write function takes the the file descriptor(our socket), a pointer the what we want to write, and how much to write
        // The POSIX write function returns number of bytes that were actually written on the socket, and -1 on an error (hence the return is caught as an int)
        // See: http://linux.die.net/man/2/write
        int num_bytes_actualy_writen=write(socket_fd, instruction_packet, num_bytes_to_send);
        // verify that the data was actually written
        if(num_bytes_to_send!=num_bytes_actualy_writen)
        {
                fprintf(stderr, "ERROR writing instruction. Only %d of %u bytes were sent.\n", num_bytes_actualy_writen, num_bytes_to_send);
                comm_status = COMM_TXFAIL;
                using_bus = false;
                return false;
        }

        // Now that the data was written correctly, we need to set the timers to leave the port open depending on the instruction we were performing.
        if( instruction_packet[INSTRUCTION] == INST_READ )
        {
                transfer_start_time = getCurrentTime();
                // The time for a read instruction is dependent on the number of bytes we want to read and how long it take to transfer a byte at a given baud.
                // Remember that when the port was opened that byte_transfer_time was configured according to how long it takes to send a byte at a given baud
                // Also, remember that for a read instruction instruction_packet[PARAMETER+1] specifies how many bytes to read.
                // Note: that the packet sent back will be the number of bytes to read + 6 bytes (i.e. 0xff, 0xff, Id, Length, Error, and Checksum )
                // Note: There is a return delay time (configured on the servo at EEPROM address 0x05) that must also be included in our calculation.
                receive_wait_time = (float)(byte_transfer_time*(float)( instruction_packet[PARAMETER+1] + 6 ) + RETURN_DELAY_TIME);
        }
        else
        {
                // If it isn't a read instruction, our default behavior is to leave the port open for the Status Return Packet
                // Note: Sending to the broadcast id (0x254) does not return a Status Return Packet.
                transfer_start_time = getCurrentTime();
                // Note: that the packet sent back will be 6 bytes (i.e. 0xff, 0xff, Id, Length, Error, and Checksum )
                // Note: There is a return delay time (configured on the servo at EEPROM address 0x05) that must also be included in our calculation.
                receive_wait_time = (float)(byte_transfer_time*6.0f + RETURN_DELAY_TIME);
        }

        // set the status to indicate that we were successful, and return true since the write completed.
        comm_status = COMM_TXSUCCESS;
        return true;
}

// Note that while a timeout has not happened, this function may be called multiple times. Hence why all amounts and locations specified to read are based on receive_packet_total_length and num_packets_currently_received
bool DynamixelSerialPort::receivePacket()
{
  if(using_bus==false)
  {
    // the send instruction should have reserved the bus so we can at least receive the default package sent back
    fprintf(stderr, "Can't receive the Dynamixel packet, the bus is not reserved and thus can't accept data\n");
    return false;
  }

  if(instruction_packet[ID]==BROADCAST_ID)
  {
    // a message sent to all servo(s) (broadcasted) will not receive a reply from the servo(s) as it could potentially overload the bus.
    // since since a message is not sent back and this is the correct behavior, free the buss and return success
    comm_status=COMM_RXSUCCESS;
    using_bus=false;
    return true;
  }

  if(comm_status==COMM_TXSUCCESS)
  {
    // if the packet was properly sent we should be waiting for (AT LEAST) the default reply message- the "status packet".
    // the default status packet is: 0xFF, 0xFF, ID, Length, Error, Checksum. So the length specifier field here would be 2 (since it has the Error parameter and checksum).
    receive_packet_total_length=6;
    num_packets_currently_received=0;
    // Keep in mind that we still mat need to get more data depending on the value specified in status_packet[LENGTH]
  }

  // 0 out the locations of the status_packet that we are about to receive (to make sure we get good data)
  size_t num_bytes_to_read=(size_t)(receive_packet_total_length-num_packets_currently_received);
  memset(&status_packet[num_packets_currently_received], 0, num_bytes_to_read);
  // now we read from the port and write the results in the locations that we just zero(ed) out.
  // On success the number of bytes read is returned. A returned 0 indicates end of file reached, and since the serial port is technically a file it means there is no data sitting on it. A -1 is returned if there is an error.
  // Note: read returns a ssize_t (not a size_t)
  // see: http://linux.die.net/man/2/read
  ssize_t read_return_value =0;
  read_return_value = read(socket_fd, &status_packet[num_packets_currently_received], num_bytes_to_read);
  if(read_return_value==-1)
  {
    // to print a size_t use %zd. see: http://stackoverflow.com/questions/2524611/how-to-print-size-t-variable-portably
    fprintf(stderr, "ERROR reading the status return packet from the socket %zd was returned.\n", read_return_value);
    read_return_value=0;
  }
  else if(read_return_value > std::numeric_limits<uint8_t>::max())
  {
    //the number of bytes read is larger than how many bytes a Dynamixel servo can send. hence we should indicate that something might be wrong
    // Note: adding this with a uint8_t will recast read_return_value (chopping off bits)
    fprintf(stderr, "ERROR reading the status return packet from the socket. %zd bytes were read- which is more than we are allowed (%u).\n", read_return_value, std::numeric_limits<uint8_t>::max());
  }
  // note an implicit cast would be done by default. The cast done is to exemplify this.
  num_packets_currently_received+=(uint8_t)read_return_value;

  // if we don't have all the data yet see if a timeout has happened
  if(num_packets_currently_received < receive_packet_total_length)
  {
    if(hasTimedOut()==true)
    {
      // if we have timed out see what kind of error we should indicate
      if(num_packets_currently_received==0)
      {
        // if we haven't received anything at all yet just indicate that it timed out
        comm_status=COMM_RXTIMEOUT;
        fprintf(stderr, "ERROR reading the status return packet due to a timeout.\n");
      }
      else
      {
        // if we have received data indicate that we can't trust it- it's corrupt.
        comm_status=COMM_RXCORRUPT;
        fprintf(stderr, "ERROR reading the status return packet due to a timeout; the data is corrupt.\n");
      }
      // free the bus and return false to indicate that there was an issue
      using_bus=false;
      return false;
    }
  }

  // Note: A header is indicated by consecutive 0xFF, 0xFF. We need to examine both values at once, and hence the for loop range.
  // Also note that the sdk does this operation with a unsigned values (uint8_t-uint8_t) which could result in a negative number.
  //    Per the ISO specs this would subsequently become  negative_result modulo UINT8_MAX+1, which could access data out of bounds. For examples see: http://craighenderson.co.uk/papers/numeric_compare/ and http://jwwalker.com/pages/safe-compare.html
  //    Because of this a short int is used here to account for this, and subsequent type promotion (If the type of the operand with signed integer type can represent all of the values of the type of the operand with unsigned integer type, the operand with unsigned integer type is converted to the type of the operand with signed integer type),
  //            For type promotions see: https://www.securecoding.cert.org/confluence/display/seccode/INT02-C.+Understand+integer+conversion+rules
  short int header_location=0;
  for(header_location=0;header_location<(num_packets_currently_received-1);header_location++)
  {
    if(status_packet[header_location]==0xFF && status_packet[header_location+1]==0xFF)
    {
      // we found the header so break
      break;
    }
    else if(header_location == num_packets_currently_received-2 && status_packet[num_packets_currently_received-1]==0xFF )
    {
      // if we have read it to fast ()
      // if the last byte received was 0xFF we could receive another 0xFF in the future, and we need to account for this- given the next operations
      break;
    }
  }

  if(header_location>0)
  {
    // if our header wasn't at the start, we still need the rest of the data associated with this packet
    for(short int j=0;j<(num_packets_currently_received-header_location); j++)
    {
      status_packet[j]=status_packet[j+1];
    }
    // the header_location is guaranteed to be less than num_packets_currently_received (given that the previous for loop stops at num_packets_currently_received-1).
    //          Therefore a negative value wont occur here, and the subtraction and subsequent casting is safe.
    num_packets_currently_received-=header_location;
  }
  if(num_packets_currently_received<receive_packet_total_length)
  {
    // we need to wait for more data
    comm_status=COMM_RXWAITING;
    // waiting for more data isn't an error and is completely acceptable- per the sdk. Since we are still waiting don't touch the bus use indicator.
    return true;
  }

  // if we have made it this far we have all our packet's data
  // So now make sure that the packet received came from the servo we just talked with.
  if(instruction_packet[ID]!=status_packet[ID])
  {
    // If the ID(s) don't match, assume that the data is corrupt. So indicate this, free up the bus, and return.
    comm_status=COMM_RXCORRUPT;
    using_bus=false;
    return false;
  }

  // Remember that we could be receiving something bigger than the default status packet, so now that we have the length specifier we may need to get more data
  //    the return packets structure is 0xFF,0xFF,ID,LENGTH, ERROR, PARAMETER 1, ... , PARAMETER N, CHECKSUM
  receive_packet_total_length=4+status_packet[LENGTH];
  if(num_packets_currently_received<receive_packet_total_length)
  {
    num_bytes_to_read=(size_t)(receive_packet_total_length-num_packets_currently_received);
    // Possibly consider doing another memset based on the new amount to get- curently another isn't done here for speed.
    read_return_value = read(socket_fd, &status_packet[num_packets_currently_received], num_bytes_to_read);
    // again check what is returned
    if (read_return_value == -1)
    {
      fprintf(stderr, "ERROR reading the status return packet from the socket %zd was returned.\n", read_return_value);
      read_return_value = 0;
    }
    else if (read_return_value > std::numeric_limits<uint8_t>::max())
    {
      //the number of bytes read is larger than how many bytes a Dynamixel servo can send. hence we should indicate that something might be wrong
      // Note: adding this with a uint8_t will recast read_return_value (chopping off bits)
      fprintf(stderr, "ERROR reading the status return packet from the socket. %zd bytes were read- which is more than we are allowed (%u).\n", read_return_value, std::numeric_limits<uint8_t>::max());
    }
    // note an implicit cast would be done by default. The cast done is to exemplify this.
    num_packets_currently_received += (uint8_t)read_return_value;
    // again see if we need to wait on more data
    if(num_packets_currently_received<receive_packet_total_length)
    {
      comm_status=COMM_RXWAITING;
      // waiting for more data isn't an error and is completely acceptable- per the sdk. Since we are still waiting don't touch the bus use indicator.
      return true;
    }
  }

  // now if we have made it this far we have the entire packet and can process the checksum
  if(status_packet[getChecksumLocation(status_packet)]!=calcPacketChecksum(status_packet))
  {
    //if the checksum(s) don't match we have an error. so free the bus and return false
    comm_status=COMM_RXCORRUPT;
    using_bus=false;
    return false;
  }

  // since the checksum(s) matched indicate that the read was successful. Then free the bus and return true.
  comm_status=COMM_RXSUCCESS;
  using_bus=false;
  return true;
}


// calcPacketChecksum can be used to calculate the checksum on any packet (remember the class has 2 different sized packets in it). Note: c\c++ has undefined behavior for an access out of bounds in an array.
//              The problem is that when you pass an array to a function (i.e. a pass by reference) the type information is lost (uint8_t packetArray[] decays into a pointer to a uint8_t (uint8_t *packetArray)).
//              Hence the size of the array can't be recovered with sizeof() tricks. The type information can be retained if were passed as a sized array (i.e. uint8_t packetArray[MIN_ARRAY_SIZE]).
//              But I wan't to make the function more dynamic so I can just use it on any dynamixel packet anywhere.
//              Templating the function allows me to do this; typename T will be the variable type of the packet array, and int passed_array_size will be the size of the packet array.
//              Also just to ensure safety and indicate to other programmers things about the function, const is used- indicates its constant and doesn't change.
//              The const on the packetArray[] ensures that the start address isn't changed and the const at the end of the function is to ensure that the function changes no member variables of the class.
// note here that calcPacketChecksum takes a reference to the start address of a packet;
template <typename T, unsigned int passed_array_size>
T DynamixelSerialPort::calcPacketChecksum(const T (&packetArray) [passed_array_size]) const
{
        T calculatedChecksum=0x00;
        //the minimum size of a packet is that from a PING instruction
        if(passed_array_size < 6)
        {
                fprintf(stderr, "ERROR Can't calculate a checksum on a packet this small (size: %u )\n",passed_array_size);
                return calculatedChecksum;
        }
        // we need to ensure that when calculating the checksum that we wouldn't access any elements past the array.
        if (packetArray[LENGTH]+4>passed_array_size)
        {
                fprintf(stderr, "ERROR Can't calculate the checksum on passed packet; an access out of bounds of the array would occur due to the length specifier (%u) in the packet.)\n",packetArray[LENGTH]+4);
                return calculatedChecksum;
        }

        // Remember that instruction_packet[LENGTH] has the number of bytes following this field (which includes 1 space for the checksum).
        for(int i=0; i<(packetArray[LENGTH]+1); i++ )
        {
                // As mentioned instruction_packet[0] and instruction_packet[1] are not in the calculation, hence the +2
                calculatedChecksum += packetArray[i+2];
        }
        // Per the formula previously mentioned, we also still have to negate the checksum.
        calculatedChecksum = ~calculatedChecksum;
        return calculatedChecksum;
}
// This is also templated just in case its needed for future use with checksum(s).
// Here we specifically return an unsigned int because it should be a integral location location in a packet.
template <typename T, unsigned int passed_array_size>
unsigned int DynamixelSerialPort::getChecksumLocation(const T (&packetArray) [passed_array_size]) const
{
        if(passed_array_size > LENGTH)//strictly grater than due to array indexing- guarantees packetArray[LENGTH] exists.
        {
                // Remember that the 0xFF, 0xff, and ID come before the instruction_packet[LENGTH] parameter
                return ((unsigned int)packetArray[LENGTH] + 3);
        }
        else // packet is to small and an access would be out of bounds
        {
                fprintf(stderr, "ERROR Can't calculate the checksum's location. Passed packet size of %u is to small.\n",passed_array_size);
                return 0;
        }
}

template <typename T, unsigned int passed_array_size>
string DynamixelSerialPort::printPacket(const T (&packetArray) [passed_array_size]) const
{
        unsigned int num_bytes_per_element=sizeof(T)*2;
        unsigned int number_of_elements_to_print=0;
        string temp="";
        if(passed_array_size > LENGTH)//strictly grater than due to array indexing- guarantees packetArray[LENGTH] exists.
        {
                // Remember that the 0xFF, 0xFF, and ID come before the instruction_packet[LENGTH] parameter
                // Also instruction_packet[LENGTH] specifies how many bytes follows this field.
                number_of_elements_to_print=4+(unsigned int)packetArray[LENGTH];
        }
        else
        {
                fprintf(stderr, "ERROR Can't calculate the checksum's location. Passed packet size of %u is to small.\n",passed_array_size);
                return "";
        }
        bool checksum_valid=false;
        if(packetArray[getChecksumLocation(packetArray)]==calcPacketChecksum(packetArray))
        {
          checksum_valid=true;
        }
        if(passed_array_size >=number_of_elements_to_print)
        {
                char hex_representation[2+num_bytes_per_element+1];//0xto specify the base + num_bytes_per_element+ null terminating char

                for(unsigned int i =0; i<number_of_elements_to_print;i++)
                {
                        // for anything written to the string stream, we want its hex representation.
                        // This representation should be prefaced with "0x" and padded with 0(s) such that it's indicative of the number of bytes.
                        // Also each will be a comma separated value (0x#...#, 0x#...#, 0x#...#)
                        sprintf (hex_representation,"0x%0*X",num_bytes_per_element,packetArray[i]);
                        temp+=hex_representation;
                        if(i!=number_of_elements_to_print-1)
                        {
                                temp+=", ";
                        }
                }
        }
        else
        {
                fprintf(stderr, "ERROR Can't print the passed packet. An access at %u would be out of bounds of its size (%u)\n",4+(unsigned int)packetArray[LENGTH],passed_array_size);
                fprintf(stderr, "The status of the COM port %s is %s\n", port_name.c_str(), printComStatus().c_str());
                return "";
        }
        if(checksum_valid==false)
        {
          temp+=" BAD_CHECKSUM";
        }
        return temp;
}

// Form the Dynamixel instruction packet for a command; the packet will be created in instruction_packet[].
// Return true on success and false on failure
bool DynamixelSerialPort::createPacket(uint8_t id, uint8_t instruction_code, vector<uint8_t> paramaters)
{
        // No packet sent from the controller to the servo(s) can have more than MAXNUM_NUM_TX_PARAMS in them
        if(paramaters.size()>MAXNUM_NUM_TX_PARAMS)
        {
                fprintf(stderr, "ERROR %u bytes can't be written to a servo, only %u bytes can be.\n", (unsigned int) paramaters.size(),MAXNUM_NUM_TX_PARAMS);
                return false; //packet can't be created
        }
        instruction_packet[ID]=id;
        // Remember that the the value specified in instruction_packet[LENGTH] is the number of bytes in the packet following instruction_packet[LENGTH].
        // Hence instruction_packet[LENGTH]= 1 byte for the Instruction Code that will be performed (i.e. read/write/ping)+paramaters.size() bytes + 1 byte for the checksum.
        instruction_packet[LENGTH]=1+paramaters.size()+1;
        instruction_packet[INSTRUCTION]=instruction_code;
        for (int i=0; i<paramaters.size();i++)
        {
                // fill in the various parameters for the packet
                instruction_packet[PARAMETER+i]=paramaters[i];
        }

        instruction_packet[getChecksumLocation(instruction_packet)]=calcPacketChecksum(instruction_packet);
        return true;
}

template <typename T, unsigned int passed_array_size>
string DynamixelSerialPort::getStatusPacketErrors(const T (&packetArray) [passed_array_size]) const
{
	string error_bits_set="";
	if(passed_array_size > ERRBIT)//strictly grater than due to array indexing- guarantees packetArray[ERRBIT] exists.
	{
		// Remember that the 0xFF, 0xFF, ID, and LENGTH come before the status_packet[ERRBIT] parameter
		if(packetArray[ERRBIT]&ERRBIT_VOLTAGE)
		{
			if(error_bits_set!="")
			{
				error_bits_set.append(", ");
			}
			error_bits_set.append("Input Voltage");
		}
		if(packetArray[ERRBIT]&ERRBIT_ANGLE)
		{
			if(error_bits_set!="")
			{
				error_bits_set.append(", ");
			}
			error_bits_set.append("Angle Limit");

		}
		if(packetArray[ERRBIT]&ERRBIT_OVERHEAT)
		{
			if(error_bits_set!="")
			{
				error_bits_set.append(", ");
			}
			error_bits_set.append("Overheating");
		}
		if(packetArray[ERRBIT]&ERRBIT_RANGE)
		{
			if(error_bits_set!="")
			{
				error_bits_set.append(", ");
			}
			error_bits_set.append("Out of Defined Range");
		}
		if(packetArray[ERRBIT]&ERRBIT_CHECKSUM)
		{
			if(error_bits_set!="")
			{
				error_bits_set.append(", ");
			}
			error_bits_set.append("Wrong instruction_packet Checksum");
		}
		if(packetArray[ERRBIT]&ERRBIT_OVERLOAD)
		{
			if(error_bits_set!="")
			{
				error_bits_set.append(", ");
			}
			error_bits_set.append("Required Torque Overload");
		}
		if(packetArray[ERRBIT]&ERRBIT_INSTRUCTION)
		{
			if(error_bits_set!="")
			{
				error_bits_set.append(", ");
			}
			error_bits_set.append("Undefined Instruction Code Sent");
		}
	}
	else
	{
		fprintf(stderr, "ERROR Can't pull the ERRBIT's location in the returned status_packet. Passed packet size of %u is to small.\n",passed_array_size);
		return "";
	}
	error_bits_set.insert(0,"The status_packet had the following error bit(s) set ");
	return error_bits_set;
}


string DynamixelSerialPort::printInstructionType(const uint8_t &instruction_code) const
{
  switch(instruction_code)
  {
    case INST_PING:
      return "PING";
      break;
    case INST_READ:
      return "READ";
      break;
    case INST_WRITE:
      return "WRITE";
      break;
    case INST_REG_WRITE:
      return "WRITE_REGISTERED";
      break;
    case INST_ACTION:
      return "ACTION";
      break;
    case INST_RESET:
      return "RESET";
      break;
    case INST_SYNC_WRITE:
      return "WRITE_SYNCED";
      break;
    case INST_DIGITAL_RESET:
      return "DIGITAL_RESET";
      break;
    case INST_SYSTEM_READ:
      return "SYSTEM_READ";
      break;
    case INST_SYSTEM_WRITE:
      return "SYSTEM_WRITE";
      break;
    case INST_SYNC_REG_WRITE:
      return "WRITE_REISTERED_SYNC";
      break;
    default:
      break;
  }
  return "UNKNOWN_INSTRUCTION";

}

string DynamixelSerialPort::printComStatus() const
{
  switch (comm_status)
  {
    case COMM_TXSUCCESS:
      return "COMM_TXSUCCESS";
      break;
    case COMM_RXSUCCESS:
      return "COMM_RXSUCCESS";
      break;
    case COMM_TXFAIL:
      return "COMM_TXFAIL";
      break;
    case COMM_RXFAIL:
      return "COMM_RXFAIL";
      break;
    case COMM_TXERROR:
      return "COMM_TXERROR";
      break;
    case COMM_RXWAITING:
      return "COMM_RXWAITING";
      break;
    case COMM_RXTIMEOUT:
      return "COMM_RXTIMEOUT";
      break;
    case COMM_RXCORRUPT:
      return "COMM_RXCORRUPT";
      break;
    case COMM_PORTCLOSED:
   	 return "COMM_PORTCLOSED";
   	 break;
    default:
      break;
  }
  return "UNKNOWN_COM_STATUS";
}

bool DynamixelSerialPort::comErrorPresent() const
{
  switch (comm_status)
  {
    case COMM_TXFAIL:
      return true;
      break;
    case COMM_RXFAIL:
      return true;
      break;
    case COMM_TXERROR:
      return true;
      break;
    case COMM_RXTIMEOUT:
      return true;
      break;
    case COMM_RXCORRUPT:
      return true;
      break;
    case COMM_PORTCLOSED:
   	 return true;
   	 break;
    default:
      break;
  }
  return false;
}
//	Note: "vector<uint8_t> paramaters" is defined with default arguments in the class definition- where it should be defined. Also, default arguments can only be defined once- hence they are not shown in the function's definition.
//				For more on default arguments see: http://goo.gl/MK0RZ2
string DynamixelSerialPort::sendAndReceive(uint8_t id, uint8_t instruction_code, vector<uint8_t> paramaters)
{

  if(createPacket(id,instruction_code,paramaters)==false)
  {
		string ret_information;
		//all values prefaced with 0x, also cut out space for the null
		unsigned int num_bytes_per_element = sizeof(uint8_t) * 2;
		char hex_representation_buffer[2 + num_bytes_per_element + 1];
		ret_information += "Error Creating Instruction packet (id:";
		sprintf(hex_representation_buffer, "0x%0*X", num_bytes_per_element, id);
		ret_information += hex_representation_buffer;
		ret_information += " ";
		return ret_information;
  }
  sendPacket();
  if (comErrorPresent() == true)
  {
    return printComStatus();
  }
  // this is a do while loop to ensure it is executed at least once (because receivePacket will set the comm_status to COMM_RXWAITING on the first execution)
  do
  {
    receivePacket();

  }while (comm_status==COMM_RXWAITING);

  if(comErrorPresent()==true)
  {
    return printComStatus();
  }
  if(status_packet[ERRBIT]!=0x00)
  {
	  return getStatusPacketErrors(status_packet);
  }

  return DYNAMIXEL_COMUNICATION_SUCCESSFUL;

}

vector<uint8_t> DynamixelSerialPort::getReturnPacketParamaters() const
{
	vector<uint8_t> return_vector;
	// So the vector doesn't have the waste time growing, reserve an initial capacity that is approximately the data's size.
	return_vector.reserve(status_packet[LENGTH]-2);

	// Remember a status_packet is: 0xFF,0xFF,ID,LENGTH, ERROR, PARAMETER 1, ... , PARAMETER N, CHECKSUM
	// We only want the bytes for the parameters 1..N (not the ERROR byte and CHECKSUM byte)
	for(uint8_t i=0; i<(status_packet[LENGTH]-2);i++)
	{
		return_vector.push_back(status_packet[PARAMETER+i]);
	}
	return return_vector;
}



#endif /* DYNAMIXEL_SERIAL_PORT_H_ */
