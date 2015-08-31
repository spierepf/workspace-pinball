'''
Created on May 20, 2015

@author: peter
'''
from collections import deque

from hdlc.crc import crc_ccitt_update

class IncomingDataLink(object):
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
        self.currentFrameLength = 0
        self.incomingFrameLengths = deque()
        self.nextByteEscaped = False
        self.incomingCRC = 0xFFFF
     
    def schedule(self):
        if self.hardware.getReady():
            b = self.hardware.get()
            if b == IncomingDataLink.FLAG():
                if self.currentFrameLength != 0:
                    if self.incomingCRC == 0:
                        self.revert(2)
                        self.incomingFrameLengths.append(self.currentFrameLength - 2)
                    else:
                        self.revert(self.currentFrameLength)
                    self.currentFrameLength = 0
                self.incomingCRC = 0xFFFF
            elif b != IncomingDataLink.ESC():
                if self.nextByteEscaped:
                    b ^= IncomingDataLink.MASK()
                    self.nextByteEscaped = False
                self.queue.append(b)
                self.incomingCRC = crc_ccitt_update(self.incomingCRC, b)
                self.currentFrameLength += 1
            else:
                self.nextByteEscaped = True
                
    def peek(self, i):
        return self.queue[i]
    
    def haveIncomingFrame(self):
        return len(self.incomingFrameLengths) > 0
    
    def incomingFrameLength(self):
        return self.incomingFrameLengths[0]
    
    def nextIncomingFrame(self):
        for _ in range(0, self.incomingFrameLengths.popleft()):
            self.queue.popleft()

    def revert(self, count):
        for _ in range(0, count):
            self.queue.pop()
        