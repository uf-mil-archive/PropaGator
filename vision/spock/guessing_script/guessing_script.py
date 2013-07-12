#!/usr/bin/env python

import socket


TCP_IP = '192.168.9.7'
TCP_PORT = 20938
BUFFER_SIZE = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
s.send("PING\n")

data = s.recv(BUFFER_SIZE)
if data=="PONG\n" :
   s.send("REPORT;UF:ROCK@1234.1234,1234.1234\n")
data = s.recv(BUFFER_SIZE)
if data=="FAIL\n" :
   for i in range(0,500) :
      s.send("REPORT;Trollolol:Troll@Trololol,Trolololol\n")
s.close()

print "received data:", data
