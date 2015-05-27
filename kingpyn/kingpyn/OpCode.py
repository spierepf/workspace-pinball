'''
Created on May 21, 2015

@author: peter
'''

class OpCode(object):
    '''
    classdocs
    '''

    @staticmethod
    def MY_ID():
        return 0x00

    @staticmethod
    def PING():
        return 0x01

    @staticmethod
    def PONG():
        return 0x02

    @staticmethod
    def LOG():
        return 0x03

    @staticmethod
    def SWITCH_ACTIVE():
        return 0x10

    @staticmethod
    def SWITCH_INACTIVE():
        return 0x11

    @staticmethod
    def SR_INHIBIT():
        return 0x20

    @staticmethod
    def SR_ENABLE():
        return 0x21

    @staticmethod
    def SR_CONFIG():
        return 0x22
        