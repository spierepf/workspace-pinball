'''
Created on May 20, 2015

@author: peter
'''

import serial

class Tty(object):
    '''
    classdocs
    '''


    def __init__(self, port):
        '''
        Constructor
        '''
        self.serial = serial.Serial(port, 9600)

    def getReady(self):
        '''
        returns true if a byte is available from the hardware
        '''
        return self.serial.inWaiting() > 0

    def get(self):
        '''
        returns the next byte from the hardware
        '''
        return bytearray(self.serial.read())[0]

    def putReady(self):
        '''
        returns true if there is space in the outgoing buffer for a put
        '''
        return True

    def put(self, byte):
        '''
        sends the given byte over the tty
        '''
        tmp = bytearray()
        tmp.append(byte)
        return self.serial.write(tmp)        