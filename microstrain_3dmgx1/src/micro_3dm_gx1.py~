#!/usr/bin/python
from __future__ import print_function
import serial
import sys
import struct
import select
from constants import *

DEBUG = False

class micro_3dm_gx1:
	def __init__(self, port = 'IMU', baud = 115200):
		self.port = port
		self.baud = baud	
		self.current_command = 0
		self.packet = []
		
	def init_EEPROM(self):
		#check addresses 238, 240, 242. 246 and change them to 
		#4, 10, 250, 3 respectively if they are not already. Gives 333hz data output rate
		try:
			if(self.read_EEPROM(238) != 4):
				self.write_EEPROM(238, 4)
			if(self.read_EEPROM(240) != 10):
				self.write_EEPROM(240, 10)
			if(self.read_EEPROM(242) != 250):
				self.write_EEPROM(242, 250)
			if(self.read_EEPROM(246) != 3):
				self.write_EEPROM(246, 3)
		except IOError:
			return 0		
		return 1
			
		#check and make sure compensation algorithm is off address 122 = 0
		if(self.read_EEPROM(122) != 0):
			self.write_EEPROM(122, 0)
						
	def open_port(self):
		try:
			self.ser = serial.Serial('/dev/' + self.port, self.baud)
			self.send_byte(0x0)
		except IOError:
			print('cannot open port')
			return False
		except:
			return False
		if DEBUG:
			print("opened port")
		return True
				
	def send_byte(self, byte):
		try:
			if(byte > 255):
				print('trying to send a non byte value')
				return 0
			self.ser.write(chr(byte))
		except IOError:
			print('cannot write value to IMU')
			return 0
		return 1
		
	def read_int(self):
		try:
		    int_read = int(struct.unpack('>h', self.ser.read(2))[0])
		except IOError:
			print('could not read an integer from IMU')
			return False
		return int_read		
		
	def read_EEPROM(self, address):
		try:
			self.send_byte(read_EEPROM_cmd)
			self.send_byte(address)
			value_read = self.read_int()
			return value_read
		except IOError:
			print('cannot read eeprom value from IMU')
			return 0
	
	def write_EEPROM(self, address, data):
		try:
			self.send_byte(write_EEPROM_cmd)
			self.send_byte(0x71)
			self.send_byte(address)
			#calculate the two bytes of data
			msb = (data & 0xFF00)>>8
			lsb = (data & 0x00FF)
			self.send_byte(msb)
			self.send_byte(lsb)			
			self.send_byte(0xAA)
			response = self.read_int()
			if(response != data):
				if DEBUG:
					print('write failed')
				return 0
			else:
				if DEBUG:
					print('write succeeded')
				return 1
		except IOError:
			pass
									
	def set_continuous_command(self, command):
		#The command sent to the IMU will be result in a continuous data stream
		try:
			self.send_byte(0x10)
			self.send_byte(0x00)
			self.send_byte(command)
			self.current_command = command
			#setting a continuous command will send back a 7 byte response
			_ = self.ser.read(7)		
		except IOError:
			print('cannot write and set to continuous mode')
			return 0
		return 1
		
	def get_packet(self, num_bytes):
		while(True):
			self.packet.append(self.ser.read(1))
			if DEBUG:
				print('self packet = ' + str(self.packet[0]))
				print('current command = ' + str(self.current_command))
			#if the first value of the packet is not the command byte, pop values off until it is
			if(ord(self.packet[0]) != self.current_command and len(self.packet) != 0):
				self.packet.pop(0)
					
			#check if we have enough bytes for a complete packet
			if(len(self.packet) >= num_bytes):
				#try to get a complete packet
				int_list = self.parse_packet(num_bytes)
				if(int_list):
					return int_list				
					
	def parse_packet(self, num_bytes):
		int_list = []
		#pop off the header value first since it is not an integer
		computed_checksum = ord(self.packet.pop(0))
		if DEBUG:
			print('header = ' + str(computed_checksum))
		#read only the data integers from the packet. Two bytes per integer
		for _ in range((num_bytes - 1)/2):
			#pack first two bytes of packet into a string
			int_val = struct.pack('BB', ord(self.packet[0]), ord(self.packet[1]))
			if DEBUG:
				print('appending ' + str(ord(self.packet[0])) + ' ' + str(ord(self.packet[1])))
				print('int val = ' + str(int(struct.unpack('>h', int_val)[0])))
			#unpack the two byte string as a short and store as an integer in the list
			int_list.append(int(struct.unpack('>h', int_val)[0]))
			#clear the values from the packet
			self.packet[0:2] = []
		#checksum is the last integer in the list. Pop it off.
		checksum = int_list.pop()
		for ints in int_list:
			computed_checksum += ints 
		
		if(computed_checksum == checksum):
			if DEBUG:
				print('good packet!')
			return int_list
		else:
			if DEBUG:
				print('bad packet!')
			return 0
	
	def get_inst_vectors(self):
		try:
			header = self.ser.read(1)
			if(header != chr(0x03)):
				print('wrong header returned for vectors')
				return 0	
			header = ord(header)				
			mag_x = self.read_int()
			mag_y = self.read_int()
			mag_z = self.read_int()
			acc_x = self.read_int()
			acc_y = self.read_int()
			acc_z = self.read_int()
			angrate_x = self.read_int()
			angrate_y = self.read_int()
			angrate_z = self.read_int()
			ticks = self.read_int()
			checksum = self.read_int()
			computed_checksum = header + mag_x + mag_y + mag_z + acc_x + acc_y + acc_z + angrate_x + angrate_y + angrate_z + ticks
			
			if(checksum != computed_checksum):
				print('checksums do not match')
				return -1
			return (mag_x, mag_y, mag_z, acc_x, acc_y, acc_z, angrate_x, angrate_y, angrate_z, ticks, checksum)
		except IOError:
			print('cannot read vectors from imu')
			return 0
	
	def get_quarternion(self):
		try:
			quarternion = self.ser.read(13)						
			return quarternion
		except IOError:
			print('cannot read or write')
			return 0
