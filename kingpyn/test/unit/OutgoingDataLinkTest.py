'''
Created on May 21, 2015

@author: peter
'''
import unittest
from hdlc.OutgoingDataLink import OutgoingDataLink
from collections import deque


class MockOutgoingHardware(object):
    def __init__(self):
        self.outgoingBytes = deque()
        
    def putReady(self):
        return True

    def put(self, b):
        self.outgoingBytes.append(b)

class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testOutgoingFrame(self):
        hardware = MockOutgoingHardware()
        datalink = OutgoingDataLink(hardware)

        datalink.beginOutgoingFrame(0x00)
        datalink.appendPayload(0x01)
        datalink.endOutgoingFrame()

        for _ in range(4) : datalink.schedule()
        
        self.assertEqual(0x7e, hardware.outgoingBytes.popleft())
        self.assertEqual(0x00, hardware.outgoingBytes.popleft())
        self.assertEqual(0x01, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7e, hardware.outgoingBytes.popleft())

    def testTwoOutgoingFrame(self):
        hardware = MockOutgoingHardware()
        datalink = OutgoingDataLink(hardware)

        datalink.beginOutgoingFrame(0x00)
        datalink.appendPayload(0x01)
        datalink.endOutgoingFrame()

        datalink.beginOutgoingFrame(0x02)
        datalink.appendPayload(0x03)
        datalink.endOutgoingFrame()

        for _ in range(7) : datalink.schedule()
        
        self.assertEqual(0x7e, hardware.outgoingBytes.popleft())
        self.assertEqual(0x00, hardware.outgoingBytes.popleft())
        self.assertEqual(0x01, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7e, hardware.outgoingBytes.popleft())
        self.assertEqual(0x02, hardware.outgoingBytes.popleft())
        self.assertEqual(0x03, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7e, hardware.outgoingBytes.popleft())

    def testOutgoingFrameEscape(self):
        hardware = MockOutgoingHardware()
        datalink = OutgoingDataLink(hardware)

        datalink.beginOutgoingFrame(0x11);
        datalink.appendPayload(0x13);
        datalink.appendPayload(0x7d);
        datalink.appendPayload(0x7e);
        datalink.endOutgoingFrame();

        for _ in range(10) : datalink.schedule()

        self.assertEqual(0x7e, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7d, hardware.outgoingBytes.popleft())
        self.assertEqual(0x11 ^ 0x20, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7d, hardware.outgoingBytes.popleft())
        self.assertEqual(0x13 ^ 0x20, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7d, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7d ^ 0x20, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7d, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7e ^ 0x20, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7e, hardware.outgoingBytes.popleft())

    def testNoOutput(self):
        hardware = MockOutgoingHardware()
        datalink = OutgoingDataLink(hardware)

        datalink.schedule()
        
        self.assertEqual(0, len(hardware.outgoingBytes))

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testOutgoingFrame']
    unittest.main()