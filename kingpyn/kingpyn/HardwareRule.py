'''
Created on May 28, 2015

@author: peter
'''
from kingpyn.Stimulus import Stimulus
from kingpyn.SolenoidAction import SolenoidAction

class HardwareRule(object):
    '''
    classdocs
    '''


    def __init__(self, stimulus, solenoidAction):
        '''
        Constructor
        '''
        self.stimulus = stimulus
        self.solenoidAction = solenoidAction
    
    def __eq__(self, other):
        if isinstance(other, HardwareRule):
            return self.stimulus == other.stimulus and self.solenoidAction == other.solenoidAction
        return NotImplemented
    
    def __hash__(self):
        return hash(self.stimulus) ^ hash(self.solenoidAction)
    
    def toByteArray(self):
        byteArray = self.stimulus.toByteArray()
        byteArray.extend(self.solenoidAction.toByteArray())
        return byteArray
    
    @staticmethod
    def fromByteArray(byteArray):
        stimulus = Stimulus.fromByteArray([byteArray[0]])
        solenoidAction = SolenoidAction.fromByteArray([byteArray[1], byteArray[2], byteArray[3], byteArray[4]])
        return HardwareRule(stimulus, solenoidAction)
        
        