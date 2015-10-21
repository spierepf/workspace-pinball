'''
Created on Oct 14, 2015

@author: peter
'''
from hdlc.TtySink import TtySink
from hdlc.EscapingSink import EscapingSink
from hdlc.TtySource import TtySource
from hdlc.EscapingSource import EscapingSource
from hdlc.FrameTransmitter import FrameTransmitter
from hdlc.FrameReceiver import FrameReceiver
from hdlc.EndPoint import EndPoint
from collections import deque
from kingpyn.OpCode import OpCode
import logging

class EndPointWrapper(object):
    '''
    classdocs
    '''


    def __init__(self, tty, switchEventQueue):
        '''
        Constructor
        '''
        escapingSink = EscapingSink(TtySink(tty))
        escapingSource = EscapingSource(TtySource(tty))
        outgoingFrameBuffer = deque()
        frameTransmitter = FrameTransmitter(escapingSink)
        frameReceiver = FrameReceiver(escapingSource)

        self.endPoint = EndPoint(escapingSource, frameReceiver, self, outgoingFrameBuffer, frameTransmitter, escapingSink)
        self.switchEventQueue = switchEventQueue
        frameReceiver.setFrameHandler(self.endPoint)
        self.id = 255
        
    def handle(self, header, payload):
        frame = (payload[0], payload[1:])
        if frame[0] == OpCode.MY_ID() and len(frame[1]) == 1:
            self.id = frame[1][0]
            self.log = logging.getLogger("EndPoint[{}]".format(self.id))
            self.log.setLevel(logging.DEBUG)
            self.log.debug("Starting...")
        elif frame[0] == OpCode.PING() and len(frame[1]) == 1:
            self.endPoint.outgoingFrameBuffer += [[OpCode.PONG(), frame[1][0]]]
        elif frame[0] == OpCode.LOG() and len(frame[1]) > 0:
            if self.log != None:
                self.log.info("".join(map(chr, frame[1])))
        elif frame[0] == OpCode.SWITCH_ACTIVE() and len(frame[1]) == 1:
            self.switchEventQueue.append(("{}-{}".format(self.id, frame[1][0]), 1))
        elif frame[0] == OpCode.SWITCH_INACTIVE() and len(frame[1]) == 1:
            self.switchEventQueue.append(("{}-{}".format(self.id, frame[1][0]), 0))
    
    def ensureID(self):
        pass
        '''while self.id == None:
            self.schedule()'''
    
    def schedule(self):
        self.endPoint.schedule()
        
