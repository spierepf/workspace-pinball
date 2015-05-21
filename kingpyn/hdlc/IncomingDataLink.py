'''
Created on May 20, 2015

@author: peter
'''
from collections import deque

class IncomingDataLink(object):
    '''
    classdocs
    '''


    def __init__(self, hardware):
        '''
        Constructor
        '''
        self.hardware = hardware
        self.queue = deque()
        self.currentFrameLength = 0
        self.incomingFrameLengths = deque()
     
    def schedule(self):
        if self.hardware.getReady():
            b = self.hardware.get()
            if b == 0x7e:
                if self.currentFrameLength != 0:
                    self.incomingFrameLengths.append(self.currentFrameLength)
                    self.currentFrameLength = 0
            elif b != 0x7d:
                self.queue.append(b)
                self.currentFrameLength += 1
            else:
                self.queue.append(self.hardware.get() ^ 0x20)
                self.currentFrameLength += 1
                
    def peek(self, i):
        return self.queue[i]
    
    def haveIncomingFrame(self):
        return len(self.incomingFrameLengths) > 0
    
    def incomingFrameLength(self):
        return self.incomingFrameLengths[0]
    
    def nextIncomingFrame(self):
        for _ in range(0, self.incomingFrameLength()):
            self.queue.popleft()
