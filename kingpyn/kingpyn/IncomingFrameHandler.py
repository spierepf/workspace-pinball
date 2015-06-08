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
            self.frameNotifier.notifyObservers()
            self.incomingDatalink.nextIncomingFrame()

                
    class FrameNotifier(Observable):
        def __init__(self, outer):
            Observable.__init__(self)
            self.outer = outer
        def notifyObservers(self):
            Observable.notifyObservers(self)
