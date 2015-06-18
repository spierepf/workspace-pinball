/*
 * SolenoidBank.h
 *
 *  Created on: Jun 18, 2015
 *      Author: peter
 */

#ifndef SOLENOIDBANK_H_
#define SOLENOIDBANK_H_

#include <pt.h>

#include "Solenoid.h"

class SolenoidBank {
	struct pt pt;
	PT_THREAD(run());

	Solenoid** solenoids;
	uint8_t& dirtyList;

public:
	SolenoidBank(Solenoid**, uint8_t&);
	virtual ~SolenoidBank();

	void schedule();
};

#endif /* SOLENOIDBANK_H_ */
