/*
 * SolenoidAction.h
 *
 *  Created on: Mar 1, 2015
 *      Author: peter
 */

#ifndef SOLENOIDACTION_H_
#define SOLENOIDACTION_H_

#include <Solenoid.h>

#include <stdint.h>

extern Solenoid solenoids[6];

class SolenoidAction {
public:
	uint8_t enabled : 1;
	uint8_t solenoidIndex : 7;
	uint16_t attack;
	uint8_t sustain;

	SolenoidAction();
	SolenoidAction(bool, uint8_t, uint16_t, uint8_t);

	void trigger();
};

#endif /* SOLENOIDACTION_H_ */
