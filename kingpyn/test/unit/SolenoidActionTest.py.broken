'''
Created on May 29, 2015

@author: peter
'''
import unittest
from kingpyn.SolenoidAction import SolenoidAction


class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testByteArray(self):
        x = SolenoidAction(True, 2, 3, 4)
        self.assertEqual(x, SolenoidAction.fromByteArray(x.toByteArray()))

    def testFromByteArray(self):
        x = SolenoidAction(True, 2, 3, 4)
        self.assertEqual((2 << 1) | 1, x.toByteArray()[0])
        self.assertEqual(4, x.toByteArray()[1])
        self.assertEqual(3, x.toByteArray()[2])
        self.assertEqual(0, x.toByteArray()[3])

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()