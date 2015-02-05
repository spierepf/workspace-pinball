/*
 * Blinker.h
 *
 *  Created on: Feb 4, 2015
 *      Author: peter
 */

#ifndef BLINKER_H_
#define BLINKER_H_

#include <pt.h>

#include <Pins.h>

class Blinker {
	struct pt pt;
	Pin13 led;
	uint16_t count;

	PT_THREAD(run());

public:
	Blinker();
	virtual ~Blinker();

	void schedule();
};

#endif /* BLINKER_H_ */
