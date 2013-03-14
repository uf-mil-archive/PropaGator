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

def crc16(data, prev=0, mask=0x1021):
    crc = prev
    for char in data:
        c = ord(char)
        c = c << 8
        
        for j in xrange(8):
            if (crc ^ c) & 0x8000:
                crc = ((crc << 1) ^ mask) & 0xffff
            else:
                crc = (crc << 1) & 0xffff
            c = c << 1
    return crc

class EmbeddedProtocol(protocol.Protocol):
    def connectionMade(self):
        self.dataReceived = datachunker.DataChunker(self.dataReceiver())
        self.packet_count = 0
    
    def datagramReceived(self, data, addr):
        self.dataReceived(data)
    
    def dataReceiver(self):
        while True:
            # wait for 7E
            garbage = []
            while True:
                b = yield 1
                if b == '\x7e':
                    break
                garbage.append(b)
            if garbage:
                print self, 'embedded garbage', ''.join(garbage).encode('hex')
            
            data = []
            while True:
                b = yield 1
                if b == '\x7e' and not data: # ignore leading 7E - zero length message will never happen, so we're out of sync
                    pass
                elif b == '\x7e' and data:
                    break
                elif b == '\x7d':
                    data.append(chr(ord((yield 1)) ^ 0x20))
                else:
                    data.append(b)
            data2 = ''.join(data)
            
            #print data2.encode('hex')
            
            if len(data2) < 2 or data2[-2:] != struct.pack('<H', crc16(data2[:-2])):
                print 'embedded invalid', data.encode('hex')
                continue
            data3 = data2[:-2]
            
            try:
                (dest_address, src_address, packetcount), data4 = struct.unpack('BBH', data3[:4]), data3[4:]
                
                if dest_address != self.local_address or src_address != self.remote_address:
                    print 'embedded address mismatch', (dest_address, src_address), (self.local_address, self.remote_address)
                    continue
                
                if data4:
                    (typecode,), data5 = struct.unpack('B', data4[:1]), data4[1:]
                else:
                    typecode, data5 = None, ''
                
                self.packetReceived(typecode, data5)
            except:
                print
                print self, "Error handling packet:", data3.encode('hex')
                log.err()
                print
    
    def sendPacket(self, typecode, contents, addr=None):
        data_pre = struct.pack('BBHB', self.remote_address, self.local_address, self.packet_count % 2**16, typecode) + contents
        data = data_pre + struct.pack('H', crc16(data_pre))
        
        res = []
        res.append('\x7e')
        for char in data:
            if char == '\x7d' or char == '\x7e':
                res.append('\x7d')
                res.append(chr(ord(char) ^ 0x20))
            else:
                res.append(char)
        res.append('\x7e')
        if addr:
            self.transport.write(''.join(res), addr)
        else:
            self.transport.write(''.join(res))
        
        self.packet_count += 1

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

