import serial

class IOBoard():
	def __init__(self,usb_id):
		self.commport = serial.Serial(port = "/dev/"+usb_id,baudrate=115200, parity=serial.PARITY_NONE, stopbits=1, bytesize=8,timeout = 1)
                self.shooting = False
                self.reading_temp = False
        
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
                                return [True,False]
                        elif(data == 'E'):
                                return [False,True]
                        else:
                                return [False,False]
                else:
                        return [True,False]

        def read_temp(self):
                self.commport.write('T')
                self.reading_temp = True

        def temp_status(self):
                if (self.reading_temp == True):
                        if (self.commport.inWaiting() > 0):
                                self.reading_temp = False
                                return [str(self.commport.readline()),False]
                        elif(data == 'E'):
                                return [False,True]
                        else:
                                return [False,False]
                else:
                        return [False,False]
       

        def check_port(self):
                return self.commport.readline()

        def close(self):
                self.commport.close()



if __name__ == "__main__":
	board = IOBoard()
	
