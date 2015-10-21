'''
Created on Oct 11, 2015

@author: peter
'''
import unittest
from hdlc.EndPoint import EndPoint
from unit.MockSink import MockSink
from unit.MockSource import MockSource
from hdlc.EscapingSink import EscapingSink
from hdlc.EscapingSource import EscapingSource
from collections import deque
from hdlc.FrameTransmitter import FrameTransmitter
from hdlc.FrameReceiver import FrameReceiver
from hdlc.crc import crc_ccitt_update
from test.unit.NoisySink import NoisySink
from random import seed

class FrameHandler(object):
    def __init__(self):
        self.headers = deque()
        self.payloads = deque()
        
    def handle(self, header, payload):
        self.headers.append(header)
        self.payloads.append(payload)
        pass        

class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testEndPointIsBornDisconnected(self):
        mediumAB = deque()
        mediumBA = deque()

        sinkA = MockSink(mediumAB)
        sinkB = MockSink(mediumBA)
        readerA = MockSource(mediumBA)
        readerB = MockSource(mediumAB)
        
        escapingSink = [EscapingSink(sinkA), EscapingSink(sinkB)]
        escapingSource = [EscapingSource(readerA), EscapingSource(readerB)]
                
        outgoingFrameBuffer = [deque(), deque()]
        
        frameTransmitter = [FrameTransmitter(escapingSink[0]), FrameTransmitter(escapingSink[1])]
        userFrameHandler = [FrameHandler(), FrameHandler()]
        frameReceiver = [FrameReceiver(escapingSource[0]), FrameReceiver(escapingSource[1])]

        endPoint = [
            EndPoint(escapingSource[0], frameReceiver[0], userFrameHandler[0], outgoingFrameBuffer[0], frameTransmitter[0], escapingSink[0]),
            EndPoint(escapingSource[1], frameReceiver[1], userFrameHandler[1], outgoingFrameBuffer[1], frameTransmitter[1], escapingSink[1])
        ]

        frameReceiver[0].setFrameHandler(endPoint[0])
        frameReceiver[1].setFrameHandler(endPoint[1])
        
        self.assertFalse(endPoint[0].isConnected())
        
    def testEndPointConnectDoesWhatItSaysOnTheBox(self):
        mediumAB = deque()
        mediumBA = deque()

        sinkA = MockSink(mediumAB)
        sinkB = MockSink(mediumBA)
        readerA = MockSource(mediumBA)
        readerB = MockSource(mediumAB)
        
        escapingSink = [EscapingSink(sinkA), EscapingSink(sinkB)]
        escapingSource = [EscapingSource(readerA), EscapingSource(readerB)]
                
        outgoingFrameBuffer = [deque(), deque()]
        
        frameTransmitter = [FrameTransmitter(escapingSink[0]), FrameTransmitter(escapingSink[1])]
        userFrameHandler = [FrameHandler(), FrameHandler()]
        frameReceiver = [FrameReceiver(escapingSource[0]), FrameReceiver(escapingSource[1])]

        endPoint = [
            EndPoint(escapingSource[0], frameReceiver[0], userFrameHandler[0], outgoingFrameBuffer[0], frameTransmitter[0], escapingSink[0]),
            EndPoint(escapingSource[1], frameReceiver[1], userFrameHandler[1], outgoingFrameBuffer[1], frameTransmitter[1], escapingSink[1])
        ]
        
        frameReceiver[0].setFrameHandler(endPoint[0])
        frameReceiver[1].setFrameHandler(endPoint[1])
        
        endPoint[0].connect();
    
        for i in range(0, 100):
            endPoint[0].schedule()
            endPoint[1].schedule()
    
        self.assertTrue(endPoint[0].isConnected());
        self.assertTrue(endPoint[1].isConnected());

    def testEndPointConnectDoesNothingWhileDisconnected(self):
        mediumAB = deque()
        mediumBA = deque()

        sinkA = MockSink(mediumAB)
        sinkB = MockSink(mediumBA)
        readerA = MockSource(mediumBA)
        readerB = MockSource(mediumAB)
        
        escapingSink = [EscapingSink(sinkA), EscapingSink(sinkB)]
        escapingSource = [EscapingSource(readerA), EscapingSource(readerB)]
                
        outgoingFrameBuffer = [deque(), deque()]
        
        frameTransmitter = [FrameTransmitter(escapingSink[0]), FrameTransmitter(escapingSink[1])]
        userFrameHandler = [FrameHandler(), FrameHandler()]
        frameReceiver = [FrameReceiver(escapingSource[0]), FrameReceiver(escapingSource[1])]

        endPoint = [
            EndPoint(escapingSource[0], frameReceiver[0], userFrameHandler[0], outgoingFrameBuffer[0], frameTransmitter[0], escapingSink[0]),
            EndPoint(escapingSource[1], frameReceiver[1], userFrameHandler[1], outgoingFrameBuffer[1], frameTransmitter[1], escapingSink[1])
        ]
        
        frameReceiver[0].setFrameHandler(endPoint[0])
        frameReceiver[1].setFrameHandler(endPoint[1])

        outgoingFrameBuffer[0] += [[0x42]]
                
        for i in range(0, 100):
            endPoint[0].schedule()
            endPoint[1].schedule()
    
        self.assertTrue(0 == len(userFrameHandler[0].payloads));
        self.assertTrue(0 == len(userFrameHandler[1].payloads));

    def testEndPointConnectDoesNothingWhenGivenNoFrames(self):
        mediumAB = deque()
        mediumBA = deque()

        sinkA = MockSink(mediumAB)
        sinkB = MockSink(mediumBA)
        readerA = MockSource(mediumBA)
        readerB = MockSource(mediumAB)
        
        escapingSink = [EscapingSink(sinkA), EscapingSink(sinkB)]
        escapingSource = [EscapingSource(readerA), EscapingSource(readerB)]
                
        outgoingFrameBuffer = [deque(), deque()]
        
        frameTransmitter = [FrameTransmitter(escapingSink[0]), FrameTransmitter(escapingSink[1])]
        userFrameHandler = [FrameHandler(), FrameHandler()]
        frameReceiver = [FrameReceiver(escapingSource[0]), FrameReceiver(escapingSource[1])]

        endPoint = [
            EndPoint(escapingSource[0], frameReceiver[0], userFrameHandler[0], outgoingFrameBuffer[0], frameTransmitter[0], escapingSink[0]),
            EndPoint(escapingSource[1], frameReceiver[1], userFrameHandler[1], outgoingFrameBuffer[1], frameTransmitter[1], escapingSink[1])
        ]
        
        frameReceiver[0].setFrameHandler(endPoint[0])
        frameReceiver[1].setFrameHandler(endPoint[1])
        
        endPoint[0].connect();
    
        for i in range(0, 100):
            endPoint[0].schedule()
            endPoint[1].schedule()
    
        self.assertTrue(0 == len(userFrameHandler[0].payloads));
        self.assertTrue(0 == len(userFrameHandler[1].payloads));

    def testEndPointMovesOneFrame(self):
        mediumAB = deque()
        mediumBA = deque()

        sinkA = MockSink(mediumAB)
        sinkB = MockSink(mediumBA)
        readerA = MockSource(mediumBA)
        readerB = MockSource(mediumAB)
        
        escapingSink = [EscapingSink(sinkA), EscapingSink(sinkB)]
        escapingSource = [EscapingSource(readerA), EscapingSource(readerB)]
                
        outgoingFrameBuffer = [deque(), deque()]
        
        frameTransmitter = [FrameTransmitter(escapingSink[0]), FrameTransmitter(escapingSink[1])]
        userFrameHandler = [FrameHandler(), FrameHandler()]
        frameReceiver = [FrameReceiver(escapingSource[0]), FrameReceiver(escapingSource[1])]

        endPoint = [
            EndPoint(escapingSource[0], frameReceiver[0], userFrameHandler[0], outgoingFrameBuffer[0], frameTransmitter[0], escapingSink[0]),
            EndPoint(escapingSource[1], frameReceiver[1], userFrameHandler[1], outgoingFrameBuffer[1], frameTransmitter[1], escapingSink[1])
        ]
        
        frameReceiver[0].setFrameHandler(endPoint[0])
        frameReceiver[1].setFrameHandler(endPoint[1])
        
        endPoint[0].connect();

        while not endPoint[1].isConnected():
            endPoint[0].schedule();
            endPoint[1].schedule();
    
        outgoingFrameBuffer[0] += [[0x42]]
        
        for i in range(0, 100):
            endPoint[0].schedule()
            endPoint[1].schedule()

        self.assertTrue(0 == len(outgoingFrameBuffer[0]))
        self.assertTrue(1 == len(userFrameHandler[1].payloads))
        self.assertEqual([0x42], userFrameHandler[1].payloads[0])

    def testEndPointMovesSeventyFrames(self):
        mediumAB = deque()
        mediumBA = deque()

        sinkA = MockSink(mediumAB)
        sinkB = MockSink(mediumBA)
        readerA = MockSource(mediumBA)
        readerB = MockSource(mediumAB)
        
        escapingSink = [EscapingSink(sinkA), EscapingSink(sinkB)]
        escapingSource = [EscapingSource(readerA), EscapingSource(readerB)]
                
        outgoingFrameBuffer = [deque(), deque()]
        
        frameTransmitter = [FrameTransmitter(escapingSink[0]), FrameTransmitter(escapingSink[1])]
        userFrameHandler = [FrameHandler(), FrameHandler()]
        frameReceiver = [FrameReceiver(escapingSource[0]), FrameReceiver(escapingSource[1])]

        endPoint = [
            EndPoint(escapingSource[0], frameReceiver[0], userFrameHandler[0], outgoingFrameBuffer[0], frameTransmitter[0], escapingSink[0]),
            EndPoint(escapingSource[1], frameReceiver[1], userFrameHandler[1], outgoingFrameBuffer[1], frameTransmitter[1], escapingSink[1])
        ]
        
        frameReceiver[0].setFrameHandler(endPoint[0])
        frameReceiver[1].setFrameHandler(endPoint[1])
        
        endPoint[0].connect();

        while not endPoint[1].isConnected():
            endPoint[0].schedule();
            endPoint[1].schedule();
    
        frameCount = 70;
        for i in range(0, frameCount) :
            outgoingFrameBuffer[0] += [[0x42 + i]];
        
        for i in range(0, 1600):
            endPoint[0].schedule()
            endPoint[1].schedule()

        self.assertTrue(0 == len(outgoingFrameBuffer[0]))
        self.assertTrue(frameCount == len(userFrameHandler[1].payloads))
        for i in range(0, frameCount):
            self.assertEqual([0x42 + i], userFrameHandler[1].payloads[i])

    def testEndPointMovesOneFrameInEachDirection(self):
        mediumAB = deque()
        mediumBA = deque()

        sinkA = MockSink(mediumAB)
        sinkB = MockSink(mediumBA)
        readerA = MockSource(mediumBA)
        readerB = MockSource(mediumAB)
        
        escapingSink = [EscapingSink(sinkA), EscapingSink(sinkB)]
        escapingSource = [EscapingSource(readerA), EscapingSource(readerB)]
                
        outgoingFrameBuffer = [deque(), deque()]
        
        frameTransmitter = [FrameTransmitter(escapingSink[0]), FrameTransmitter(escapingSink[1])]
        userFrameHandler = [FrameHandler(), FrameHandler()]
        frameReceiver = [FrameReceiver(escapingSource[0]), FrameReceiver(escapingSource[1])]

        endPoint = [
            EndPoint(escapingSource[0], frameReceiver[0], userFrameHandler[0], outgoingFrameBuffer[0], frameTransmitter[0], escapingSink[0]),
            EndPoint(escapingSource[1], frameReceiver[1], userFrameHandler[1], outgoingFrameBuffer[1], frameTransmitter[1], escapingSink[1])
        ]
        
        frameReceiver[0].setFrameHandler(endPoint[0])
        frameReceiver[1].setFrameHandler(endPoint[1])
        
        endPoint[0].connect();

        while not endPoint[1].isConnected():
            endPoint[0].schedule();
            endPoint[1].schedule();
    
        outgoingFrameBuffer[0] += [[0x42]]
        outgoingFrameBuffer[1] += [[0x43]]
        
        for i in range(0, 100):
            endPoint[0].schedule()
            endPoint[1].schedule()

        self.assertTrue(0 == len(outgoingFrameBuffer[0]))
        self.assertTrue(1 == len(userFrameHandler[1].payloads))
        self.assertEqual([0x42], userFrameHandler[1].payloads[0])

        self.assertTrue(0 == len(outgoingFrameBuffer[1]))
        self.assertTrue(1 == len(userFrameHandler[0].payloads))
        self.assertEqual([0x43], userFrameHandler[0].payloads[0])

    def testEndPointIgnoresFramesWithUnexpectedSequenceNumber(self):
        mediumAB = deque()
        mediumBA = deque()

        sinkA = MockSink(mediumAB)
        sinkB = MockSink(mediumBA)
        readerA = MockSource(mediumBA)
        readerB = MockSource(mediumAB)
        
        escapingSink = [EscapingSink(sinkA), EscapingSink(sinkB)]
        escapingSource = [EscapingSource(readerA), EscapingSource(readerB)]
                
        outgoingFrameBuffer = [deque(), deque()]
        
        frameTransmitter = [FrameTransmitter(escapingSink[0]), FrameTransmitter(escapingSink[1])]
        userFrameHandler = [FrameHandler(), FrameHandler()]
        frameReceiver = [FrameReceiver(escapingSource[0]), FrameReceiver(escapingSource[1])]

        endPoint = [
            EndPoint(escapingSource[0], frameReceiver[0], userFrameHandler[0], outgoingFrameBuffer[0], frameTransmitter[0], escapingSink[0]),
            EndPoint(escapingSource[1], frameReceiver[1], userFrameHandler[1], outgoingFrameBuffer[1], frameTransmitter[1], escapingSink[1])
        ]
        
        endPoint[0].connected.timeout = 0x0000000F
        endPoint[1].connected.timeout = 0x0000000F
        
        frameReceiver[0].setFrameHandler(endPoint[0])
        frameReceiver[1].setFrameHandler(endPoint[1])
        
        endPoint[0].connect();
    
        while not endPoint[1].isConnected():
            endPoint[0].schedule();
            endPoint[1].schedule();
    
        self.assertTrue(0 == len(userFrameHandler[0].payloads));
        self.assertTrue(0 == len(userFrameHandler[1].payloads));

        mediumAB += [EscapingSource.FLAG()]
        crc = 0xFFFF
        mediumAB += [0x00 ^ 0x01]
        crc = crc_ccitt_update(crc, 0x00 ^ 0x01)
        mediumAB += [0x42]
        crc = crc_ccitt_update(crc, 0x42)
        mediumAB += [crc >> 8]
        mediumAB += [crc & 0xFF]
        mediumAB += [EscapingSource.FLAG()]

        while len(mediumAB) > 0:
            endPoint[1].schedule()
    
        self.assertTrue(len(userFrameHandler[1].payloads) == 0)

    def testEndPointMovesOneHundredFramesInEachDirectionWithNoise(self):
        seed(1)
        mediumAB = deque()
        mediumBA = deque()

        writerA = MockSink(mediumAB)
        sinkA = NoisySink(writerA)
        writerB = MockSink(mediumBA)
        sinkB = NoisySink(writerB)
        
        readerA = MockSource(mediumBA)
        readerB = MockSource(mediumAB)
        
        escapingSink = [EscapingSink(sinkA), EscapingSink(sinkB)]
        escapingSource = [EscapingSource(readerA), EscapingSource(readerB)]
                
        outgoingFrameBuffer = [deque(), deque()]
        
        frameTransmitter = [FrameTransmitter(escapingSink[0]), FrameTransmitter(escapingSink[1])]
        userFrameHandler = [FrameHandler(), FrameHandler()]
        frameReceiver = [FrameReceiver(escapingSource[0]), FrameReceiver(escapingSource[1])]

        endPoint = [
            EndPoint(escapingSource[0], frameReceiver[0], userFrameHandler[0], outgoingFrameBuffer[0], frameTransmitter[0], escapingSink[0]),
            EndPoint(escapingSource[1], frameReceiver[1], userFrameHandler[1], outgoingFrameBuffer[1], frameTransmitter[1], escapingSink[1])
        ]
        
        frameReceiver[0].setFrameHandler(endPoint[0])
        frameReceiver[1].setFrameHandler(endPoint[1])
        
        endPoint[0].connect();

        while not endPoint[1].isConnected():
            endPoint[0].schedule();
            endPoint[1].schedule();
    
        frameCount = 100
        for i in range(0, frameCount):
            outgoingFrameBuffer[0] += [[i]]
            outgoingFrameBuffer[1] += [[i]]
        
        while len(userFrameHandler[0].payloads) < frameCount or len(userFrameHandler[1].payloads) < frameCount:
            endPoint[0].schedule()
            endPoint[1].schedule()

        for i in range(0, frameCount):
            self.assertEqual([i], userFrameHandler[0].payloads[i])
            self.assertEqual([i], userFrameHandler[1].payloads[i])

