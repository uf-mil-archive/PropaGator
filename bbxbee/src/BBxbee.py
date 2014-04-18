#!/usr/bin/python

from std_msgs.msg import Header
from geometry_msgs.msg import Wrench, Vector3, WrenchStamped
import serial
import rospy
rospy.init_node('bbxbee')

class BB_xbee():
    def __init__(self,usb_id):
        self.commport = serial.Serial(port = "/dev/"+usb_id,baudrate=9600, parity=serial.PARITY_NONE, stopbits=1, bytesize=8)
        self.wrench_pub=rospy.Publisher('wrench',WrenchStamped)
    
    def recievedirection(self):
        birdinput = self.commport.read(1)
        if birdinput == 'w':
            wrench = Wrench(force=Vector3(2,0,0),torque=Vector3(0,0,0))
        elif birdinput == 's':
            wrench = Wrench(force=Vector3(-2,0,0),torque=Vector3(0,0,0))
        elif birdinput == 'a':
            wrench = Wrench(force=Vector3(0,0,0),torque=Vector3(0,0,2))
        elif birdinput == 'd':
            wrench = Wrench(force=Vector3(0,0,0),torque=Vector3(0,0,-2))
        elif birdinput == 'x':
            wrench = Wrench(force=Vector3(0,0,0),torque=Vector3(0,0,0))
        
        self.wrench_pub.publish(WrenchStamped(
            header=Header(
                frame_id='/base_link',
            ),
            wrench=wrench,
        ))

bb=BB_xbee(usb_id='ttyUSB0')

while True:
    bb.recievedirection()
