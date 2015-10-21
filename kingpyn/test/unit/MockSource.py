'''
Created on Oct 10, 2015

@author: peter
'''

class MockSource(object):
    '''
    classdocs
    '''


    def __init__(self, items):
        '''
        Constructor
        '''
        self.items = items
        
    def isReady(self):
        return len(self.items) > 0
    
    def read(self):
        return self.items.popleft()