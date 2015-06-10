'''
Created on Jun 10, 2015

@author: peter
'''
import logging
from PingPong import PingPong
from OpCode import OpCode

class EndPoint(object):
    '''
    classdocs
    '''


    def __init__(self, incomingFrameHandler, outgoingFrameHandler, switchEventQueue):
        '''
        Constructor
        '''
        self.id = None
        self.log = None
        self.switchEventQueue = switchEventQueue
        
        self.incomingFrameHandler = incomingFrameHandler
        self.outgoingFrameHandler = outgoingFrameHandler
        
        self.incomingFrameHandler.frameNotifier.addObserver(self)
        
        self.pingPong = PingPong(outgoingFrameHandler)
        self.incomingFrameHandler.frameNotifier.addObserver(self.pingPong)
        
    def schedule(self):
        self.incomingFrameHandler.schedule()
        self.outgoingFrameHandler.schedule()

    def update(self, observable, frame):
        if frame[0] == OpCode.MY_ID():
            self.id = frame[1][0]
            self.log = logging.getLogger("EndPoint[{}]".format(self.id))
        elif frame[0] == OpCode.LOG():
            if self.log != None:
                self.log.info("".join(map(chr, frame[1])))
        elif frame[0] == OpCode.SWITCH_ACTIVE():
            self.switchEventQueue.append(("{}-{}".format(self.id, frame[1][0]), True))
        elif frame[0] == OpCode.SWITCH_INACTIVE():
            self.switchEventQueue.append(("{}-{}".format(self.id, frame[1][0]), False))

    def ensureID(self):
        while self.id == None:
            self.schedule()