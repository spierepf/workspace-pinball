'''
Created on May 28, 2015

@author: peter
'''

class Stimulus(object):
    '''
    classdocs
    '''


    def __init__(self, switchId, activity):
        '''
        Constructor
        '''
        self.switchId = switchId
        self.activity = activity
    
    def __eq__(self, other):
        if isinstance(other, Stimulus):
            return self.switchId == other.switchId and self.activity == other.activity
        return NotImplemented
    
    def __hash__(self):
        return hash(self.switchId) ^ hash(self.activity)
    
    def toByteArray(self):
        return [(self.switchId << 1) | (self.activity & 1)]
    
    @staticmethod
    def fromByteArray(byteArray):
        switchId = byteArray[0] >> 1
        activity = byteArray[0] & 1
        return Stimulus(switchId, activity)
        
