'''
Created on May 21, 2015

@author: peter
'''
from collections import deque
from array import array
from hdlc.crc import crc_ccitt_update

class OutgoingDataLink(object):
    '''
    classdocs
    '''

    @staticmethod
    def XON():
        return 0x11
    
    @staticmethod
    def XOFF():
        return 0x13
    
    @staticmethod
    def MASK():
        return 0x20
    
    @staticmethod
    def ESC():
        return 0x7d
    
    @staticmethod
    def FLAG():
        return 0x7e

    def __init__(self, hardware):
        '''
        Constructor
        '''
        self.hardware = hardware
        self.queue = deque()
        self.outgoingCRC = 0xFFFF
    
    def put(self, b):
        if b == OutgoingDataLink.XON() or b == OutgoingDataLink.XOFF() or b == OutgoingDataLink.ESC() or b == OutgoingDataLink.FLAG():
            self.queue.append(OutgoingDataLink.ESC())
            self.queue.append(OutgoingDataLink.MASK() ^ b)
        else:
            self.queue.append(b)

    def beginOutgoingFrame(self, opcode):
        if len(self.queue) == 0: self.queue.append(OutgoingDataLink.FLAG())
        self.put(opcode)
        self.outgoingCRC = 0xFFFF
        self.outgoingCRC = crc_ccitt_update(self.outgoingCRC, opcode)
    
    def appendPayload(self, payload):
        if isinstance(payload, (list, tuple, array)):
            for p in payload:
                self.appendPayload(p)
        else:
            self.put(payload)
            self.outgoingCRC = crc_ccitt_update(self.outgoingCRC, payload)
    
    def endOutgoingFrame(self):
        self.put(self.outgoingCRC >> 8)
        self.put(self.outgoingCRC & 0xFF)
        self.queue.append(OutgoingDataLink.FLAG())
    
    def schedule(self):
        if len(self.queue) > 0 and self.hardware.putReady():
            self.hardware.put(self.queue.popleft())
