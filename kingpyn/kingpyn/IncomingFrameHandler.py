'''
Created on Jun 8, 2015

@author: peter
'''
from kingpyn.Observer import Observable

class IncomingFrameHandler(object):
    '''
    classdocs
    '''


    def __init__(self, incomingDatalink):
        '''
        Constructor
        '''
        self.incomingDatalink = incomingDatalink
        self.frameNotifier = self.FrameNotifier(self)

    def schedule(self):
        self.incomingDatalink.schedule()
        if self.incomingDatalink.haveIncomingFrame():
            self.frameNotifier.setChanged()
            opcode = self.incomingDatalink.peek(0)
            payload = []
            for i in range(1, self.incomingDatalink.incomingFrameLength()):
                payload.append(self.incomingDatalink.peek(i)) 
            self.frameNotifier.notifyObservers((opcode, payload))
            self.incomingDatalink.nextIncomingFrame()

                
    class FrameNotifier(Observable):
        def __init__(self, outer):
            Observable.__init__(self)
            self.outer = outer
        def notifyObservers(self, frame):
            Observable.notifyObservers(self, frame)
