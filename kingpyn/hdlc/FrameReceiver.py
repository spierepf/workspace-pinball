'''
Created on Oct 10, 2015

@author: peter
'''

from crc import crc_ccitt_update

class FrameReceiver(object):
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

    @staticmethod
    def CONTROL_BITS():
        return 0xC0
    
    def __init__(self, source):
        '''
        Constructor
        '''
        self.source = source
        self.frameHandler = None
        self.frame = []
        self.incomingCRC = 0xFFFF
    
    def setFrameHandler(self, frameHandler):
        self.frameHandler = frameHandler
        
    def schedule(self):
        while self.source.isReady():
            if self.source.isFlag():
                if len(self.frame) != 0:
                    if self.incomingCRC == 0:
                        header = self.frame[0]
                        del self.frame[0]
                        del self.frame[-2:]
                        if self.frameHandler != None:
                            self.frameHandler.handle(header, self.frame)
                self.frame = []
                self.nextByteEscaped = False
                self.incomingCRC = 0xFFFF
            else:
                b = self.source.read()
                self.frame += [b]
                self.incomingCRC = crc_ccitt_update(self.incomingCRC, b)
            self.source.next()
        pass
