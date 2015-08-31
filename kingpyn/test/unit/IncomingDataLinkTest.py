'''
Created on May 20, 2015

@author: peter
'''
import unittest
from hdlc.IncomingDataLink import IncomingDataLink
from test.unit.MockIncomingHardware import MockIncomingHardware

from hdlc.crc import crc_ccitt_update

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
        
        crc = 0xFFFF
        hardware.incomingBytes.append(0x00)
        crc = crc_ccitt_update(crc, 0x00)
        hardware.incomingBytes.append(crc >> 8)
        hardware.incomingBytes.append(crc & 0xFF)
        hardware.incomingBytes.append(0x7e)

        datalink.schedule()
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()

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

        crc = 0xFFFF
        hardware.incomingBytes.append(0x00)
        crc = crc_ccitt_update(crc, 0x00)
        hardware.incomingBytes.append(crc >> 8)
        hardware.incomingBytes.append(crc & 0xFF)
        hardware.incomingBytes.append(0x7e)

        datalink.schedule()
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()

        self.assertEqual(1, datalink.incomingFrameLength())
    
    def testNextIncomingFrame(self):
        hardware = MockIncomingHardware()
        datalink = IncomingDataLink(hardware)

        crc = 0xFFFF
        hardware.incomingBytes.append(0x00)
        crc = crc_ccitt_update(crc, 0x00)
        hardware.incomingBytes.append(crc >> 8)
        hardware.incomingBytes.append(crc & 0xFF)
        hardware.incomingBytes.append(0x7e)

        crc = 0xFFFF
        hardware.incomingBytes.append(0x01)
        crc = crc_ccitt_update(crc, 0x01)
        hardware.incomingBytes.append(crc >> 8)
        hardware.incomingBytes.append(crc & 0xFF)
        hardware.incomingBytes.append(0x7e)
        
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()

        datalink.nextIncomingFrame();
        
        self.assertTrue(datalink.haveIncomingFrame())
        self.assertEqual(1, datalink.peek(0))

        datalink.nextIncomingFrame();

        self.assertFalse(datalink.haveIncomingFrame())

    def testCorruptIncomingFrame(self):
        hardware = MockIncomingHardware()
        datalink = IncomingDataLink(hardware)

        crc = 0xFFFF
        hardware.incomingBytes.append(0x00)
        crc = crc_ccitt_update(crc, 0x00)
        hardware.incomingBytes.append(crc >> 8)
        hardware.incomingBytes.append((crc & 0xFF) ^ 0x01) # corruption!
        hardware.incomingBytes.append(0x7e)

        crc = 0xFFFF
        hardware.incomingBytes.append(0x01)
        crc = crc_ccitt_update(crc, 0x01)
        hardware.incomingBytes.append(crc >> 8)
        hardware.incomingBytes.append(crc & 0xFF)
        hardware.incomingBytes.append(0x7e)
        
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()
        datalink.schedule()

        self.assertTrue(datalink.haveIncomingFrame())
        self.assertEqual(1, datalink.peek(0))

        datalink.nextIncomingFrame();

        self.assertFalse(datalink.haveIncomingFrame())

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testGet']
    unittest.main()