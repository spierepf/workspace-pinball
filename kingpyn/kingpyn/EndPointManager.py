'''
Created on May 22, 2015

@author: peter
'''
from Queue import Queue
from Observer import Observer

class EndPointManager(object):
    '''
    classdocs
    '''


    def __init__(self):
        '''
        Constructor
        '''
        self.endPoints = []
        self.registeredSwitches = []
        self.switchEventQueue = Queue()

    def addEndPoint(self, endpoint):
        self.endPoints.append(endpoint)
        for switchIndex in range(12):
            for state in range(2):
                switchObserver = EndPointManager.SwitchObserver(self, endpoint, switchIndex, state)
                endpoint.switchNotifiers[switchIndex][state].addObserver(switchObserver)
    
    def hasEndPoints(self):
        return self.endPoints.count() > 0
    
    def run(self):
        for endPoint in self.endPoints:
            endPoint.schedule()

    def hasSwitchEvent(self):
        return not self.switchEventQueue.empty()
    
    def nextSwitchEvent(self):
        return self.switchEventQueue.get()

    def registerSwitch(self, switchNumber):
        self.registeredSwitches.append(switchNumber)

    def getEndPoint(self, boardId):
        for endPoint in self.endPoints:
            if(endPoint.id == boardId):
                return endPoint


    class SwitchObserver(Observer):
        def __init__(self, outer, endpoint, switchIndex, state):
            self.outer = outer
            self.endpoint = endpoint
            self.switchIndex = switchIndex
            self.state = state
            self.switchNumber = "{}-{}".format(self.endpoint.id, self.switchIndex) 
        def update(self, observable, arg):
            if self.switchNumber in self.outer.registeredSwitches:
                self.outer.switchEventQueue.put((self.endpoint.id, self.switchIndex, self.state))
