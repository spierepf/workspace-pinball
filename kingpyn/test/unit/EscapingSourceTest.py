'''
Created on Oct 10, 2015

@author: peter
'''
import unittest
from test.unit.MockSource import MockSource
from EscapingSource import EscapingSource
from collections import deque

class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testEscapingSourceIsBornUnready(self):
        mock = MockSource(deque([0x42]))
        escapingSource = EscapingSource(mock)
        self.assertFalse(escapingSource.isReady())
        
    def testEscapingSourceBecomesReadyAfterReceiving(self):
        mock = MockSource(deque([0x42]))
        escapingSource = EscapingSource(mock)
        escapingSource.schedule()
        self.assertTrue(escapingSource.isReady())
        
    def testEscapingSourceBecomesUnreadyAfterReceiving(self):
        mock = MockSource(deque([0x42]))
        escapingSource = EscapingSource(mock)
        escapingSource.schedule()
        escapingSource.next();
        self.assertFalse(escapingSource.isReady())

    def testEscapingSourceReceivesOctetCorrectly(self):
        mock = MockSource(deque([0x42]))
        escapingSource = EscapingSource(mock)
        escapingSource.schedule()
        self.assertFalse(escapingSource.isFlag())
        self.assertTrue(0x42 == escapingSource.read())

    def testEscapingSourceReceivesEscapedOctetCorrectly(self):
        mock = MockSource(deque([EscapingSource.ESCAPE(), EscapingSource.FLAG() ^ EscapingSource.MASK(), EscapingSource.ESCAPE(), EscapingSource.ESCAPE() ^ EscapingSource.MASK()]))
        escapingSource = EscapingSource(mock)
        escapingSource.schedule()
        escapingSource.schedule()
        self.assertFalse(escapingSource.isFlag())
        self.assertTrue(EscapingSource.FLAG() == escapingSource.read())
        escapingSource.schedule()
        escapingSource.schedule()
        self.assertFalse(escapingSource.isFlag())
        self.assertTrue(EscapingSource.ESCAPE() == escapingSource.read())

    def testEscapingSourceReceivesFlagOctetCorrectly(self):
        mock = MockSource(deque([EscapingSource.FLAG()]))
        escapingSource = EscapingSource(mock)
        escapingSource.schedule()
        self.assertTrue(escapingSource.isReady())
        self.assertTrue(escapingSource.isFlag())

    def testEscapingSourceReceivesRegularOctetCorrectlyAfterReceivingFlag(self):
        mock = MockSource(deque([EscapingSource.FLAG(), 0x42]))
        escapingSource = EscapingSource(mock)
        escapingSource.schedule()
        self.assertTrue(escapingSource.isFlag())
        escapingSource.next()
        escapingSource.schedule()
        self.assertFalse(escapingSource.isFlag())
        self.assertTrue(0x42 == escapingSource.read())

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()