import serial

###########Communication Protocol##################
#
#
#
#
#
####################################################
class MotorDriver():
	def __init__(self,usb_id):
		self.commport = serial.Serial(port = "/dev/"+usb_id,baudrate=9600, parity=serial.PARITY_NONE, stopbits=1, bytesize=8,timeout = 0)


	def set_forward_speed(self,speed):
		self.commport.write('1')
		self.commport.write(speed)
		
	def set_reverse_speed(self,speed):
		self.commport.write('2')
		self.commport.write(speed)
		
	def stop(self,speed):
		self.commport.write('3')
		
	def get_current(self,speed):
		self.commport.write('4')
		return (self.commport.read(1))
		
	def get_out_voltage(self,speed):
		self.commport.write('5')
		return (self.commport.read(1))
		
	def get_batt_voltage(self,speed):
		self.commport.write('6')
		return (self.commport.read(1))


