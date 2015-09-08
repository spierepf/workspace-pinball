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

Switch::Switch(OutgoingPinDuinoDataLink& outgoingDatalink, FrameBuffer<64, 4>& outgoingFrameBuffer, uint8_t index, uint8_t id, uint8_t& dirtyList) : Item(_BV(index), dirtyList), outgoingDatalink(outgoingDatalink), outgoingFrames(outgoingFrames), id(id), history(0xff), state(true) {
}

Switch::~Switch() {
	// TODO Auto-generated destructor stub
}

void Switch::pinChange(bool newState) {
	state = newState;
	outgoingFrames.put(newState ? OpCode::SWITCH_INACTIVE : OpCode::SWITCH_ACTIVE);
	outgoingFrames.put(id);
	outgoingFrames.endFrame();
	stimulusResponse.trigger(Stimulus(id, newState));
}

void Switch::update(uint8_t read) {
	history = (history << 1) | ((read & mask) == 0 ? 0 : 1);
	if(history == 0x00) {
		clean();
		if(state) pinChange(false);
	} else if(history == 0xff) {
		clean();
		if(!state) pinChange(true);
	}
}
