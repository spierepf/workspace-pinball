/*
 * InputPin.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: peter
 */

#include "InputPin.h"

InputPin::InputPin(Application& application, PinBank& bank, uint8_t index) : application(application), bank(bank), mask(_BV(index)) {
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
		application.pin_low();
		PT_WAIT_UNTIL(&pt, (bank.read() & mask) != 0);
		application.pin_high();
	}
	PT_END(&pt);
}
