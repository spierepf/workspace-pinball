'''
Created on Oct 11, 2015

@author: peter
'''

class EscapingSink(object):
    '''
    classdocs
    '''


    @staticmethod
    def MASK():
        return 0x20

    @staticmethod
    def ESCAPE():
        return 0x7D

    @staticmethod
    def FLAG():
        return 0x7E

    def __init__(self, sink):
        '''
        Constructor
        '''
        self.sink = sink
        self.ready = True
        
    def isReady(self):
        return self.ready
    
    def write(self, octet):
        if octet == EscapingSink.ESCAPE() or octet == EscapingSink.FLAG():
            self.sink.write(EscapingSink.ESCAPE())
            self.sink.write(octet ^ EscapingSink.MASK())
        else:
            self.sink.write(octet)
        self.ready = False
        
    def writeFlag(self):
        self.sink.write(EscapingSink.FLAG())
        self.ready = False
        
    def schedule(self):
        self.ready = True