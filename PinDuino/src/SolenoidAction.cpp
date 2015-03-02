/*
 * SolenoidAction.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: peter
 */

#include "SolenoidAction.h"

SolenoidAction::SolenoidAction() : enabled(1), solenoidIndex(5), attack(65000), sustain(0) {
}

SolenoidAction::SolenoidAction(bool enabled, uint8_t solenoidIndex, uint16_t attack, uint8_t sustain) : enabled(enabled), solenoidIndex(solenoidIndex), attack(attack), sustain(sustain) {
}

void SolenoidAction::trigger() {
	if(attack == 0) {
		solenoids[solenoidIndex].release();
	} else {
		solenoids[solenoidIndex].trigger(attack);
	}
}
