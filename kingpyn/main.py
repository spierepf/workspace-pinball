import logging

from kingpyn.EndPointManager import EndPointManager

logging.basicConfig()

log = logging.getLogger("main")
log.setLevel(logging.DEBUG)

log.debug("Starting...")

endPointManager = EndPointManager()
endPointManager.addDevices()

while True:
    endPointManager.schedule()