'''
Created on Oct 13, 2015

@author: peter
'''
from random import getrandbits

class NoisySink(object):
    '''
    classdocs
    '''


    def __init__(self, sink):
        '''
        Constructor
        '''
        self.sink = sink
        
    def isReady(self):
        return self.sink.isReady()
    
    def write(self, octet):
        if getrandbits(8) != 0:
            self.sink.write(octet)
        else:
            noise = getrandbits(8)
            if noise != 0:
                self.sink.write(octet ^ noise);
