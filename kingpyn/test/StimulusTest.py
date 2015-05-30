'''
Created on May 29, 2015

@author: peter
'''
import unittest
from kingpyn.Stimulus import Stimulus


class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testByteArray(self):
        x = Stimulus(2, 1)
        self.assertEqual(x, Stimulus.fromByteArray(x.toByteArray()))


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testByteArray']
    unittest.main()