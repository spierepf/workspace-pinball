/*
 * InputPin.h
 *
 *  Created on: Feb 19, 2015
 *      Author: peter
 */

#ifndef INPUTPIN_H_
#define INPUTPIN_H_

#include <pt.h>

#include <Application.h>
#include <Timer.h>
#include <PinBank.h>

class InputPin {
	struct pt pt;
	PT_THREAD(run());

	Application& application;
	PinBank& bank;
	uint8_t mask;
	Timer<unsigned long> t;

public:
	InputPin(Application&, PinBank&, uint8_t);
	virtual ~InputPin();

	void schedule();
};

#endif /* INPUTPIN_H_ */
