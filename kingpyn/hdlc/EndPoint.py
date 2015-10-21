'''
Created on Oct 11, 2015

@author: peter
'''
from hdlc.FrameTransmitter import FrameTransmitter
from hdlc.FrameReceiver import FrameReceiver
from hdlc.SequenceNumber import SequenceNumber

class EndPoint(object):
    '''
    classdocs
    '''

    class State(object):
        def __init__(self, outer):
            self.outer = outer
            self.sendSyn = False
            self.timeout =  0x00001FFF
            self.idleCount = 0
            
        def onEntry(self):
            pass
        
        def connect(self):
            pass
        
        def go(self):
            pass
        
        def handle(self, header, payload):
            pass
        
    class Disconnected(State):
        def connect(self):
            self.outer.enterState(self.outer.syncRequestSent)

        def go(self):
            if self.sendSyn and self.outer.transmitter.isReady():
                self.outer.transmitter.transmit(FrameTransmitter.SYN_DISCONNECT())
                self.sendSyn = False
                
        def handle(self, header, payload):
            if header == FrameReceiver.SYN_DISCONNECT():
                pass
            elif header == FrameReceiver.SYN_REQUEST():
                self.outer.enterState(self.outer.syncResponseSent)
            else:
                self.sendSyn = True

    class SyncRequestSent(State):
        def onEntry(self):
            self.sendSyn = True
            
        def go(self):
            if self.sendSyn and self.outer.transmitter.isReady():
                self.outer.transmitter.transmit(FrameTransmitter.SYN_REQUEST())
                self.sendSyn = False
    
        def handle(self, header, payload):
            if header == FrameReceiver.SYN_DISCONNECT():
                self.outer.enterState(self.outer.disconnected)
            elif header == FrameReceiver.SYN_REQUEST():
                self.outer.enterState(self.outer.syncResponseSent)
            elif header == FrameReceiver.SYN_RESPONSE():
                self.outer.connected.sendSyn = True;
                self.outer.enterState(self.outer.connected);
            else:
                self.sendSyn = True

    class SyncResponseSent(State):
        def onEntry(self):
            self.sendSyn = True

        def connect(self):
            self.outer.enterState(self.outer.syncRequestSent)
        
        def go(self):
            if self.sendSyn and self.outer.transmitter.isReady():
                self.outer.transmitter.transmit(FrameTransmitter.SYN_RESPONSE())
                self.idleCount = 0
                self.sendSyn = False
            else:
                self.idleCount += 1
                if self.idleCount > self.timeout:
                    self.sendSyn = True

        def handle(self, header, payload):
            if header == FrameReceiver.SYN_DISCONNECT():
                self.outer.enterState(self.outer.disconnected)
            elif header == FrameReceiver.SYN_REQUEST():
                pass
            elif header == FrameReceiver.SYN_RESPONSE():
                self.outer.connected.sendSyn = True;
                self.outer.enterState(self.outer.connected);
            elif header == FrameReceiver.SYN_COMPLETE():
                self.outer.enterState(self.outer.connected);
            else:
                self.sendSyn = True
        
    class Connected(State):
        def __init__(self, outer, outgoingFrameBuffer):
            super(EndPoint.Connected, self).__init__(outer)
            self.outgoingFrameBuffer = outgoingFrameBuffer
            
        def onEntry(self):
            self.zeroFrame = SequenceNumber()
            self.lastAckReceived = SequenceNumber()
            self.sendAck = False
            self.sendUserFrame = True
            self.expectedSequenceNumber = SequenceNumber()

        def connect(self):
            self.outer.enterState(self.outer.syncRequestSent)
        
        def go(self):
            if self.outer.transmitter.isReady():
                while self.zeroFrame != self.lastAckReceived:
                    del(self.outgoingFrameBuffer[0])
                    self.zeroFrame += 1
                if self.sendSyn:
                    self.outer.transmitter.transmit(FrameTransmitter.SYN_COMPLETE())
                    self.idleCount = 0
                    self.sendSyn = False
                elif self.sendAck:
                    self.outer.transmitter.transmit(FrameTransmitter.ACK() + self.expectedSequenceNumber.value)
                    self.idleCount = 0
                    self.sendAck = False
                elif len(self.outgoingFrameBuffer) > 0:
                    if self.sendUserFrame:
                        self.outer.transmitter.transmit(self.zeroFrame.value, self.outgoingFrameBuffer[0])
                        self.idleCount = 0
                        self.sendUserFrame = False
                    else:
                        self.idleCount += 1
                        if self.idleCount > self.timeout:
                            self.sendUserFrame = True
            
        def handle(self, header, payload):
            if header == FrameReceiver.SYN_DISCONNECT():
                self.outer.enterState(self.outer.disconnected)
            elif header == FrameReceiver.SYN_REQUEST():
                self.outer.enterState(self.outer.syncResponseSent)
            elif header == FrameReceiver.SYN_RESPONSE():
                self.outer.connected.sendSyn = True;
            elif header == FrameReceiver.SYN_COMPLETE():
                pass
            else:
                if (header & FrameReceiver.CONTROL_BITS()) == FrameReceiver.ACK():
                    self.lastAckReceived = SequenceNumber(header)
                    self.sendUserFrame = True
                else:
                    self.sendAck = True
                    if(header == self.expectedSequenceNumber.value) :
                        self.expectedSequenceNumber += 1
                        self.outer.handler.handle(header, payload)

    def __init__(self, escapingSource, frameReceiver, frameHandler, outgoingFrameBuffer, frameTransmitter, escapingSink):
        '''
        Constructor
        '''
        self.source = escapingSource
        self.receiver = frameReceiver
        self.handler = frameHandler
        self.outgoingFrameBuffer = outgoingFrameBuffer
        self.transmitter = frameTransmitter
        self.sink = escapingSink
        self.disconnected = EndPoint.Disconnected(self)
        self.syncRequestSent = EndPoint.SyncRequestSent(self)
        self.syncResponseSent = EndPoint.SyncResponseSent(self)
        self.connected = EndPoint.Connected(self, outgoingFrameBuffer)
        self.state = self.disconnected
    
    def enterState(self, newState):
        newState.onEntry()
        self.state = newState

    def isConnected(self):
        return self.state == self.connected
    
    def connect(self):
        self.state.connect()
    
    def schedule(self):
        self.source.schedule();
        self.receiver.schedule();
    
        self.state.go();
    
        self.transmitter.schedule();
        self.sink.schedule();

    def handle(self, header, payload):
        self.state.handle(header, payload)
        