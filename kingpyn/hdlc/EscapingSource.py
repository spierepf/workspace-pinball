'''
Created on Oct 10, 2015

@author: peter
'''

class EscapingSource(object):
    '''
    classdocs
    '''

    @staticmethod
    def MASK():
        return 0x20

    @staticmethod
    def ESCAPE():
        return 0x7D

    @staticmethod
    def FLAG():
        return 0x7E

    def __init__(self, source):
        '''
        Constructor
        '''
        self.octet = None
        self.escape = False
        self.source = source
        self.flag = False
    
    def isReady(self):
        return self.octet != None or self.flag

    def schedule(self):
        if self.source.isReady():
            tmp = self.source.read()
            if self.escape:
                self.octet = tmp ^ EscapingSource.MASK()
                self.flag = False
                self.escape = False
            elif tmp == EscapingSource.ESCAPE():
                self.escape = True
            elif tmp == EscapingSource.FLAG():
                self.flag = True
            else:
                self.octet = tmp
                self.flag = False
                
    def next(self):
        self.octet = None
        self.flag = False
    
    def isFlag(self):
        return self.flag
    
    def read(self):
        return self.octet
    
'''
bool EscapingSource::isReady() {
    return ready;
}

uint8_t EscapingSource::read() {
    return octet;
}

bool EscapingSource::isFlag() {
    return flag;
}

void EscapingSource::next() {
    ready = false;
}
'''    