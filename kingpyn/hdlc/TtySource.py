'''
Created on Oct 14, 2015

@author: peter
'''

class TtySource(object):
    '''
    classdocs
    '''


    def __init__(self, serial):
        '''
        Constructor
        '''
        self.serial = serial
        
    def isReady(self):
        return self.serial.inWaiting() > 0
    
    def read(self):
        return bytearray(self.serial.read())[0]