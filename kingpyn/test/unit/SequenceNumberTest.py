'''
Created on Oct 13, 2015

@author: peter
'''
import unittest
from SequenceNumber import SequenceNumber


class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testName(self):
        pass

    def testSequenceNumberIntegerCast(self):
        x = SequenceNumber(5)
        
        self.assertTrue(5 == x.value)
        
    def testSequenceNumberIncrement(self):
        x = SequenceNumber()
        
        for i in range(1, 64):
            x += 1
            self.assertEqual(i, x.value)
            
        x += 1
        self.assertEqual(0, x.value)
        
    def testSequenceNumberInequality(self):
        x = SequenceNumber(1)
        y = SequenceNumber(1)
        z = SequenceNumber(2)
        
        self.assertFalse(x != y)
        self.assertTrue(x != z)
        self.assertTrue(z != x)
        
if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()