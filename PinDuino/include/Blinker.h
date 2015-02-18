/*
 * Blinker.h
 *
 *  Created on: Feb 4, 2015
 *      Author: peter
 */

#ifndef BLINKER_H_
#define BLINKER_H_

#include <pt.h>

#include <PinBank.h>

class Blinker {
	struct pt pt;
	PT_THREAD(run());

	PinBank led;
	uint16_t count;

public:
	Blinker();
	virtual ~Blinker();

	void schedule();
};

#endif /* BLINKER_H_ */
