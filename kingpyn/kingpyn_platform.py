"""Contains code for a virtual hardware platform. At this point this is more
for testing before you have a P-ROC or FAST board installed. Eventually this
can be used to allow the MPF to drive PinMAME and Virtual Pinball machines.

This is similar to the P-ROC's 'FakePinPROC' mode of operation, though unlike
that it doesn't require any P-ROC drivers or modules to be installed.

"""
# virtual.py
# Mission Pinball Framework
# Written by Brian Madden & Gabe Knuth
# Released under the MIT License. (See license info at the end of this file.)

# Documentation and more info at http://missionpinball.com/mpf

import logging
from mpf.system.platform import Platform
from mpf.system.utility_functions import Util
from mpf.system.config import Config
from kingpyn.EndPointManager import EndPointManager


class HardwarePlatform(Platform):
    """Base class for the virtual hardware platform."""

    def __init__(self, machine):
        super(HardwarePlatform, self).__init__(machine)
        self.log = logging.getLogger("Kingpyn Platform")
        self.log.debug("Configuring kingpyn hardware interface.")
        #self.machine.physical_hw = False

        # Since the virtual platform doesn't have real hardware, we need to
        # maintain an internal list of switches that were confirmed so we have
        # something to send when MPF needs to know the hardware states of
        # switches
        self.hw_switches = dict()
        self.initial_states_sent = False

        # ----------------------------------------------------------------------
        # Platform-specific hardware features. WARNING: Do not edit these. They
        # are based on what the virtual hardware can and cannot do.
        self.features['max_pulse'] = 255
        self.features['hw_timer'] = False
        self.features['hw_rule_coil_delay'] = False
        self.features['variable_recycle_time'] = False
        self.features['variable_debounce_time'] = False

        # Make the platform features available to everyone
        self.machine.config['platform'] = self.features
        # ----------------------------------------------------------------------

        self.endPointManager = EndPointManager()
        self.endPointManager.addDevices()

    def __repr__(self):
        return '<Platform.Kingpyn>'

    def configure_driver(self, config, device_type='coil'):
        # todo should probably throw out the number that we get since it could
        # be a weird string and just return an incremental int?
        self.log.debug("Configuring driver: {}".format(config))

        driver = KingpynDriver(self, config['number'])

        driver.driver_settings = config
        if driver.driver_settings['pulse_ms'] == None:
            driver.driver_settings['pulse_ms'] = 30

        return driver, config['number']

    def configure_switch(self, config):
        # We want to have the virtual platform set all the initial switch states
        # to inactive, so we have to check the config.

        state = 0

        if config['type'].upper() == 'NC':
            state = 1

        self.hw_switches[config['number']] = state

        if not config['debounce_open']:
            config['debounce_open'] = None

        if not config['debounce_close']:
            config['debounce_close'] = None

        switch = KingpynSwitch(self, config['number'],
                               debounce_open=config['debounce_open'],
                               debounce_close=config['debounce_close'],
                               disable_pullup="kingpyn_disable_pullup" in config['tags'])

        switch.driver_settings = config

        return switch, config['number']

    def get_hw_switch_states(self):

        if not self.initial_states_sent:

            if 'virtual_platform_start_active_switches' in self.machine.config:

                initial_active_switches = [self.machine.switches[x].number for x in
                    Util.string_to_list(
                        self.machine.config['virtual_platform_start_active_switches'])]

                for k, v in self.hw_switches.iteritems():
                    if k in initial_active_switches:
                        self.hw_switches[k] ^= 1

            self.initial_states_sent = True

        else:
            switches = [x for x in self.machine.switches if x.platform == self]

            for switch in switches:
                self.hw_switches[switch.number] = switch.state ^ switch.invert

        return self.hw_switches
    '''
    def configure_matrixlight(self, config):
        return VirtualMatrixLight(config['number']), config['number']

    def configure_led(self, config):
        return VirtualLED(config['number'])

    def configure_gi(self, config):
        return VirtualGI(config['number']), config['number']

    def configure_dmd(self):
        return VirtualDMD(self.machine)
    '''
    def write_hw_rule(self, *args, **kwargs):
        self.log.info(args)
        self.log.info(kwargs)
        switch_obj = args[0]
        sw_activity = args[1]
        driver_obj = args[2]
        driver_action = args[3]
        #disable_on_release = args[4]
        #drive_now = args[5]

        if kwargs['pulse_ms'] == None:
            pulse_ms = driver_obj.config['pulse_ms']
        else:
            pulse_ms = kwargs['pulse_ms']

        endPointId = int(switch_obj.number.split("-")[0])
        assert int(driver_obj.number.split("-")[0]) == endPointId

        switchId = int(switch_obj.number.split("-")[1])
        activity = 1 - sw_activity
        solenoidId = int(driver_obj.number.split("-")[1])
        
        if driver_action == 'hold':
            self.endPointManager.addHardwareRule(endPointId, switchId, activity, solenoidId, 1, 255)
            self.endPointManager.addHardwareRule(endPointId, switchId, 1 - activity, solenoidId, 0, 0)
        else:
            self.endPointManager.addHardwareRule(endPointId, switchId, activity, solenoidId, pulse_ms * 1000, 0)

    def clear_hw_rule(self, sw_name):
        sw_num = self.machine.switches[sw_name].number

        for entry in self.hw_switch_rules.keys():  # slice for copy
            if entry.startswith(
                    self.machine.switches.number(sw_num).name):
                del self.hw_switch_rules[entry]

    def tick(self):
        Platform.tick(self)
        self.endPointManager.schedule()
        while len(self.endPointManager.switchEventQueue) > 0:
            (number, newState) = self.endPointManager.switchEventQueue.popleft()
            self.log.debug("Switch event: {} {}".format(number, str(newState)))
            if self.hw_switches.has_key(number):
                self.machine.switch_controller.process_switch(state=newState, num=number)


