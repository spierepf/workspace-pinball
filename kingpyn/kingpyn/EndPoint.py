'''
Created on May 21, 2015

@author: peter
'''
import logging

from OpCode import OpCode
from Observer import Observable

class EndPoint(object):
    '''
    classdocs
    '''

    def __init__(self, incomingDatalink, outgoingDatalink):
        '''
        Constructor
        '''
        self.incomingDatalink = incomingDatalink
        self.outgoingDatalink = outgoingDatalink
        self.switchNotifiers = [[EndPoint.SwitchNotifier(self) for _ in range(2)] for _ in range(12)]
        self.id = None
        
    def handleIncomingFrame(self):
        if self.incomingDatalink.peek(0) == OpCode.MY_ID():
            self.id = self.incomingDatalink.peek(1)
            self.log = logging.getLogger("EndPoint.{0.id}".format(self))
            self.log.setLevel(logging.DEBUG)
            self.log.debug("Starting new endpoint.")
        elif self.id == None:
            pass # Ignore all messages until we get an id
        elif self.incomingDatalink.peek(0) == OpCode.PING() and self.incomingDatalink.incomingFrameLength() == 2:
            self.outgoingDatalink.beginOutgoingFrame(OpCode.PONG())
            self.outgoingDatalink.appendPayload(self.incomingDatalink.peek(1))
            self.outgoingDatalink.endOutgoingFrame()
        elif self.incomingDatalink.peek(0) == OpCode.LOG():
            buf = ""
            for i in range(1, self.incomingDatalink.incomingFrameLength()):
                buf = buf + str(unichr(self.incomingDatalink.peek(i)))
            self.log.info(buf)
        elif self.incomingDatalink.peek(0) == OpCode.SWITCH_INACTIVE():
            self.log.info("Switch Inactive: {0.id}-{1}".format(self, self.incomingDatalink.peek(1)))
            self.switchNotifiers[self.incomingDatalink.peek(1)][0].notifyObservers()
        elif self.incomingDatalink.peek(0) == OpCode.SWITCH_ACTIVE():
            self.log.info("Switch Active: {0.id}-{1}".format(self, self.incomingDatalink.peek(1)))
            self.switchNotifiers[self.incomingDatalink.peek(1)][1].notifyObservers()
        
    def schedule(self):
        self.incomingDatalink.schedule()
        self.outgoingDatalink.schedule()
        
        if self.incomingDatalink.haveIncomingFrame():
            self.handleIncomingFrame()
            self.incomingDatalink.nextIncomingFrame()

    def ensureID(self):
        while self.id == None:
            self.schedule()

    def setHardwareRule(self, switchId, activity, driverId, attack, sustain):
        self.outgoingDatalink.beginOutgoingFrame(OpCode.SR_CONFIG())
        if activity == 0:
            self.outgoingDatalink.appendPayload(switchId + 128)
        else:
            self.outgoingDatalink.appendPayload(switchId + 0)
        self.outgoingDatalink.appendPayload(driverId * 2 + 1)
        self.outgoingDatalink.appendPayload(sustain)
        self.outgoingDatalink.appendPayload(attack % 256)
        self.outgoingDatalink.appendPayload(int(attack / 256))
        self.outgoingDatalink.endOutgoingFrame()
    
    
    class SwitchNotifier(Observable):
        def __init__(self, outer):
            Observable.__init__(self)
            self.outer = outer
        def notifyObservers(self):
            self.setChanged()
            Observable.notifyObservers(self)
