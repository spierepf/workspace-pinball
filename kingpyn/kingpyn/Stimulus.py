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
        return [((self.activity & 1) << 7) | self.switchId ]
    
    @staticmethod
    def fromByteArray(byteArray):
        activity = byteArray[0] >> 7
        switchId = byteArray[0] & 0x7F
        return Stimulus(switchId, activity)
        
