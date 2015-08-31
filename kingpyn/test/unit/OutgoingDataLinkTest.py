'''
Created on May 21, 2015

@author: peter
'''
import unittest
from hdlc.OutgoingDataLink import OutgoingDataLink
from test.unit.MockOutgoingHardware import MockOutgoingHardware
from hdlc.crc import crc_ccitt_update

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

        for _ in range(6) : datalink.schedule()
        
        self.assertEqual(0x7e, hardware.outgoingBytes.popleft())
        crc = 0xFFFF
        self.assertEqual(0x00, hardware.outgoingBytes.popleft())
        crc = crc_ccitt_update(crc, 0x00)
        self.assertEqual(0x01, hardware.outgoingBytes.popleft())
        crc = crc_ccitt_update(crc, 0x01)
        self.assertEqual(crc >> 8, hardware.outgoingBytes.popleft())
        self.assertEqual(crc & 0xFF, hardware.outgoingBytes.popleft())
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

        for _ in range(11) : datalink.schedule()
        
        self.assertEqual(0x7e, hardware.outgoingBytes.popleft())
        crc = 0xFFFF
        self.assertEqual(0x00, hardware.outgoingBytes.popleft())
        crc = crc_ccitt_update(crc, 0x00)
        self.assertEqual(0x01, hardware.outgoingBytes.popleft())
        crc = crc_ccitt_update(crc, 0x01)
        self.assertEqual(crc >> 8, hardware.outgoingBytes.popleft())
        self.assertEqual(crc & 0xFF, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7e, hardware.outgoingBytes.popleft())
        crc = 0xFFFF
        self.assertEqual(0x02, hardware.outgoingBytes.popleft())
        crc = crc_ccitt_update(crc, 0x02)
        self.assertEqual(0x03, hardware.outgoingBytes.popleft())
        crc = crc_ccitt_update(crc, 0x03)
        self.assertEqual(crc >> 8, hardware.outgoingBytes.popleft())
        self.assertEqual(crc & 0xFF, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7e, hardware.outgoingBytes.popleft())

    def testOutgoingFrameEscape(self):
        hardware = MockOutgoingHardware()
        datalink = OutgoingDataLink(hardware)

        datalink.beginOutgoingFrame(0x11);
        datalink.appendPayload(0x13);
        datalink.appendPayload(0x7d);
        datalink.appendPayload(0x7e);
        datalink.endOutgoingFrame();

        for _ in range(12) : datalink.schedule()

        self.assertEqual(0x7e, hardware.outgoingBytes.popleft())
        crc = 0xFFFF
        self.assertEqual(0x7d, hardware.outgoingBytes.popleft())
        self.assertEqual(0x11 ^ 0x20, hardware.outgoingBytes.popleft())
        crc = crc_ccitt_update(crc, 0x11)
        self.assertEqual(0x7d, hardware.outgoingBytes.popleft())
        self.assertEqual(0x13 ^ 0x20, hardware.outgoingBytes.popleft())
        crc = crc_ccitt_update(crc, 0x13)
        self.assertEqual(0x7d, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7d ^ 0x20, hardware.outgoingBytes.popleft())
        crc = crc_ccitt_update(crc, 0x7d)
        self.assertEqual(0x7d, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7e ^ 0x20, hardware.outgoingBytes.popleft())
        crc = crc_ccitt_update(crc, 0x7e)
        self.assertEqual(crc >> 8, hardware.outgoingBytes.popleft())
        self.assertEqual(crc & 0xFF, hardware.outgoingBytes.popleft())
        self.assertEqual(0x7e, hardware.outgoingBytes.popleft())

    def testNoOutput(self):
        hardware = MockOutgoingHardware()
        datalink = OutgoingDataLink(hardware)

        datalink.schedule()
        
        self.assertEqual(0, len(hardware.outgoingBytes))

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testOutgoingFrame']
    unittest.main()