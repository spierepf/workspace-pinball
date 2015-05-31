'''
Created on May 28, 2015

@author: peter
'''
import unittest
from kingpyn.HardwareRuleManager import HardwareRuleManager
from kingpyn.HardwareRule import HardwareRule
from kingpyn.Stimulus import Stimulus
from kingpyn.SolenoidAction import SolenoidAction

class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testCleanByDefault(self):
        manager = HardwareRuleManager()
        self.assert_(manager.isClean())

    def testAddRuleMakesDirty(self):
        manager = HardwareRuleManager()
        manager.addRule(HardwareRule(Stimulus(0, 0), SolenoidAction(True, 0, 0, 0)))
        self.assertFalse(manager.isClean())
        
    def testRequestDirtyRule(self):
        manager = HardwareRuleManager()
        rule = HardwareRule(Stimulus(0, 0), SolenoidAction(True, 0, 0, 0))
        manager.addRule(rule)
        self.assertEquals(rule, manager.requestDirtyRule())
        
    def testMakeRuleClean(self):
        manager = HardwareRuleManager()
        rule1 = HardwareRule(Stimulus(0, 0), SolenoidAction(True, 0, 0, 0))
        rule2 = HardwareRule(Stimulus(0, 0), SolenoidAction(True, 0, 0, 0))
        manager.addRule(rule1)
        manager.validateRule(rule2)
        self.assert_(manager.isClean())

    def testUnsuccessfulValidateRule(self):
        manager = HardwareRuleManager()
        clean = HardwareRule(Stimulus(0, 0), SolenoidAction(True, 0, 0, 0))
        dirty = HardwareRule(Stimulus(0, 0), SolenoidAction(True, 0, 0, 1))
        manager.addRule(clean)
        manager.validateRule(dirty)
        self.assertFalse(manager.isClean())

    def testUnsuccessfulValidateRuleMakesRuleDirty(self):
        manager = HardwareRuleManager()
        clean = HardwareRule(Stimulus(0, 0), SolenoidAction(True, 0, 0, 0))
        manager.addRule(clean)
        manager.validateRule(clean)
        dirty = HardwareRule(Stimulus(0, 0), SolenoidAction(True, 0, 0, 1))
        manager.validateRule(dirty)
        self.assertFalse(manager.isClean())
        
if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()