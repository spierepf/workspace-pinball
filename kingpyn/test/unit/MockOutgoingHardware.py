'''
Created on Jun 8, 2015

@author: peter
'''
from collections import deque

class MockOutgoingHardware(object):
    def __init__(self):
        self.outgoingBytes = deque()
        
    def putReady(self):
        return True

    def put(self, b):
        self.outgoingBytes.append(b)
