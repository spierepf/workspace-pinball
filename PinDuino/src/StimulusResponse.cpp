/*
 * StimulusResponse.cpp
 *
 *  Created on: Feb 24, 2015
 *      Author: peter
 */

#include "StimulusResponse.h"

StimulusResponse::StimulusResponse(Solenoid *solenoids) : solenoids(solenoids), enabled(false) {
}

StimulusResponse::~StimulusResponse() {
	// TODO Auto-generated destructor stub
}

void StimulusResponse::trigger(uint8_t pin, bool newState) {
	Entry *entry = &(entries[newState][pin]);

	if(enabled && entry->enabled) {
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
