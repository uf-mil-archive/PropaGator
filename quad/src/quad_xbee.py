import serial

class quad_xbee():
    def __init__(self,usb_id):
        self.commport = serial.Serial(port = "/dev/"+usb_id,baudrate=115200, parity=serial.PARITY_NONE, stopbits=1, bytesize=8,timeout = 1)
    
    def recieve_pos(pos):#felix does degrees decimal minutes
        boat_pos = []
        for i in range(3):
            boat_pos.append = self.commport.readline()
        return boat_pos# Lat, long, alt
    
    def recieve_start():
        start_command=self.commport.readline()
        if(start_command=='S\n') :
            return True
        else :
            return False
    
    
    def check_timeout():
        check_timeout=self.commport.readline()
        if(check_timeout=='F\n') :
            return True
        else :
            return False
    
    def send_puck_recieve():
        self.commport.write('R\n')
    
    def send_complete():
        self.commport.write('C\n')
