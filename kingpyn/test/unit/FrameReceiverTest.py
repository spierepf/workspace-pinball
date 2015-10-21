'''
Created on Oct 10, 2015

@author: peter
'''
import unittest
from hdlc.crc import crc_ccitt_update
from hdlc.EscapingSource import EscapingSource
from test.unit.MockSource import MockSource
from hdlc.FrameReceiver import FrameReceiver
from _collections import deque

class FrameHandler(object):
    def __init__(self):
        self.header = None
        self.count = 0
        
    def handle(self, header, payload):
        self.count += 1
        self.header = header
        self.payload = payload
        pass
    
    def hasFrame(self):
        return self.header != None
        
class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testFrameReceiverReceivesFrame(self):
        medium = deque()
        medium += [EscapingSource.FLAG()]
        crc = 0xFFFF
        medium += [0x00]
        crc = crc_ccitt_update(crc, 0x00)
        medium += [0x42]
        crc = crc_ccitt_update(crc, 0x42)
        medium += [crc >> 8]
        medium += [crc & 0xFF]
        medium += [EscapingSource.FLAG()]
        
        source = EscapingSource(MockSource(medium))
        
        frameHandler = FrameHandler()
        frameReceiver = FrameReceiver(source)
        frameReceiver.setFrameHandler(frameHandler)
        
        source.schedule()
        while(source.isReady()):
            frameReceiver.schedule()
            source.schedule()
            
        self.assertEqual(0x00, frameHandler.header)
        self.assertEqual([0x42], frameHandler.payload)

    def testFrameReceiverIgnoresEmptyFrames(self):
        medium = deque()
        medium += [EscapingSource.FLAG()]
        medium += [EscapingSource.FLAG()]
        
        source = EscapingSource(MockSource(medium))
        
        frameHandler = FrameHandler()
        frameReceiver = FrameReceiver(source)
        frameReceiver.setFrameHandler(frameHandler)
        
        source.schedule()
        while(source.isReady()):
            frameReceiver.schedule()
            source.schedule()

        self.assertFalse(frameHandler.hasFrame())

    def testFrameReceiverIgnoresFrameWithBadCrc(self):
        medium = deque()
        medium += [EscapingSource.FLAG()]
        crc = 0xFFFF
        medium += [0x00]
        crc = crc_ccitt_update(crc, 0x00)
        medium += [0x42]
        crc = crc_ccitt_update(crc, 0x42)
        crc ^= 0x01
        medium += [crc >> 8]
        medium += [crc & 0xFF]
        medium += [EscapingSource.FLAG()]
        
        source = EscapingSource(MockSource(medium))
        
        frameHandler = FrameHandler()
        frameReceiver = FrameReceiver(source)
        frameReceiver.setFrameHandler(frameHandler)
        
        source.schedule()
        while(source.isReady()):
            frameReceiver.schedule()
            source.schedule()

        self.assertFalse(frameHandler.hasFrame())

    def testFrameReceiverReceivesMultipleFrames(self):
        medium = deque()
        
        count = 7
        for i in range(0, count):
            medium += [EscapingSource.FLAG()]
            crc = 0xFFFF
            medium += [0x00 + i]
            crc = crc_ccitt_update(crc, 0x00 + i)
            medium += [i]
            crc = crc_ccitt_update(crc, i)
            medium += [crc >> 8]
            medium += [crc & 0xFF]
            medium += [EscapingSource.FLAG()]
        
        source = EscapingSource(MockSource(medium))
        
        frameHandler = FrameHandler()
        frameReceiver = FrameReceiver(source)
        frameReceiver.setFrameHandler(frameHandler)
        
        source.schedule()
        while(source.isReady()):
            frameReceiver.schedule()
            source.schedule()
            
        self.assertEqual(count, frameHandler.count)

    def testFrameReceiverRecognizesAckFrames(self):
        medium = deque()
        medium += [EscapingSource.FLAG()]
        crc = 0xFFFF
        medium += [FrameReceiver.ACK() + 0x24]
        crc = crc_ccitt_update(crc, FrameReceiver.ACK() + 0x24)
        medium += [crc >> 8]
        medium += [crc & 0xFF]
        medium += [EscapingSource.FLAG()]
        
        source = EscapingSource(MockSource(medium))
        
        frameHandler = FrameHandler()
        frameReceiver = FrameReceiver(source)
        frameReceiver.setFrameHandler(frameHandler)
        
        source.schedule()
        while(source.isReady()):
            frameReceiver.schedule()
            source.schedule()
            
        self.assertEqual(FrameReceiver.ACK() + 0x24, frameHandler.header)
        self.assertEqual([], frameHandler.payload)

'''

class AckFrameHandler : public FrameHandler {
public:
    SequenceNumber lastAckReceived;

    virtual void handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) {
        lastAckReceived = header;
    }
};

BOOST_AUTO_TEST_CASE( test_FrameReceiver_recognizes_ack_frames ) {
    RingBuffer<64> medium;
    RingBufferReader tmp = RingBufferReader(medium);
    EscapingSource source = EscapingSource(tmp);
    AckFrameHandler handler;
    FrameReceiver receiver(source);
    receiver.setFrameHandler(&handler);

    medium.put(EscapingSource::FLAG);        // flag
    uint16_t crc = 0xFFFF;
    medium.put(FrameReceiver::ACK + 0x24);    // ack with sequence number
    crc_ccitt_update(crc, FrameReceiver::ACK + 0x24);
    medium.put(crc >> 8);                    // crc msb
    medium.put(crc & 0xFF);                    // crc lsb
    medium.put(EscapingSource::FLAG);        // flag

    while(!medium.isEmpty()) {
        source.schedule();
        receiver.schedule();
    }

    BOOST_CHECK(0x24 == handler.lastAckReceived);
}
'''

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()