'''

BOOST_AUTO_TEST_CASE( test_EndPoint_moves_one_hundred_frames_in_each_direction_with_noise ) {
    RingBuffer<64> mediumAB;
    RingBuffer<64> mediumBA;

    RingBufferWriter writerA = RingBufferWriter(mediumAB);
    NoisySink sinkA = NoisySink(writerA);
    RingBufferWriter writerB = RingBufferWriter(mediumBA);
    NoisySink sinkB = NoisySink(writerB);
    RingBufferReader readerA = RingBufferReader(mediumBA);
    RingBufferReader readerB = RingBufferReader(mediumAB);

    EscapingSink sink[2] = {
            EscapingSink(sinkA),
            EscapingSink(sinkB)
    };

    EscapingSource source[2] = {
            EscapingSource(readerA),
            EscapingSource(readerB)
    };

    FrameBuffer outgoingFrameBuffer[2];
    FrameBuffer incomingFrameBuffer[2];

    FrameTransmitter frameTransmitter[2] = {
            FrameTransmitter(sink[A]),
            FrameTransmitter(sink[B])
    };

    FrameBufferUserFrameHandler userFrameHandler[2] = {
            FrameBufferUserFrameHandler(incomingFrameBuffer[A]),
            FrameBufferUserFrameHandler(incomingFrameBuffer[B])
    };

    FrameReceiver frameReceiver[2] = {
            FrameReceiver(source[A]),
            FrameReceiver(source[B])
    };

    EndPoint endPoint[2] = {
            EndPoint(source[A], frameReceiver[A], userFrameHandler[A], outgoingFrameBuffer[A], frameTransmitter[A], sink[A]),
            EndPoint(source[B], frameReceiver[B], userFrameHandler[B], outgoingFrameBuffer[B], frameTransmitter[B], sink[B])
    };

    frameReceiver[A].setFrameHandler(&(endPoint[A]));
    frameReceiver[B].setFrameHandler(&(endPoint[B]));

    endPoint[A].connect();
    while(!endPoint[B].isConnected()) {
        endPoint[A].schedule();
        endPoint[B].schedule();
    }

    int framesSent[2] = { 0, 0 };
    int framesReceived[2] = { 0, 0 };

    int count = 0;
    while(framesReceived[A] != 100 || framesReceived[B] != 100) {
        count++;
        for(int i = 0; i < 2; i++) {
            while(!outgoingFrameBuffer[i].isFull() && framesSent[i] < 100) {
                outgoingFrameBuffer[i].put(framesSent[i]++);
                outgoingFrameBuffer[i].endFrame();
            }
        }
        endPoint[A].schedule();
        endPoint[B].schedule();

        for(int i = 0; i < 2; i++) {
            while(!incomingFrameBuffer[i].isEmpty()) {
                BOOST_CHECK(1 == incomingFrameBuffer[i][0].size());
                BOOST_CHECK(framesReceived[i]++ == incomingFrameBuffer[i][0][0]);
                incomingFrameBuffer[i].removeFrame();
            }
        }
    }
    std::cout << count << std::endl;
}
'''

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()