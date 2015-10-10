/*
 * Blinker.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: peter
 */

#include <extern.h>

#include <Blinker.h>
#include <Arduino.h>

#include <log.h>

Blinker::Blinker() : count(0) {
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
		if(!(++count)) {
			float t = timer.elapsed(micros());
			t /= 65536.0;
			LOG("Loop time: %4.2f us", t);
			timer.set(micros());
			led.toggle();
		}
	}
	PT_END(&pt);
}
