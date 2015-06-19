/*
 * Blinker.h
 *
 *  Created on: Feb 4, 2015
 *      Author: peter
 */

#ifndef BLINKER_H_
#define BLINKER_H_

#include <pt.h>

#include <OutgoingPinDuinoDataLink.h>
#include <Timer.h>

#include <PinBank.h>

class Blinker {
	struct pt pt;
	PT_THREAD(run());

	OutgoingPinDuinoDataLink& outgoingDatalink;
	Timer<unsigned long> timer;

	PinBank led;
	uint16_t count;

public:
	Blinker(OutgoingPinDuinoDataLink&);
	virtual ~Blinker();

	void schedule();
};

#endif /* BLINKER_H_ */
