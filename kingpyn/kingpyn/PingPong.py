'''
Created on Jun 10, 2015

@author: peter
'''
from kingpyn.OpCode import OpCode

class PingPong(object):
    '''
    classdocs
    '''


    def __init__(self, outgoingFrameHandler):
        '''
        Constructor
        '''
        self.outgoingFrameHandler = outgoingFrameHandler
        
    def update(self, observable, frame):
        if frame[0] == OpCode.PING():
            self.outgoingFrameHandler += [[OpCode.PONG(), frame[1][0]]]