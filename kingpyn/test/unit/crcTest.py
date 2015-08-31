'''
Created on Aug 31, 2015

@author: peter
'''
import unittest

from hdlc.crc import crc_ccitt_update

class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testCrc_ccitt_update(self):
        crc = 0xFFFF;
        
        crc = crc_ccitt_update(crc, 0x00)
        self.assertTrue(0xE1F0 == crc)
    
        crc = crc_ccitt_update(crc, 0x01)
        self.assertTrue(0x0D2E == crc)
    
        crc = crc_ccitt_update(crc, 0x02)
        self.assertTrue(0xDFEF == crc)
    
        crc = crc_ccitt_update(crc, 0xDF)
        crc = crc_ccitt_update(crc, 0xEF)
        self.assertTrue(0x0000 == crc)


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testCrc_ccitt_update']
    unittest.main()