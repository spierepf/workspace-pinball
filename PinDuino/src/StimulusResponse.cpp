/*
 * StimulusResponse.cpp
 *
 *  Created on: Feb 24, 2015
 *      Author: peter
 */

#include "StimulusResponse.h"

StimulusResponse::StimulusResponse() : enabled(false) {
}

StimulusResponse::~StimulusResponse() {
	// TODO Auto-generated destructor stub
}

void StimulusResponse::config(uint8_t pin, bool newState, SolenoidAction action) {
	entries[newState][pin] = action;
}

void StimulusResponse::trigger(uint8_t pin, bool newState) {
	if(enabled && entries[newState][pin].enabled) {
		entries[newState][pin].trigger();
	}
}

void StimulusResponse::inhibit() {
	enabled = false;
}

void StimulusResponse::enable() {
	enabled = true;
}
