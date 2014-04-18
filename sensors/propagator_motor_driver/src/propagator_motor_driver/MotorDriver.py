import serial,time

###########Communication Protocol##################
#
#
#
#
#
####################################################
class MotorDriver():
    def __init__(self, port):
        self.commport = serial.Serial(port=port, baudrate=115200, parity=serial.PARITY_NONE, stopbits=1, bytesize=8,timeout = 1)
        self.commport.write('\r')
    
    def set_forward_speed(self,speed):
        assert 0 <= speed <= 1, repr(speed)
        self.commport.write('1')
        self.commport.write(str(int(round(200*speed))) + '\r')
    
    def set_reverse_speed(self,speed):
        assert 0 <= speed <= 1
        self.commport.write('3')
        self.commport.write(str(int(round(200*speed))) + '\r')
    
    def stop(self):
        #while True:
        #try:
        #       self.commport.open()
        self.commport.write('2')
    #except serial.SerialException:
    #       print('cannot communicate with motor driver, will keep trying to reconnect')
    #       self.commport.close()
    #       time.sleep(1)
    #       continue
    #       except KeyboardInterrupt:
    #               break
    #       break
    
    def get_current(self):
        while True:
            try:
                self.commport.open()
                self.commport.write('4')
                return (self.commport.read(1))
            except serial.SerialException:
                print('cannot communicate with motor driver, will keep trying to reconnect')
                self.commport.close()
                time.sleep(1)
                continue
            except KeyboardInterrupt:
                break
            break
    
    
    def get_out_voltage(self):
        while True:
            try:
                self.commport.open()
                self.commport.write('5')
                return (self.commport.read(1))
            except serial.SerialException:
                print('cannot communicate with motor driver, will keep trying to reconnect')
                self.commport.close()
                time.sleep(1)
                continue
            except KeyboardInterrupt:
                break
            break
    
    def get_batt_voltage(self):
        while True:
            try:
                self.commport.open()
                self.commport.write('6')
                return (self.commport.read(1))
            except serial.SerialException:
                print('cannot communicate with motor driver, will keep trying to reconnect')
                self.commport.close()
                time.sleep(1)
                continue
            except KeyboardInterrupt:
                break
            break


if __name__ == "__main__":
    md = MotorDriver("BL")
    md.set_reverse_speed(100)
