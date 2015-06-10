'''
Created on Jun 9, 2015

@author: peter
'''
import unittest
from kingpyn.HardwareRuleManager import HardwareRuleManager
from kingpyn.HardwareRule import HardwareRule
from kingpyn.Stimulus import Stimulus
from kingpyn.SolenoidAction import SolenoidAction
from kingpyn.OpCode import OpCode
from kingpyn.OutgoingFrameHandler import OutgoingFrameHandler
from hdlc.OutgoingDataLink import OutgoingDataLink
from test.unit.MockOutgoingHardware import MockOutgoingHardware
from collections import deque

class Test(unittest.TestCase):


    def setUp(self):
        self.hardware = MockOutgoingHardware()
        self.manager = HardwareRuleManager(OutgoingFrameHandler(OutgoingDataLink(self.hardware)))
        self.rule1 = HardwareRule(Stimulus(0, 0), SolenoidAction(True, 0, 0, 0))
        self.rule2 = HardwareRule(Stimulus(0, 1), SolenoidAction(False, 0, 0, 0))


    def tearDown(self):
        self.manager = None
        self.hardware = None
        pass


    def testNewManagerIsClean(self):
        self.assertFalse(self.manager.isDirty())

    def testAddRuleMakesDirty(self):
        self.manager.addRule(self.rule1)
        self.assertTrue(self.manager.isDirty())
        
        for _ in range(100):
            self.manager.outgoingFrameHandler.schedule()
        self.assertEqual(deque([126, 34, 0, 1, 0, 0, 0, 126, 34, 0, 126]), self.hardware.outgoingBytes)

    def testReceiveMatchingRuleFromHardwareMakesClean(self):
        self.manager.addRule(self.rule1)
        
        self.manager.update(None, (OpCode.SR_CONFIG(), self.rule1.toByteArray()))
        self.assertFalse(self.manager.isDirty())

        for _ in range(100):
            self.manager.outgoingFrameHandler.schedule()
        self.assertEqual(deque([126, 34, 0, 1, 0, 0, 0, 126, 34, 0, 126]), self.hardware.outgoingBytes)
        
    def testAddMatchingRuleKeepsClean(self):
        self.manager.addRule(self.rule1)
        self.manager.update(None, (OpCode.SR_CONFIG(), self.rule1.toByteArray()))

        self.manager.addRule(self.rule1)
        self.assertFalse(self.manager.isDirty())

        for _ in range(100):
            self.manager.outgoingFrameHandler.schedule()
        self.assertEqual(deque([126, 34, 0, 1, 0, 0, 0, 126, 34, 0, 126]), self.hardware.outgoingBytes)

    def testReceiveNonMatchingRuleFromHardwareMakesDirty(self):
        self.manager.addRule(self.rule1)
        self.manager.update(None, (OpCode.SR_CONFIG(), self.rule1.toByteArray()))

        self.manager.update(None, (OpCode.SR_CONFIG(), HardwareRule(Stimulus(0, 0), SolenoidAction(False, 0, 0, 0)).toByteArray()))
        self.assertTrue(self.manager.isDirty())

        for _ in range(100):
            self.manager.outgoingFrameHandler.schedule()
        self.assertEqual(deque([126, 34, 0, 1, 0, 0, 0, 126, 34, 0, 126, 34, 0, 1, 0, 0, 0, 126, 34, 0, 126]), self.hardware.outgoingBytes)
    
    def testReceiveMyIDfromHardwareMakesDirty(self):
        self.manager.addRule(self.rule1)
        self.manager.update(None, (OpCode.SR_CONFIG(), self.rule1.toByteArray()))

        self.manager.update(None, (OpCode.MY_ID(), [5]))
        self.assertTrue(self.manager.isDirty())

        for _ in range(100):
            self.manager.outgoingFrameHandler.schedule()
        self.assertEqual(deque([126, 34, 0, 1, 0, 0, 0, 126, 34, 0, 126, 34, 0, 1, 0, 0, 0, 126, 34, 0, 126]), self.hardware.outgoingBytes)
    
    def testSendOnlyOneRuleAtOnce(self):
        self.manager.addRule(self.rule1)
        self.manager.addRule(self.rule2)
        
        for _ in range(100):
            self.manager.outgoingFrameHandler.schedule()
        self.assertEqual(deque([126, 34, 0, 1, 0, 0, 0, 126, 34, 0, 126]), self.hardware.outgoingBytes)
    
    def testSendSecondRuleOnceFirstIsAccepted(self):
        self.manager.addRule(self.rule1)
        self.manager.addRule(self.rule2)
        self.manager.update(None, (OpCode.SR_CONFIG(), self.rule1.toByteArray()))
        
        for _ in range(100):
            self.manager.outgoingFrameHandler.schedule()
        self.assertEqual(deque([126, 34, 0, 1, 0, 0, 0, 126, 34, 0, 126, 34, 128, 0, 0, 0, 0, 126, 34, 128, 126]), self.hardware.outgoingBytes)
        
if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()