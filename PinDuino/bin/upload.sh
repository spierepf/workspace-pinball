#!/bin/bash

for i in /dev/kingpyn*
do
    avrdude	-pm328p -carduino -P$i -Uflash:w:PinDuino.hex:a
done

