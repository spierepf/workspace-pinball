/*
 * Switch.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: peter
 */

#include <Arduino.h>

#include <OutgoingPinDuinoDataLink.h>
#include <StimulusResponse.h>
#include <OpCode.h>

#include "Switch.h"

Switch::Switch(OutgoingDataLink& outgoingDatalink, PinBank& bank, uint8_t index, uint8_t id) : outgoingDatalink(outgoingDatalink), bank(bank), mask(_BV(index)), id(id), history(0xff), state(true) {
	PT_INIT(&pt);
}

Switch::~Switch() {
	// TODO Auto-generated destructor stub
}

void Switch::schedule() {
	PT_SCHEDULE(run());
}

void Switch::pinChange(bool newState) {
	state = newState;
	outgoingDatalink.begin_outgoing_frame(newState ? OpCode::SWITCH_INACTIVE : OpCode::SWITCH_ACTIVE);
	outgoingDatalink.append_payload(id);
	outgoingDatalink.end_outgoing_frame();
	stimulusResponse.trigger(Stimulus(id, newState));
}

PT_THREAD(Switch::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		history = (history << 1) | ((bank.read() & mask) == 0 ? 0 : 1);
		if(state) {
			if(history == 0x00) pinChange(false);
		} else {
			if(history == 0xff) pinChange(true);
		}
		PT_YIELD(&pt);
	}
	PT_END(&pt);
}
