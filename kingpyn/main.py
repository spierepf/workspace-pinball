'''
Created on May 22, 2015

@author: peter
'''
from hdlc.Tty import Tty
from hdlc.IncomingDataLink import IncomingDataLink
from hdlc.OutgoingDataLink import OutgoingDataLink
from kingpyn.EndPoint import EndPoint
from kingpyn.EndPointManager import EndPointManager
import logging

if __name__ == '__main__':
    logging.basicConfig()
    hardware = Tty("/dev/ttyACM0")
    #hardware = Hardware("/dev/ttyUSB0")
    incomingDatalink = IncomingDataLink(hardware)
    outgoingDatalink = OutgoingDataLink(hardware)
    endpoint = EndPoint(incomingDatalink, outgoingDatalink)

    endPointManager = EndPointManager()
    endPointManager.addEndPoint(endpoint)

    while True:
        endPointManager.run()
