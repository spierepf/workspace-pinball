'''
Created on Jun 9, 2015

@author: peter
'''
from kingpyn.HardwareRule import HardwareRule
from kingpyn.OpCode import OpCode

class HardwareRuleManager(object):
    '''
    classdocs
    '''


    def __init__(self, outgoingFrameHandler):
        '''
        Constructor
        '''
        self.dirty = set()
        self.rules = {}
        self.outgoingFrameHandler = outgoingFrameHandler
    
    def isDirty(self):
        return len(self.dirty) > 0
    
    def sendRule(self, rule = None):
        if rule == None:
            rule = self.rules[list(self.dirty)[0]]
        self.outgoingFrameHandler.postFrame(OpCode.SR_CONFIG(), rule.toByteArray())
        self.outgoingFrameHandler.postFrame(OpCode.SR_CONFIG(), rule.stimulus.toByteArray())
        
    def addRule(self, rule):
        alreadyDirty = self.isDirty()
        if not rule.stimulus in self.rules.keys() or self.rules[rule.stimulus] != rule:
            self.rules[rule.stimulus] = rule
            self.dirty.add(rule.stimulus)
            if not alreadyDirty:
                self.sendRule(rule)
        
    def update(self, observable, frame):
        if frame[0] == OpCode.SR_CONFIG() and len(frame[1]) == 5:
            rule = HardwareRule.fromByteArray(frame[1])
            existingRule = self.rules[rule.stimulus] 
            if existingRule == rule:
                self.dirty.remove(rule.stimulus)
                if self.isDirty():
                    self.sendRule()
            else:
                self.dirty.add(existingRule.stimulus)
                self.sendRule(existingRule)
        elif frame[0] == OpCode.MY_ID():
            if len(self.rules) > 0:
                for stimulus in self.rules.keys():
                    self.dirty.add(stimulus)
                self.sendRule()