'''
Created on Oct 11, 2015

@author: peter
'''
import unittest

from hdlc.EscapingSink import EscapingSink
from test.unit.MockSink import MockSink

class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testEscapingSinkIsBornReady(self):
        mockSink = MockSink()
        escapingSink = EscapingSink(mockSink)
        self.assertTrue(escapingSink.isReady())

    def testEscapingSinkBecomesUnreadyWhenTransmitting(self):
        mockSink = MockSink()
        escapingSink = EscapingSink(mockSink)
        escapingSink.write(0x42)
        self.assertFalse(escapingSink.isReady())

    def testEscapingSinkBecomesReadyAfterTransmitting(self):
        mockSink = MockSink()
        escapingSink = EscapingSink(mockSink)
        escapingSink.write(0x42)
        escapingSink.schedule()
        self.assertTrue(escapingSink.isReady())

    def testEscapingSinkTransmitsOctetCorrectly(self):
        mockSink = MockSink()
        escapingSink = EscapingSink(mockSink)
        escapingSink.write(0x42)
        escapingSink.schedule()
        self.assertTrue(escapingSink.isReady())
        self.assertTrue([0x42] == mockSink.items)

    def testEscapingSinkTransmitsEscapableOctetCorrectly(self):
        mockSink = MockSink()
        escapingSink = EscapingSink(mockSink)
        escapingSink.write(EscapingSink.FLAG())
        escapingSink.schedule()
        escapingSink.write(EscapingSink.ESCAPE())
        escapingSink.schedule()
        self.assertTrue(escapingSink.isReady())
        self.assertEqual([EscapingSink.ESCAPE(), EscapingSink.FLAG() ^ EscapingSink.MASK(), EscapingSink.ESCAPE(), EscapingSink.ESCAPE() ^ EscapingSink.MASK()], mockSink.items)

    def testEscapingSinkTransmitsFlagOctetCorrectly(self):
        mockSink = MockSink()
        escapingSink = EscapingSink(mockSink)
        escapingSink.writeFlag()
        escapingSink.schedule()
        self.assertTrue(escapingSink.isReady())
        self.assertEqual([EscapingSink.FLAG()], mockSink.items)

'''
BOOST_AUTO_TEST_CASE( test_EscapingSink_transmits_flag_octet_correctly ) {
    RingBuffer<64> medium;
    RingBufferWriter sink = RingBufferWriter(medium);
    EscapingSink escapingSink = EscapingSink(sink);

    escapingSink.writeFlag();
    escapingSink.schedule();

    BOOST_CHECK(EscapingSink::FLAG == medium.get());
    BOOST_CHECK(medium.isEmpty());
}
'''

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()