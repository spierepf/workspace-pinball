'''
Created on Jun 10, 2015

@author: peter
'''
from collections import deque
from hdlc.TtySource import TtySource
from hdlc.TtySink import TtySink
from kingpyn.SolenoidAction import SolenoidAction
from serial import Serial
from hdlc.EndPoint import EndPoint
from hdlc.EscapingSource import EscapingSource

import glob
from hdlc.FrameReceiver import FrameReceiver
from kingpyn.EndPointWrapper import EndPointWrapper

import logging

class EndPointManager(object):
    '''
    classdocs
    '''


    def __init__(self):
        '''
        Constructor
        '''
        self.endPoints = {}
        self.switchEventQueue = deque()
        self.log = logging.getLogger("EndPointManager")
        
    def addDevice(self, device):
        tty = Serial(device)
        tty.setBaudrate(115200)
        endPointWrapper = EndPointWrapper(tty, self.switchEventQueue)
        endPointWrapper.ensureID()
        self.endPoints[endPointWrapper.id] = endPointWrapper
    
    def addDevices(self):
        for name in glob.glob('/dev/ttyUSB*'):
            self.addDevice(name)
        for name in glob.glob('/dev/ttyACM*'):
            self.addDevice(name)
            
    def schedule(self):
        for endPoint in self.endPoints.itervalues():
            endPoint.schedule()

    def addHardwareRule(self, endPointId, switchId, activity, solenoidId, attack, sustain):
        if endPointId in self.endPoints:
            self.endPoints[endPointId].addHardwareRule(switchId, activity, solenoidId, attack, sustain)
        else:
            self.log.error("Dropping hardware rule for nonexistant endpoint: {}".format(endPointId))

    def pulseSolenoid(self, endPointId, solenoidId, milliseconds):
        if endPointId in self.endPoints:
            self.endPoints[endPointId].pulseSolenoid(SolenoidAction(True, solenoidId, milliseconds * 1000, 0))
        else:
            self.log.error("Dropping pulse for nonexistant endpoint: {}".format(endPointId))
