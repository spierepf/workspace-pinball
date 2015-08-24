'''
Created on Jun 10, 2015

@author: peter
'''
import logging
from PingPong import PingPong
from OpCode import OpCode
from HardwareRuleManager import HardwareRuleManager
from HardwareRule import HardwareRule
from Stimulus import Stimulus
from SolenoidAction import SolenoidAction

class EndPoint(object):
    '''
    classdocs
    '''


    def __init__(self, incomingFrameHandler, outgoingFrameHandler, switchEventQueue):
        '''
        Constructor
        '''
        self.id = None
        self.log = logging.getLogger("EndPoint[{}]".format(self.id))
        self.switchEventQueue = switchEventQueue
        
        self.incomingFrameHandler = incomingFrameHandler
        self.outgoingFrameHandler = outgoingFrameHandler
        
        self.incomingFrameHandler.frameNotifier.addObserver(self)
        
        self.pingPong = PingPong(outgoingFrameHandler)
        self.incomingFrameHandler.frameNotifier.addObserver(self.pingPong)
        
        self.hardwareRuleManager = HardwareRuleManager(outgoingFrameHandler)
        self.incomingFrameHandler.frameNotifier.addObserver(self.hardwareRuleManager)
        
    def schedule(self):
        self.incomingFrameHandler.schedule()
        self.outgoingFrameHandler.schedule()

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
            
    def addHardwareRule(self, switchId, activity, solenoidId, attack, sustain):
        self.hardwareRuleManager.addRule(HardwareRule(Stimulus(switchId, activity), SolenoidAction(True, solenoidId, attack, sustain)))

    def pulseSolenoid(self, activity):
        self.outgoingFrameHandler.postFrame(OpCode.PULSE_COIL(), activity.toByteArray())
        pass
