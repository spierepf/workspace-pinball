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

	OutgoingDataLink& outgoingDatalink;
	PinBank& bank;
	uint8_t mask;
	uint8_t id;
	uint8_t history;
	bool state;

	Timer<unsigned int> timer;

	void pinChange(bool);

public:
	InputPin(OutgoingDataLink&, PinBank&, uint8_t, uint8_t);
	virtual ~InputPin();

	void schedule();
};

#endif /* INPUTPIN_H_ */
