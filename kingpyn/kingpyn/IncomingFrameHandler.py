'''
Created on Jun 8, 2015

@author: peter
'''
from kingpyn.Observer import Observable

class IncomingFrameHandler(object):
    '''
    classdocs
    '''


    def __init__(self):
        '''
        Constructor
        '''
        self.frameNotifier = self.FrameNotifier(self)

    def handle(self, header, payload):
        self.frameNotifier.setChanged()
        self.frameNotifier.notifyObservers((payload[0], payload[1:]))
                
    class FrameNotifier(Observable):
        def __init__(self, outer):
            Observable.__init__(self)
            self.outer = outer
        def notifyObservers(self, frame):
            Observable.notifyObservers(self, frame)
