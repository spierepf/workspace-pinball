/*
 * SolenoidAction.h
 *
 *  Created on: Mar 1, 2015
 *      Author: peter
 */

#ifndef SOLENOIDACTION_H_
#define SOLENOIDACTION_H_

#include <stdint.h>

struct SolenoidAction {
	uint8_t enabled : 1;
	uint8_t solenoidIndex : 7;
	uint16_t attack;
	uint8_t sustain;

	SolenoidAction() : enabled(1), solenoidIndex(5), attack(65000), sustain(0) {
	}

	SolenoidAction(bool enabled, uint8_t solenoidIndex, uint16_t attack, uint8_t sustain) : enabled(enabled), solenoidIndex(solenoidIndex), attack(attack), sustain(sustain) {
	}
};

#endif /* SOLENOIDACTION_H_ */
