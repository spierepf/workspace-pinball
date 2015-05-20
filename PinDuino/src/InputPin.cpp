/*
 * InputPin.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: peter
 */

#include <Arduino.h>

#include <OutgoingPinDuinoDataLink.h>
#include <StimulusResponse.h>
#include <OpCode.h>

#include "InputPin.h"

InputPin::InputPin(OutgoingDataLink& outgoingDatalink, PinBank& bank, uint8_t index, uint8_t id) : outgoingDatalink(outgoingDatalink), bank(bank), mask(_BV(index)), id(id) {
	PT_INIT(&pt);
}

InputPin::~InputPin() {
	// TODO Auto-generated destructor stub
}

void InputPin::schedule() {
	PT_SCHEDULE(run());
}

void InputPin::pinChange(bool newState) {
	outgoingDatalink.begin_outgoing_frame(newState ? OpCode::PIN_HIGH : OpCode::PIN_LOW);
	outgoingDatalink.append_payload(id);
	outgoingDatalink.end_outgoing_frame();
	stimulusResponse.trigger(Stimulus(id, newState));
}

PT_THREAD(InputPin::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		PT_WAIT_UNTIL(&pt, (bank.read() & mask) == 0);
		pinChange(false);
		timer.set(micros());
		PT_WAIT_UNTIL(&pt, timer.elapsed(micros()) > 3000);
		PT_WAIT_UNTIL(&pt, (bank.read() & mask) != 0);
		pinChange(true);
		timer.set(micros());
		PT_WAIT_UNTIL(&pt, timer.elapsed(micros()) > 3000);
	}
	PT_END(&pt);
}
