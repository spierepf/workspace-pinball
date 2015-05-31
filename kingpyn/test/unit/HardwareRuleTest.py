'''
Created on May 30, 2015

@author: peter
'''
import unittest
from kingpyn.HardwareRule import HardwareRule
from kingpyn.Stimulus import Stimulus
from kingpyn.SolenoidAction import SolenoidAction


class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testByteArray(self):
        x = HardwareRule(Stimulus(1, 1), SolenoidAction(True, 1, 3000, 3))
        self.assertEqual(x, HardwareRule.fromByteArray(x.toByteArray()))


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testByteArray']
    unittest.main()