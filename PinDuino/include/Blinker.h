/*
 * Blinker.h
 *
 *  Created on: Feb 4, 2015
 *      Author: peter
 */

#ifndef BLINKER_H_
#define BLINKER_H_

#include <pt.h>

#include <Timer.h>

#include <PinBank.h>

class Blinker {
	struct pt pt;
	PT_THREAD(run());

	Timer<unsigned long> timer;

	uint16_t count;

public:
	Blinker();
	virtual ~Blinker();

	void schedule();
};

#endif /* BLINKER_H_ */
