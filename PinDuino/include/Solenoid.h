/*
 * Solenoid.h
 *
 *  Created on: Feb 24, 2015
 *      Author: peter
 */

#ifndef SOLENOID_H_
#define SOLENOID_H_

#include <stdint.h>

#include <pt.h>
#include <PinBank.h>
#include <Timer.h>

class Solenoid {
	struct pt pt;
	PT_THREAD(run());

	PinBank pin;
	uint16_t duration;
	Timer<uint16_t> timer;

public:
	Solenoid(PinBank&);
	virtual ~Solenoid();

	void trigger(uint16_t, uint8_t);
	void release();
	bool triggered();

	void schedule();
};

#endif /* SOLENOID_H_ */
