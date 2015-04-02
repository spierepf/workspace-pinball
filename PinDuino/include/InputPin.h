/*
 * InputPin.h
 *
 *  Created on: Feb 19, 2015
 *      Author: peter
 */

#ifndef INPUTPIN_H_
#define INPUTPIN_H_

#include <EndPoint.h>
#include <pt.h>

#include <PinBank.h>
#include <Timer.h>

class InputPin {
	struct pt pt;
	PT_THREAD(run());

	EndPoint& endPoint;
	PinBank& bank;
	uint8_t mask;
	uint8_t id;
	Timer<unsigned int> timer;

	void pinChange(bool);

public:
	InputPin(EndPoint&, PinBank&, uint8_t, uint8_t);
	virtual ~InputPin();

	void schedule();
};

#endif /* INPUTPIN_H_ */
