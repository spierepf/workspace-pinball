'''
Created on May 20, 2015

@author: peter
'''
import unittest
from hdlc.IncomingDataLink import IncomingDataLink
from collections import deque


class MockIncomingHardware(object):
    def __init__(self):
        self.incomingBytes = deque()
        
    def getReady(self):
        return len(self.incomingBytes) > 0
    
    def get(self):
        return self.incomingBytes.popleft()


class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testGet(self):
        hardware = MockIncomingHardware()
        datalink = IncomingDataLink(hardware)
        
        hardware.incomingBytes.append(0)
        
        datalink.schedule()

        self.assertEqual(0, datalink.peek(0))
        
    def testGetEscape(self):
        hardware = MockIncomingHardware()
        datalink = IncomingDataLink(hardware)

        hardware.incomingBytes.append(0x7d)
        hardware.incomingBytes.append(0x11 ^ 0x20)
        
        datalink.schedule()
        datalink.schedule()
        
        self.assertEqual(0x11, datalink.peek(0))

    def testGetFragmentedEscape(self):
        hardware = MockIncomingHardware()
        datalink = IncomingDataLink(hardware)

        hardware.incomingBytes.append(0x7d)

        datalink.schedule()
        
        hardware.incomingBytes.append(0x11 ^ 0x20)
        
        datalink.schedule()

        self.assertEqual(0x11, datalink.peek(0))

    def testHaveIncomingFrame(self):
        hardware = MockIncomingHardware()
        datalink = IncomingDataLink(hardware)

        self.assertFalse(datalink.haveIncomingFrame())
        
        hardware.incomingBytes.append(0x00);
        hardware.incomingBytes.append(0x7e);

        datalink.schedule();
        datalink.schedule();

        self.assertTrue(datalink.haveIncomingFrame())

    def testZeroLengthIncomingFrame(self):
        hardware = MockIncomingHardware()
        datalink = IncomingDataLink(hardware)

        self.assertFalse(datalink.haveIncomingFrame())
        
        hardware.incomingBytes.append(0x7e);

        datalink.schedule();

        self.assertFalse(datalink.haveIncomingFrame())

    def testIncomingFrameLength(self):
        hardware = MockIncomingHardware()
        datalink = IncomingDataLink(hardware)

        hardware.incomingBytes.append(0x00);
        hardware.incomingBytes.append(0x7e);

        datalink.schedule();
        datalink.schedule();

        self.assertEqual(1, datalink.incomingFrameLength())
    
    def testNextIncomingFrame(self):
        hardware = MockIncomingHardware()
        datalink = IncomingDataLink(hardware)

        hardware.incomingBytes.append(0x00);
        hardware.incomingBytes.append(0x7e);

        hardware.incomingBytes.append(0x01);
        hardware.incomingBytes.append(0x7e);
        
        datalink.schedule();
        datalink.schedule();
        datalink.schedule();
        datalink.schedule();

        datalink.nextIncomingFrame();
        
        self.assertTrue(datalink.haveIncomingFrame())
        self.assertEqual(1, datalink.peek(0))
        
if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testGet']
    unittest.main()