class KingpynSwitch(object):
    """Represents a switch in a pinball machine used with virtual hardware."""
    def __init__(self, outer, number, debounce_open, debounce_close, disable_pullup):
        self.log = logging.getLogger('KingpynSwitch')
        self.outer = outer
        self.number = number

        endPointId = int(self.number.split("-")[0])
        switchId = int(self.number.split("-")[1])

        if debounce_open != None:
            self.outer.endPointManager.setSwitchDebounceThreshold(endPointId, switchId, True, debounce_open)
        if debounce_close != None:
            self.outer.endPointManager.setSwitchDebounceThreshold(endPointId, switchId, False, debounce_close)
        if disable_pullup:
            self.outer.endPointManager.disablePullup(endPointId, switchId, disable_pullup)

'''
class VirtualMatrixLight(object):
    def __init__(self, number):
        self.log = logging.getLogger('VirtualMatrixLight')
        self.number = number

    def on(self, brightness=255, fade_ms=0, start=0):
        pass

    def off(self):
        pass


class VirtualLED(object):
    def __init__(self, number):
        self.log = logging.getLogger('VirtualLED')
        self.number = number

    def color(self, color, fade_ms=0, brightness_compensation=True):
        #self.log.debug("Setting color: %s, fade: %s, comp: %s",
        #               color, fade_ms, brightness_compensation)
        pass

    def disable(self):
        pass

    def enable(self, brightness_compensation=True):
        pass


class VirtualGI(object):
    def __init__(self, number):
        self.log = logging.getLogger('VirtualGI')
        self.number = number

    def on(self, brightness, fade_ms, start):
        pass

    def off(self):
        pass
'''

class KingpynDriver(object):

    def __init__(self, outer, number):
        self.log = logging.getLogger('KingpynDriver')
        self.outer = outer
        self.number = number
        self.endPointId = int(self.number.split("-")[0])
        self.solenoidId = int(self.number.split("-")[1])

    def __repr__(self):
        return "KingpynDriver.{}".format(self.number)

    def validate_driver_settings(self, **kwargs):
        return dict()

    def disable(self):
        self.log.debug("{}.disable()".format(self))

    def enable(self):
        self.log.debug("{}.enable()".format(self))

    def pulse(self, milliseconds=None):
        
        if milliseconds:
            t = milliseconds
        else:
            t = self.driver_settings['pulse_ms']

        self.outer.endPointManager.pulseSolenoid(self.endPointId, self.solenoidId, t)
        
        return t

    def state(self):
        self.log.debug("{}.state()".format(self))

    def tick(self):
        self.log.debug("{}.tick()".format(self))

    def reconfigure(self, polarity):
        self.log.debug("{}.reconfigure({})".format(self, polarity))

'''
class VirtualDMD(object):

    def __init__(self, machine):
        pass

    def update(self, data):
        pass
'''
# The MIT License (MIT)

# Oringal code on which this module was based:
# Copyright (c) 2009-2011 Adam Preble and Gerry Stellenberg

# Copyright (c) 2013-2015 Brian Madden and Gabe Knuth

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
