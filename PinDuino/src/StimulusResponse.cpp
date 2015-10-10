/*
 * StimulusResponse.cpp
 *
 *  Created on: Feb 24, 2015
 *      Author: peter
 */

#include "StimulusResponse.h"

#include <Solenoid.h>

#include <stdlib.h>

StimulusResponse::StimulusResponse() : enabled(true) {
}

StimulusResponse::~StimulusResponse() {
	// TODO Auto-generated destructor stub
}

void StimulusResponse::trigger(Stimulus stimulus) {
	SolenoidAction& entry = operator[](stimulus);
	if(enabled && entry.enabled && entry.solenoidIndex < 6 && entry.attack <= 65000) {
		if(entry.attack == 0) {
			if(solenoids[entry.solenoidIndex] != NULL) solenoids[entry.solenoidIndex] -> release();
		} else {
			if(solenoids[entry.solenoidIndex] != NULL) solenoids[entry.solenoidIndex] -> trigger(entry.attack, entry.sustain);
		}
	}
}

void StimulusResponse::inhibit() {
	enabled = false;
}

void StimulusResponse::enable() {
	enabled = true;
}
