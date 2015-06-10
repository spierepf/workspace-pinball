'''
Created on Jun 10, 2015

@author: peter
'''
from collections import deque
from Tty import Tty
from EndPoint import EndPoint
from IncomingFrameHandler import IncomingFrameHandler
from IncomingDataLink import IncomingDataLink
from OutgoingFrameHandler import OutgoingFrameHandler
from OutgoingDataLink import OutgoingDataLink

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
    
    def schedule(self):
        for endPoint in self.endPoints.itervalues():
            endPoint.schedule()

    def addHardwareRule(self, endPointId, switchId, activity, solenoidId, attack, sustain):
        self.endPoints[endPointId].addHardwareRule(switchId, activity, solenoidId, attack, sustain)
