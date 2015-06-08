'''
Created on Jun 8, 2015

@author: peter
'''
from collections import deque

class MockIncomingHardware(object):
    def __init__(self):
        self.incomingBytes = deque()
        
    def getReady(self):
        return len(self.incomingBytes) > 0
    
    def get(self):
        return self.incomingBytes.popleft()
