import serial

class IOBoard():
	def __init__(self,usb_id):
		self.commport = serial.Serial(port = "/dev/IOBoard",baudrate=115200, parity=serial.PARITY_NONE, stopbits=1, bytesize=8,timeout = 1)

	def shoot(self):
                self.commport.write('S')

        def read_temp(self):
                self.commport.write('T')
                return self.commport.readline()

        def check_kill(self):
                return self.commport.read(1)



if __name__ == "__main__":
	board = IOBoard()
	
