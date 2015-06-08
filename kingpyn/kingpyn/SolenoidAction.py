'''
Created on May 28, 2015

@author: peter
'''

class SolenoidAction(object):
    '''
    classdocs
    '''


    def __init__(self, enabled, driverId, attack, sustain):
        '''
        Constructor
        '''
        self.enabled = enabled
        self.driverId = driverId
        self.attack = attack
        self.sustain = sustain
        
    def __eq__(self, other):
        if isinstance(other, SolenoidAction):
            return self.enabled == other.enabled and self.driverId == other.driverId and self.attack == other.attack and self.sustain == other.sustain
        return NotImplemented
    
    def __hash__(self):
        return hash(self.enabled) ^ hash(self.driverId) ^ hash(self.attack) ^ hash(self.sustain)
    
    def toByteArray(self):
        return [self.enabled | self.driverId << 1,
                self.sustain,
                self.attack & 255,
                self.attack >> 8 ]
        
    @staticmethod
    def fromByteArray(byteArray):
        enabled = byteArray[0] & 0x01
        driverId = byteArray[0] >> 1
        attack = byteArray[2] + (byteArray[3] << 8)
        sustain = byteArray[1]
        return SolenoidAction(enabled, driverId, attack, sustain)
    
    def __str__(self):
        return "SolenoidAction({}, {}, {}, {})".format(self.enabled, self.driverId, self.attack, self.sustain)
    
    def __repr__(self):
        return self.__str__()