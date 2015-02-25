/*
 * StimulusResponse.cpp
 *
 *  Created on: Feb 24, 2015
 *      Author: peter
 */

#include "StimulusResponse.h"

StimulusResponse::StimulusResponse(Solenoid *solenoids) : solenoids(solenoids) {
}

StimulusResponse::~StimulusResponse() {
	// TODO Auto-generated destructor stub
}

void StimulusResponse::trigger(uint8_t pin, uint8_t edge) {
	Entry *entry = &(entries[edge][pin]);

	if(entry->enabled) {
		if(entry->attack == 0) {
			solenoids[entry->solenoidIndex].release();
		} else {
			solenoids[entry->solenoidIndex].trigger(entry->attack);
		}
	}
}
