'''
Created on Jun 8, 2015

@author: peter
'''

class OutgoingFrameHandler(object):
    '''
    classdocs
    '''


    def __init__(self, outgoingDatalink):
        '''
        Constructor
        '''
        self.outgoingDatalink = outgoingDatalink
        
    def postFrame(self, opcode, payload):
        self.outgoingDatalink.beginOutgoingFrame(opcode)
        for b in payload:
            self.outgoingDatalink.appendPayload(b)
        self.outgoingDatalink.endOutgoingFrame()
        
    def schedule(self):
        self.outgoingDatalink.schedule()