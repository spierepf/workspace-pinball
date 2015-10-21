'''
Created on Oct 11, 2015

@author: peter
'''
from EscapingSink import EscapingSink
from crc import crc_ccitt_update

class FrameTransmitter(object):
    '''
    classdocs
    '''

    @staticmethod
    def ACK():
        return 0x40

    @staticmethod
    def SYN_DISCONNECT():
        return 0x80

    @staticmethod
    def SYN_REQUEST():
        return 0x81

    @staticmethod
    def SYN_RESPONSE():
        return 0x82

    @staticmethod
    def SYN_COMPLETE():
        return 0x83

    def __init__(self, sink):
        '''
        Constructor
        '''
        self.sink = sink
        self.buffer = []
        self.position = -1
        
    def isReady(self):
        return self.buffer == []
    
    def transmit(self, header, payload=[]):
        self.buffer = [header]
        self.buffer += payload
        crc = crc_ccitt_update(0xFFFF, header)
        for octet in payload:
            crc = crc_ccitt_update(crc, octet)
        self.buffer += [crc >> 8, crc & 0xFF]  
        self.position = -1
        
    def schedule(self):
        if(self.isReady()):
            return
        if self.position == -1 or self.position >= len(self.buffer):
            self.sink.writeFlag()
        else:
            self.sink.write(self.buffer[self.position])
        self.position += 1 
        if self.position >= len(self.buffer) + 1:
            self.buffer = [] 
