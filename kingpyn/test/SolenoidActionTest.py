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
        x = SolenoidAction(True, 1, 3000, 3)
        self.assertEqual(x, SolenoidAction.fromByteArray(x.toByteArray()))


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()