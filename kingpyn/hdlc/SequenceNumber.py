'''
Created on Oct 13, 2015

@author: peter
'''

class SequenceNumber(object):
    '''
    classdocs
    '''


    def __init__(self, value = 0):
        '''
        Constructor
        '''
        self.value = value & 0x3F
        
    def __ne__(self, other):
        return self.value != other.value
        
    def __iadd__(self, other):
        self.value = (self.value + other) & 0x3F
        return self