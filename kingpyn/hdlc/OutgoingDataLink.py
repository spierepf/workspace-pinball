'''
Created on May 21, 2015

@author: peter
'''
from collections import deque
from array import array

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
    
    def put(self, b):
        if b == OutgoingDataLink.XON() or b == OutgoingDataLink.XOFF() or b == OutgoingDataLink.ESC() or b == OutgoingDataLink.FLAG():
            self.queue.append(OutgoingDataLink.ESC())
            self.queue.append(OutgoingDataLink.MASK() ^ b)
        else:
            self.queue.append(b)

    def beginOutgoingFrame(self, opcode):
        if len(self.queue) == 0: self.queue.append(OutgoingDataLink.FLAG())
        self.put(opcode)
        pass
    
    def appendPayload(self, payload):
        if isinstance(payload, (list, tuple, array)):
            for p in payload:
                self.put(p)
        else:
            self.put(payload)
    
    def endOutgoingFrame(self):
        self.queue.append(OutgoingDataLink.FLAG())
    
    def schedule(self):
        if len(self.queue) > 0 and self.hardware.putReady():
            self.hardware.put(self.queue.popleft())
