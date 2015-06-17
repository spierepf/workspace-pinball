/*
 * SwitchBank.h
 *
 *  Created on: Jun 17, 2015
 *      Author: peter
 */

#ifndef SWITCHBANK_H_
#define SWITCHBANK_H_

#include <pt.h>

#include <stdlib.h>

#include <Switch.h>
#include <PinBank.h>

class SwitchBank {
	struct pt pt;
	PT_THREAD(run());

	Switch** switches;
	PinBank& pinBank;
	uint8_t last;
	uint8_t current;
	uint8_t& dirtyList;

public:
	SwitchBank(Switch**, PinBank&, uint8_t&);
	virtual ~SwitchBank();

	void schedule();
};

#endif /* SWITCHBANK_H_ */
