#!/bin/bash

avrdude	-pm328p -carduino -P/dev/ttyUSB0 -Uflash:w:PinDuino.hex:a
avrdude	-pm328p -carduino -P/dev/ttyUSB1 -Uflash:w:PinDuino.hex:a
avrdude	-pm328p -carduino -P/dev/ttyUSB2 -Uflash:w:PinDuino.hex:a
