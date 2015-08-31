'''
Created on Jun 8, 2015

@author: peter
'''
import unittest
from test.unit.MockIncomingHardware import MockIncomingHardware
from hdlc.IncomingDataLink import IncomingDataLink
from kingpyn.IncomingFrameHandler import IncomingFrameHandler
from kingpyn.Observer import Observer
from kingpyn.OpCode import OpCode
from hdlc.crc import crc_ccitt_update

class MockFrameObserver(Observer):
    def __init__(self):
        self.receivedFrameCount = 0

    def update(self, observable, arg):
        self.receivedFrameCount = self.receivedFrameCount + 1
        self.receivedFrame = arg


class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testName(self):
        hardware = MockIncomingHardware()
        hardware.incomingBytes.append(IncomingDataLink.FLAG())
        crc = 0xFFFF
        hardware.incomingBytes.append(OpCode.MY_ID())
        crc = crc_ccitt_update(crc, OpCode.MY_ID())
        hardware.incomingBytes.append(5)
        crc = crc_ccitt_update(crc, 5)
        hardware.incomingBytes.append(crc >> 8)
        hardware.incomingBytes.append(crc & 0xFF)
        hardware.incomingBytes.append(IncomingDataLink.FLAG())
        
        datalink = IncomingDataLink(hardware)
        incomingFrameHandler = IncomingFrameHandler(datalink)
        observer = MockFrameObserver()
        incomingFrameHandler.frameNotifier.addObserver(observer)
        
        for _ in range(7):
            incomingFrameHandler.schedule()
        
        self.assertEquals(1, observer.receivedFrameCount)
        self.assertEquals((OpCode.MY_ID(), [5]), observer.receivedFrame)

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()