'''
Created on May 31, 2015

@author: peter
'''
import unittest
import logging
from hdlc.Tty import Tty
from hdlc.IncomingDataLink import IncomingDataLink
from hdlc.OutgoingDataLink import OutgoingDataLink
from kingpyn.EndPoint import EndPoint
from kingpyn.EndPointManager import EndPointManager
from kingpyn.HardwareRule import HardwareRule
from kingpyn.Stimulus import Stimulus
from kingpyn.SolenoidAction import SolenoidAction


class Test(unittest.TestCase):


    def setUp(self):
        logging.basicConfig()
        
        self.hardware = Tty("/dev/ttyACM0")
        self.incomingDatalink = IncomingDataLink(self.hardware)
        self.outgoingDatalink = OutgoingDataLink(self.hardware)
        self.endpoint = EndPoint(self.incomingDatalink, self.outgoingDatalink)
        self.endpoint.ensureID()

        self.endPointManager = EndPointManager()
        self.endPointManager.addEndPoint(self.endpoint)


    def tearDown(self):
        pass


    def testName(self):
        self.endpoint.addHardwareRule(HardwareRule(Stimulus(6, 1), SolenoidAction(True, 1, 10000, 0)))
        while True:
            self.endPointManager.run()


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()