/*
 * Switch.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: peter
 */

#include <Arduino.h>

#include <StimulusResponse.h>
#include <OpCode.h>

#include "Switch.h"

Switch::Switch(uint8_t index, uint8_t id, uint8_t& dirtyList) : Item(_BV(index), dirtyList), id(id), state(true), debounceThreshold(500>>2) {
}

Switch::~Switch() {
	// TODO Auto-generated destructor stub
}

void Switch::pinChange(bool newState) {
	state = newState;
	outgoingFrameBuffer.put(newState ? OpCode::SWITCH_INACTIVE : OpCode::SWITCH_ACTIVE);
	outgoingFrameBuffer.put(id);
	outgoingFrameBuffer.endFrame();
	stimulusResponse.trigger(Stimulus(id, newState));
}

void Switch::update(uint8_t last, uint8_t current) {
	bool newState = current & mask;

	if(state == newState) {
		clean();
		return;
	}

	if((mask & last) != (mask & current)) {
		debounceTimer.set(micros());
	}

	if(debounceTimer.elapsed(micros()) >= (debounceThreshold << 2)) {
		clean();
		pinChange(newState);
	}
}
