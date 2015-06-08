'''
Created on Jun 8, 2015

@author: peter
'''
import unittest
from test.unit.MockOutgoingHardware import MockOutgoingHardware
from hdlc.OutgoingDataLink import OutgoingDataLink
from kingpyn.OutgoingFrameHandler import OutgoingFrameHandler
from kingpyn.OpCode import OpCode


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

        for _ in range(5):
            outgoingFrameHandler.schedule()
        
        self.assertEqual(OutgoingDataLink.FLAG(), hardware.outgoingBytes.popleft())
        self.assertEqual(OpCode.MY_ID(), hardware.outgoingBytes.popleft())
        self.assertEqual(5, hardware.outgoingBytes.popleft())
        self.assertEqual(OutgoingDataLink.FLAG(), hardware.outgoingBytes.popleft())

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()