####################################################################################################################################			
#TESTING CODE

def prompt():
	print('r to read EEPROM address')
	print('w to write EEPROM address')
	print('v for instantaneous vectors')
	print('q for quarternion')
	print('t for random test')
	print('a to read AccelGainScale')
	print('g to read GyroGainScale')
	print('m to read MagGainScale')	
	print('x to exit')
	print('c for continous instaneous vectors')
	print('i to initialize EEPROM for 333hz output rate')
	
if __name__ == '__main__' :
	imu = micro_3dm_gx1()
	imu.open_port()
	prompt()
	run = True
	while run:
		ch = raw_input('$: ')
		if(ch == 'r'):
			address = int(raw_input('address: '))
			value_read = imu.read_EEPROM(address)
			print('value at address ' + str(address) + ' is ' + str(value_read))
		if(ch == 'w'):
			address = int(raw_input('address: '))
			data = int(raw_input('data: '))
			imu.write_EEPROM(address, data)
		if(ch == 'q'):
			print("getting quarternion")
			imu.send_byte(0x04)
			imu.get_quarternion()
		if(ch == 'v'):	
			imu.send_byte(0x00)
			imu.send_byte(0x03)
			vectors = imu.get_inst_vectors()
			print('mag_x = ' + str(vectors[0]))
			print('mag_y = ' + str(vectors[1]))	
			print('mag_z = ' + str(vectors[2]))	
			print('acc_x = ' + str(vectors[3]))
			print('acc_y = ' + str(vectors[4]))
			print('acc_z = ' + str(vectors[5]))
			print('angrate_x = ' + str(vectors[6]))
			print('angrate_y = ' + str(vectors[7]))
			print('angrate_z = ' + str(vectors[8]))	
			print('ticks = ' + str(vectors[9]))
			print('checksum = ' + str(vectors[10]))
		if(ch == '?'):
			prompt()
		if(ch == 'a'):
			value_read = imu.read_EEPROM(230)
			print(value_read)		
		if(ch == 'g'):
			value_read = imu.read_EEPROM(130)
			print(value_read)		
		if(ch == 'm'):
			value_read = imu.read_EEPROM(232)
			print(value_read)				
		if(ch == 't'):
			imu.init_EEPROM()
		if(ch == 'i'):
			imu.init_EEPROM()
		if(ch == 'x'):
			run = False
		if(ch == 'c'):
			imu.set_continuous_command(inst_vectors)
			while select.select([sys.stdin], [], [], 0)[0] == []:
				vectors = imu.get_packet(inst_vectors_bytes)
				sys.stdout.flush()
				print('mag_x = ' + str(vectors[0]),end = '      ')
				print('mag_y = ' + str(vectors[1]),end = '      ')	
				print('mag_z = ' + str(vectors[2]))	
				print('acc_x = ' + str(vectors[3]),end = '      ')
				print('acc_y = ' + str(vectors[4]),end = '      ')
				print('acc_z = ' + str(vectors[5]))
				print('angrate_x = ' + str(vectors[6]),end = '      ')
				print('angrate_y = ' + str(vectors[7]),end = '      ')
				print('angrate_z = ' + str(vectors[8]))	
				print('ticks = ' + str(vectors[9]))
				

			
	
	
