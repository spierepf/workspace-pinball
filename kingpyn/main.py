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
    while len(endPointManager.switchEventQueue) > 0:
        log.info("Switch event: {}".format(endPointManager.switchEventQueue.pop()))

