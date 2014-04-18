import serial

class xbee():
    def __init__(self,usb_id):
        self.commport = serial.Serial(port = "/dev/"+usb_id,baudrate=115200, parity=serial.PARITY_NONE, stopbits=1, bytesize=8,timeout = 1)
    
    def send_pos(pos):
        #self.commport.write('P'+'\n')
        for i in pos:
            self.commport.write(i+'\n')#Report position
    
    def send_start():
        self.commport.write('S'+'\n')#START
    
    def send_timeout():
        self.commport.write('F'+'\n')#Send quad to failsafe location
    
    def check_puck_status():
        data = self.commport.readline()#check if puck recieved
        if (data == 'R'+'\n'):
            return True
        else:
            return False
    def check_done():
        data = self.commport.readline()#check if quad on landing pad
        if (data == 'C'+'\n'):
            return True
        else:
            return False
