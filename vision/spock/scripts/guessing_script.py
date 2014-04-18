#!/usr/bin/python

import socket

import roslib
roslib.load_manifest('spock')
import rospy
#from std_msgs.msg import Header

from spock.srv import GuessSign, GuessSignResponse


rospy.init_node('guessing_script')

BUFFER_SIZE = 1024
TCP_IP = '192.168.1.10'
TCP_PORTA = 2000
TCP_PORTB = 2001



def guesssign(req):
    if req.course == 'A':
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP, TCP_PORTA))
        s.send("PING\n")
        data = s.recv(BUFFER_SIZE)
        if data=="PONG\n" :
            s.send("REPORT;UF:LIZARD@1234.1234,1234.1234\n")
            s.close()
    else:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP, TCP_PORTB))
        s.send("PING\n")
        data = s.recv(BUFFER_SIZE)
        if data=="PONG\n" :
            s.send("REPORT;UF:SPOCK@1234.1234,1234.1234\n")
            s.close()
    return GuessSignResponse()
rospy.Service('~guesssign', GuessSign, guesssign)

rospy.spin()
