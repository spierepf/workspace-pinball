'''
Created on Jun 8, 2015

@author: peter
'''
import unittest
from test.unit.MockOutgoingHardware import MockOutgoingHardware
from hdlc.OutgoingDataLink import OutgoingDataLink
from kingpyn.OutgoingFrameHandler import OutgoingFrameHandler
from kingpyn.OpCode import OpCode
from hdlc.crc import crc_ccitt_update

class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testName(self):
        hardware = MockOutgoingHardware()
        datalink = OutgoingDataLink(hardware)
        outgoingFrameHandler = OutgoingFrameHandler(datalink)
        
        outgoingFrameHandler.postFrame(OpCode.MY_ID(), [5])

        for _ in range(7):
            outgoingFrameHandler.schedule()
        
        self.assertEqual(OutgoingDataLink.FLAG(), hardware.outgoingBytes.popleft())
        crc = 0xFFFF
        self.assertEqual(OpCode.MY_ID(), hardware.outgoingBytes.popleft())
        crc = crc_ccitt_update(crc, OpCode.MY_ID())
        self.assertEqual(5, hardware.outgoingBytes.popleft())
        crc = crc_ccitt_update(crc, 5)
        self.assertEqual(crc >> 8, hardware.outgoingBytes.popleft())
        self.assertEqual(crc & 0xFF, hardware.outgoingBytes.popleft())
        self.assertEqual(OutgoingDataLink.FLAG(), hardware.outgoingBytes.popleft())

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()