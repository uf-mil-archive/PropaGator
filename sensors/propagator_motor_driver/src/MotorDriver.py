import serial,time

###########Communication Protocol##################
#
#
#
#
#
####################################################
class MotorDriver():
	def __init__(self,usb_id):
		self.commport = serial.Serial(port = "/dev/"+usb_id,baudrate=115200, parity=serial.PARITY_NONE, stopbits=1, bytesize=8,timeout = 1)
		self.commport.write('\r')

	def set_forward_speed(self,speed):
		self.commport.write('1')
		self.commport.write(str(speed) + '\r')
		check = self.commport.read(1)
		tries = 0
		while (check != '1'):
			self.commport.flushInput()
			tries = tries + 1
			if (tries > 5):
				self.commport.write('R')
			self.commport.write('\r')
			self.commport.write('1')
			self.commport.write(str(speed) + '\r')
			check = self.commport.read(1)
			
	def set_reverse_speed(self,speed):
		self.commport.write('3')
		self.commport.write(str(speed) + '\r')
		check = self.commport.read(1)
		tries = 0
		while (check != '3'):
			self.commport.flushInput()
			tries = tries + 1
			if (tries > 5):
				self.commport.write('R')
			self.commport.write('\r')
			self.commport.write('3')
			self.commport.write(str(speed) + '\r')
			check = self.commport.read(1)
		
	def stop(self):
		self.commport.write('2')
		check = self.commport.read(1)
		tries = 0
		while (check != '2'):
			self.commport.flushInput()
			tries = tries + 1
			if (tries > 5):
				self.commport.write('R')
			self.commport.write('\r')
			self.commport.write('2')
			check = self.commport.read(1)
		
	def get_current(self):
		self.commport.write('4')
		return (self.commport.read(1))
		
	def get_out_voltage(self):
		self.commport.write('5')
		return (self.commport.read(1))
		
	def get_batt_voltage(self):
		self.commport.write('6')
		return (self.commport.read(1))
		
if __name__ == "__main__":
	md = MotorDriver("BL")
	md.set_reverse_speed(100)


