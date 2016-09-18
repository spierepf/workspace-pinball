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
        self.log.setLevel(logging.INFO)
        
    def addDevice(self, device):
        tty = Serial(device, 9600)
        tty.baudrate = 115200
        endPointWrapper = EndPointWrapper(tty, self.switchEventQueue)
        endPointWrapper.ensureID()
        self.endPoints[endPointWrapper.id] = endPointWrapper
    
    def addDevices(self):
        for name in glob.glob('/dev/kingpyn*'):
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
        self.log.debug("Triggering driver {}-{} for {}ms".format(endPointId, solenoidId, milliseconds))
        if endPointId in self.endPoints:
            self.endPoints[endPointId].pulseSolenoid(SolenoidAction(True, solenoidId, milliseconds * 1000, 0))
        else:
            self.log.error("Dropping pulse for nonexistant endpoint: {}".format(endPointId))

    def setSwitchDebounceThreshold(self, endPointId, switchId, activity, milliseconds):
        if endPointId in self.endPoints:
            self.log.info("Setting switch debounce threshold for {}-{} to {}ms on {}".format(endPointId, switchId, milliseconds, activity))
            self.endPoints[endPointId].setSwitchDebounceThreshold(switchId, milliseconds * 1000, activity)
        else:
            self.log.error("Dropping switch debounce threshold for nonexistant endpoint: {}".format(endPointId))
        
    def setSwitchPullup(self, endPointId, switchId, enablePullup):
        if endPointId in self.endPoints:
            self.log.info("Setting internal pullup for switch {}-{} to {}".format(endPointId, switchId, enablePullup))
            self.endPoints[endPointId].setSwitchPullup(switchId, enablePullup)
        else:
            self.log.error("Dropping internal pullup config for nonexistant endpoint: {}".format(endPointId))
