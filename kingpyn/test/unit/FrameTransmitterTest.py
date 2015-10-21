'''
Created on Oct 11, 2015

@author: peter
'''
import unittest
from test.unit.MockSink import MockSink
from hdlc.EscapingSink import EscapingSink
from hdlc.FrameTransmitter import FrameTransmitter
from crc import crc_ccitt_update


class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testFrameTransmitterIsBornReady(self):
        tmp = MockSink()
        sink = EscapingSink(tmp)
        transmitter = FrameTransmitter(sink)
        self.assertTrue(transmitter.isReady())

    def testFrameTransmitterBecomesUnreadyWhenTransmitting(self):
        tmp = MockSink()
        sink = EscapingSink(tmp)
        transmitter = FrameTransmitter(sink)
        transmitter.transmit(FrameTransmitter.ACK() + 0x24)
        self.assertFalse(transmitter.isReady())

    def testFrameTransmitterTransmitsNothingByDefault(self):
        tmp = MockSink()
        sink = EscapingSink(tmp)
        transmitter = FrameTransmitter(sink)
        for i in range(0, 5):
            transmitter.schedule()
            sink.schedule()
            
        self.assertEqual([], tmp.items)
        
    def testFrameTransmitterTransmitsAckFrameCorrectly(self):
        tmp = MockSink()
        sink = EscapingSink(tmp)
        transmitter = FrameTransmitter(sink)
        transmitter.transmit(FrameTransmitter.ACK() + 0x24)
        
        for i in range(0, 5):
            transmitter.schedule()
            sink.schedule()
        
        crc = crc_ccitt_update(0xFFFF, FrameTransmitter.ACK() + 0x24)
        self.assertEqual([EscapingSink.FLAG(), FrameTransmitter.ACK() + 0x24, crc >> 8, crc & 0xFF, EscapingSink.FLAG()], tmp.items)
        
    def testFrameTransmitterTransmitsTwoFramesCorrectly(self):
        tmp = MockSink()
        sink = EscapingSink(tmp)
        transmitter = FrameTransmitter(sink)

        for j in range(0, 2):
            transmitter.transmit(FrameTransmitter.ACK() + 0x24)
            for i in range(0, 5):
                transmitter.schedule()
                sink.schedule()
            
            crc = crc_ccitt_update(0xFFFF, FrameTransmitter.ACK() + 0x24)
            self.assertEqual([EscapingSink.FLAG(), FrameTransmitter.ACK() + 0x24, crc >> 8, crc & 0xFF, EscapingSink.FLAG()], tmp.items)
            tmp.items = []

    def testFrameTransmitterBecomesReadyOnlyOnceTransmissionComplete(self):
        tmp = MockSink()
        sink = EscapingSink(tmp)
        transmitter = FrameTransmitter(sink)
        transmitter.transmit(FrameTransmitter.ACK() + 0x24)
        
        for i in range(0, 5):
            self.assertFalse(transmitter.isReady())
            transmitter.schedule()
            sink.schedule()
        
        self.assertTrue(transmitter.isReady())
            
    def testFrameTransmitterTransmitsUserFrameCorrectly(self):
        tmp = MockSink()
        sink = EscapingSink(tmp)
        transmitter = FrameTransmitter(sink)

        transmitter.transmit(0x00, [0x42])
        
        for i in range(0, 6):
            transmitter.schedule()
            sink.schedule()
            
        crc = crc_ccitt_update(0xFFFF, 0x00)
        crc = crc_ccitt_update(crc, 0x42)
        
        self.assertEqual([EscapingSink.FLAG(), 0x00, 0x42, crc >> 8, crc & 0xFF, EscapingSink.FLAG()], tmp.items)
        
if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()