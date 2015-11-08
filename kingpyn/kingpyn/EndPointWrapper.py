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
from kingpyn.HardwareRule import HardwareRule
from kingpyn.Stimulus import Stimulus
from kingpyn.HardwareRuleManager import HardwareRuleManager
from kingpyn.SolenoidAction import SolenoidAction
from IncomingFrameHandler import IncomingFrameHandler
from PingPong import PingPong

class EndPointWrapper(object):
    '''
    classdocs
    '''


    def __init__(self, tty, switchEventQueue):
        '''
        Constructor
        '''
        self.id = None
        self.log = logging.getLogger("EndPoint[{}]".format(self.id))
        self.switchEventQueue = switchEventQueue

        escapingSink = EscapingSink(TtySink(tty))
        escapingSource = EscapingSource(TtySource(tty))
        self.outgoingFrameBuffer = deque()
        frameTransmitter = FrameTransmitter(escapingSink)
        frameReceiver = FrameReceiver(escapingSource)
        
        self.incomingFrameHandler = IncomingFrameHandler()
        self.endPoint = EndPoint(escapingSource, frameReceiver, self.incomingFrameHandler, self.outgoingFrameBuffer, frameTransmitter, escapingSink)
        frameReceiver.setFrameHandler(self.endPoint)
        
        self.incomingFrameHandler.frameNotifier.addObserver(self)
        
        self.pingPong = PingPong(self.outgoingFrameBuffer)
        self.incomingFrameHandler.frameNotifier.addObserver(self.pingPong)
        
        self.hardwareRuleManager = HardwareRuleManager(self.outgoingFrameBuffer)
        self.incomingFrameHandler.frameNotifier.addObserver(self.hardwareRuleManager)

    def update(self, observable, frame):
        if frame[0] == OpCode.MY_ID() and len(frame[1]) == 1:
            self.id = frame[1][0]
            self.log = logging.getLogger("EndPoint[{}]".format(self.id))
            self.log.setLevel(logging.DEBUG)
            self.log.debug("Starting...")
        elif frame[0] == OpCode.LOG() and len(frame[1]) > 0:
            if self.log != None:
                self.log.info("".join(map(chr, frame[1])))
        elif frame[0] == OpCode.SWITCH_ACTIVE() and len(frame[1]) == 1:
            self.switchEventQueue.append(("{}-{}".format(self.id, frame[1][0]), 1))
        elif frame[0] == OpCode.SWITCH_INACTIVE() and len(frame[1]) == 1:
            self.switchEventQueue.append(("{}-{}".format(self.id, frame[1][0]), 0))
    
    def ensureID(self):
        while self.id == None:
            self.schedule()
    
    def schedule(self):
        self.endPoint.schedule()
        
    def addHardwareRule(self, switchId, activity, solenoidId, attack, sustain):
        self.hardwareRuleManager.addRule(HardwareRule(Stimulus(switchId, activity), SolenoidAction(True, solenoidId, attack, sustain)))

    def pulseSolenoid(self, activity):
        self.outgoingFrameBuffer += [[OpCode.PULSE_COIL()] + activity.toByteArray()]
