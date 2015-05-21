'''
Created on May 21, 2015

@author: peter
'''
from collections import deque

class OutgoingDataLink(object):
    '''
    classdocs
    '''


    def __init__(self, hardware):
        '''
        Constructor
        '''
        self.hardware = hardware
        self.queue = deque()
    
    def put(self, b):
        if b == 0x11 or b == 0x13 or b == 0x7d or b == 0x7e:
            self.queue.append(0x7d)
            self.queue.append(0x20 ^ b)
        else:
            self.queue.append(b)

    def beginOutgoingFrame(self, opcode):
        if len(self.queue) == 0: self.queue.append(0x7e)
        self.put(opcode)
        pass
    
    def appendPayload(self, payload):
        self.put(payload)
    
    def endOutgoingFrame(self):
        self.queue.append(0x7e)
    
    def schedule(self):
        if len(self.queue) > 0 and self.hardware.putReady():
            self.hardware.put(self.queue.popleft())
