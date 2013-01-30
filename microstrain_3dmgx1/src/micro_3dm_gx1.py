#!/usr/bin/python
from __future__ import print_function
import serial
import sys
import struct
import select
import signal
from constants import *

DEBUG = False
TIMEOUT_SECS = 3
#code for handling timeouts on recieving packets
class Timeout(Exception):
	pass
	
def timeout_handler(signal, frame):
	raise Timeout()
	
signal.signal(signal.SIGALRM, timeout_handler)
####################################################################################################################################
#IMU driver code

class micro_3dm_gx1:
	def __init__(self, port = 'IMU', baud = 115200):
		self.port = port
		self.baud = baud	
		self.current_command = 0
		self.packet = []
		self.bad_packets = 0
		self.good_packets = 0
		
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
			return False		
		return True
			
		#check and make sure compensation algorithm is off address 122 = 0
		if(self.read_EEPROM(122) != 0):
			self.write_EEPROM(122, 0)
						
	def open_port(self):
		try:
			self.ser = serial.Serial('/dev/' + self.port, self.baud)
			self.send_byte(0x0)
			self.ser.flushInput()
			self.ser.flushOutput()
		except IOError:
			if DEBUG:
				print("cannot open port")
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
			return False
		return True
		
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
			return False
	
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
		signal.alarm(TIMEOUT_SECS)
		try:
			self.packet = []
			self.send_byte(0x10)
			self.send_byte(0x00)
			self.send_byte(command)
			self.current_command = command
			#setting a continuous command will send back a 7 byte response
			self.packet = list(self.ser.read(7))
			#read back the packet and make sure it is correct by parsing else return false
			if(not self.parse_packet(7)):
				if DEBUG:
					print("failed setting command")	
				return False
			else:
				if DEBUG:
					print("successly set command!:")										
		except IOError:
			print('cannot write and set to continuous mode')
			return False
		except Timeout:
			return False
		signal.alarm(0)
		return True
		
	def get_packet(self, num_bytes):
		#set an alarm to timeout in 5 seconds and signal a PacketTimeout if we have not gotten a full packet
		signal.alarm(TIMEOUT_SECS)
		self.packet = []   
		try:
			while(True):
				self.packet.append(self.ser.read(1))
				if DEBUG:
					print('self packet = ' + str(self.packet[0]))
					print('current command = ' + str(self.current_command))
					
				#if the first value of the packet is not the command byte, pop values off until it is
				if(ord(self.packet[0]) != self.current_command and self.packet):
					self.packet.pop(0)
						
				#check if we have enough bytes for a complete packet
				if(len(self.packet) >= num_bytes):
					#try to get a complete packet
					int_list = self.parse_packet(num_bytes)
					if(int_list):
						self.good_packets += 1
						signal.alarm(0)
						return int_list	
					else:
						while(self.packet and self.packet[0] != self.current_command):
						#remove values at the head of the packet until you find another potential header
							self.packet.pop(0)
		except Timeout:
			self.packet = []
			return False
					
	def parse_packet(self, num_bytes):
		int_list = []
		#pop off the header value first since we will not need it again
		#checksum is equal to all the values of packet added together
		try:
			computed_checksum = ord(self.packet.pop(0))
		except IndexError:
			return False
		if DEBUG:
			print('header = ' + str(computed_checksum))
		#read only the data integers (not checmsum) from the packet. Two bytes per integer
		for i in range((num_bytes - 1)/2):
			#pack first two bytes of packet into a string
			int_val = struct.pack('BB', ord(self.packet[2*i]), ord(self.packet[2*i + 1]))
			if DEBUG:
				print('appending ' + str(ord(self.packet[2*i])) + ' ' + str(ord(self.packet[2*i+1])))
				print('int val = ' + str(int(struct.unpack('>h', int_val)[0])))
			#unpack the two byte string as a short and store as an integer in the list
			int_list.append(int(struct.unpack('>h', int_val)[0]))
			#clear the values from the packet
			#self.packet[0:2] = []
		#checksum is the last integer in the list. Pop it off.
		checksum = int_list.pop()
		for ints in int_list:
			computed_checksum += ints 
		if(computed_checksum == checksum):
			if DEBUG:
				print('good packet!')
			return int_list
		else:
			#this was an incorrect packet
			self.bad_packets += 1
			if DEBUG:
				print('bad packet!')
			return False
	
	def get_inst_vectors(self):
		try:
			header = self.ser.read(1)
			if(header != chr(0x03)):
				print('wrong header returned for vectors')
				False	
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
			return False
	
	def get_quarternion(self):
		try:
			quarternion = self.ser.read(13)						
			return quarternion
		except IOError:
			print('cannot read or write')
			return False
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
	print('b for number of bad packets')
	
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
				if(vectors):
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
		if(ch == 'b'):
			print(imu.bad_packets)
				

			
	
	
