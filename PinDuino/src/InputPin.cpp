/*
 * InputPin.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: peter
 */

#include <Arduino.h>

#include "InputPin.h"

InputPin::InputPin(Application& application, PinBank& bank, uint8_t index, uint8_t id) : application(application), bank(bank), mask(_BV(index)), id(id) {
	PT_INIT(&pt);
}

InputPin::~InputPin() {
	// TODO Auto-generated destructor stub
}

void InputPin::schedule() {
	PT_SCHEDULE(run());
}

PT_THREAD(InputPin::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		PT_WAIT_UNTIL(&pt, (bank.read() & mask) == 0);
		application.pinChange(id, false);
		timer.set(micros());
		PT_WAIT_UNTIL(&pt, timer.elapsed(micros()) > 3000);
		PT_WAIT_UNTIL(&pt, (bank.read() & mask) != 0);
		application.pinChange(id, true);
		timer.set(micros());
		PT_WAIT_UNTIL(&pt, timer.elapsed(micros()) > 3000);
	}
	PT_END(&pt);
}
