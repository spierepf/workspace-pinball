'''
Created on May 28, 2015

@author: peter
'''

class HardwareRuleManager(object):
    '''
    classdocs
    '''


    def __init__(self):
        '''
        Constructor
        '''
        self.dirtyStimuli = set()
        self.rules = {}
    
    def isClean(self):
        return len(self.dirtyStimuli) == 0
    
    def addRule(self, rule):
        self.dirtyStimuli.add(rule.stimulus)
        self.rules[rule.stimulus] = rule

    def requestDirtyRule(self):
        stimulus = None
        if len(self.rules) > 0:
            stimulus = self.dirtyStimuli.pop()
            self.dirtyStimuli.add(stimulus)
        return self.rules[stimulus]
    
    def validateRule(self, rule):
        if self.rules[rule.stimulus] == rule:
            self.dirtyStimuli.remove(rule.stimulus)
        else:
            self.dirtyStimuli.add(rule.stimulus)