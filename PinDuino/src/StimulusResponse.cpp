/*
 * StimulusResponse.cpp
 *
 *  Created on: Feb 24, 2015
 *      Author: peter
 */

#include "StimulusResponse.h"

#include <Solenoid.h>

extern Solenoid solenoids[6];

StimulusResponse::StimulusResponse() : enabled(false) {
}

StimulusResponse::~StimulusResponse() {
	// TODO Auto-generated destructor stub
}

void StimulusResponse::config(uint8_t pin, bool newState, SolenoidAction action) {
	entries[newState][pin] = action;
}

void StimulusResponse::trigger(uint8_t pin, bool newState) {
	SolenoidAction *entry = &(entries[newState][pin]);
	if(enabled && entry->enabled && entry->solenoidIndex < 6 && entry->attack < 65000) {
		if(entry->attack == 0) {
			solenoids[entry->solenoidIndex].release();
		} else {
			solenoids[entry->solenoidIndex].trigger(entry->attack);
		}
	}
}

void StimulusResponse::inhibit() {
	enabled = false;
}

void StimulusResponse::enable() {
	enabled = true;
}
