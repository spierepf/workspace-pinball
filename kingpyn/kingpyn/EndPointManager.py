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
        
    def addDevice(self, device):
        endPointWrapper = EndPointWrapper(Serial(device, 9600), self.switchEventQueue)
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
        self.endPoints[endPointId].addHardwareRule(switchId, activity, solenoidId, attack, sustain)

    def pulseSolenoid(self, endPointId, solenoidId, milliseconds):
        self.endPoints[endPointId].pulseSolenoid(SolenoidAction(True, solenoidId, milliseconds * 1000, 0))
