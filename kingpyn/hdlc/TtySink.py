'''
Created on Oct 14, 2015

@author: peter
'''

class TtySink(object):
    '''
    classdocs
    '''


    def __init__(self, serial):
        '''
        Constructor
        '''
        self.serial = serial
        
    def isReady(self):
        return True
    
    def write(self, octet):
        tmp = bytearray()
        tmp.append(octet)
        return self.serial.write(tmp)