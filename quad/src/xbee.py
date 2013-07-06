import serial

class xbee():
	def __init__(self,usb_id):
		self.commport = serial.Serial(port = "/dev/"+usb_id,baudrate=115200, parity=serial.PARITY_NONE, stopbits=1, bytesize=8,timeout = 1)
             
        def send_pos(pos):
                self.commport.write('GPS'+'\n')
                for i in pos:
                        self.commport.write(i+'\n')

        def send_start(goal):
               self.commport.write('Takeoff'+'\n')
               for i in goal:
                        self.commport.write(i+'\n')

        def send_timeout():
               self.commport.write('Timeout'+'\n')

        def check_puck_status():
                data = self.commport.readline()
                if (data == 'GotPuck'+'\n'):
                        return True
                else:
                        return False
        def check_done():
                data = self.commport.readline()
                if (data == 'Done'+'\n'):
                        return True
                else:
                        return False


 

