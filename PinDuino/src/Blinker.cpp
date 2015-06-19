/*
 * Blinker.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: peter
 */

#include <Blinker.h>
#include <Arduino.h>

Blinker::Blinker(OutgoingPinDuinoDataLink& outgoingDatalink) : outgoingDatalink(outgoingDatalink), led(PinBank::B, _BV(5)), count(0) {
	led.dataLow();
	led.dirOut();
	PT_INIT(&pt);
	timer.set(micros());
}

Blinker::~Blinker() {
	// TODO Auto-generated destructor stub
}

void Blinker::schedule() {
	PT_SCHEDULE(run());
}

PT_THREAD(Blinker::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		PT_YIELD(&pt);
		if(count++ == 0xFFFF) {
			outgoingDatalink.log("Loop time: %lu", timer.elapsed(micros()));
			timer.set(micros());
			led.toggle();
		}
	}
	PT_END(&pt);
}
