'''
Created on Oct 11, 2015

@author: peter
'''

class MockSink(object):
    '''
    classdocs
    '''


    def __init__(self, items = None):
        '''
        Constructor
        '''
        if items == None:
            self.items = []
        else:
            self.items = items
        
    def write(self, octet):
        self.items += [octet]