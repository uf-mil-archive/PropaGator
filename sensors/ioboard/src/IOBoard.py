import serial

class IOBoard():
	def __init__(self,usb_id):
		self.commport = serial.Serial(port = "/dev/"+usb_id,baudrate=115200, parity=serial.PARITY_NONE, stopbits=1, bytesize=8,timeout = 1)
                self.shooting = False

	def shoot(self,shots):
                if (shots == 2):
                        self.shooting = True
                        self.commport.write('s')
                elif(shots == 3):
                        self.shooting = True
                        self.commport.write('S')
               
        def shoot_status(self):
                if (self.shooting == True):
                        data = self.commport.read(1)
                        if (data == 's' or data == 'S'):
                                self.shooting = False
                                return True
                        else:
                                return False
                else:
                        return True

        def read_temp(self):
                self.commport.write('T')
                while (not(self.commport.inWaiting() > 0)):{}
                return str(self.commport.readline())
       

        def check_kill(self):
                return self.commport.read(1)

        def close(self):
                self.commport.close()



if __name__ == "__main__":
	board = IOBoard()
	
