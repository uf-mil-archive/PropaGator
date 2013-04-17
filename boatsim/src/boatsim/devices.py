from __future__ import division

import itertools
import math
import random
import struct

from twisted.internet import protocol, task
from twisted.python import log
from twisted.protocols.basic import LineReceiver

from sim import datachunker

perfect_round = lambda x: (-1 if x < 0 else 1)*int(abs(x) + random.random())


class ThrusterProtocol(LineReceiver):
    delimiter = '\r'
    def __init__(self, thruster_id, thrusters):
        self.thruster_id = thruster_id
        self.thrusters = thrusters
    
    def connectionMade(self):
        LineReceiver.connectionMade(self)
        print 'thruster', self.thruster_id, 'connection made'
    
    def lineReceived(self, line):
		line = line.strip()
		if line == "":
			return
		if line.startswith('1'):
			self.setSpeed(int(line[1:]))
		elif line.startswith('2'):
			self.setSpeed(0)
		elif line.startswith('3'):
			self.setSpeed(-int(line[1:]))
		else:
			print 'unknown line from thruster: %r' % line
		self.transport.write(line[0] + '\r')
	
    def setSpeed(self, speed):
        self.thrusters[self.thruster_id] = speed/200
	
    def connectionLost(self, reason):
        print 'thruster', self.thruster_id, 'connection lost', reason
    
    def doStop(self): pass

class IMUProtocol(protocol.Protocol):
    def __init__(self, listener_set):
        self.listener_set = listener_set
    
    def connectionMade(self):
        print 'imu connection made'
        self.listener_set.add(self)
    
    def dataReceived(self, data):
        print 'imu', data.encode('hex')
    
    def sendUpdate(self, flags, supply_voltage, ang_rate, acceleration, mag_field, temperature, timestamp):
        #print "M", mag_field
        #print 
        #print acceleration
        #print [perfect_round(x/9.80665/0.00333) for x in acceleration]
        data = ''.join([
            struct.pack('<HH', flags, perfect_round(supply_voltage/0.00242)),
            struct.pack('<3h', *(perfect_round(x/0.000872664626) for x in ang_rate)),
            struct.pack('<3h', *(perfect_round(x/9.80665/0.00333) for x in acceleration)),
            struct.pack('<3h', *(perfect_round(x*1e4/0.0005) for x in mag_field)),
            struct.pack('<hq', perfect_round((temperature - 25)/0.14), perfect_round(timestamp*1e9)),
        ])
        assert len(data) == 32
        self.transport.write(data)
    
    def connectionLost(self, reason):
        self.listener_set.remove(self)
        print 'imu connection lost'

