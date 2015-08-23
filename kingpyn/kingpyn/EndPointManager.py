'''
Created on Jun 10, 2015

@author: peter
'''
from collections import deque
from hdlc.Tty import Tty
from kingpyn.EndPoint import EndPoint
from kingpyn.IncomingFrameHandler import IncomingFrameHandler
from hdlc.IncomingDataLink import IncomingDataLink
from kingpyn.OutgoingFrameHandler import OutgoingFrameHandler
from hdlc.OutgoingDataLink import OutgoingDataLink
from kingpyn.SolenoidAction import SolenoidAction

import serial.tools.list_ports

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
        tty = Tty(device)
        endPoint = EndPoint(IncomingFrameHandler(IncomingDataLink(tty)),
                            OutgoingFrameHandler(OutgoingDataLink(tty)),
                            self.switchEventQueue)
        endPoint.ensureID()
        self.endPoints[endPoint.id] = endPoint
    
    def addDevices(self):
        for (name, desc, hardware) in serial.tools.list_ports.comports():
            if hardware != 'n/a':
                self.addDevice(name)
            
    def schedule(self):
        for endPoint in self.endPoints.itervalues():
            endPoint.schedule()

    def addHardwareRule(self, endPointId, switchId, activity, solenoidId, attack, sustain):
        self.endPoints[endPointId].addHardwareRule(switchId, activity, solenoidId, attack, sustain)

    def pulseSolenoid(self, endPointId, solenoidId, milliseconds):
        self.endPoints[endPointId].pulseSolenoid(SolenoidAction(True, solenoidId, milliseconds * 1000, 0))
