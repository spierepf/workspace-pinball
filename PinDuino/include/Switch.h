/*
 * Switch.h
 *
 *  Created on: Feb 19, 2015
 *      Author: peter
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include <EndPoint.h>
#include <pt.h>

#include <PinBank.h>
#include <Timer.h>

class Switch {
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
	Switch(OutgoingDataLink&, PinBank&, uint8_t, uint8_t);
	virtual ~Switch();

	void schedule();
};

#endif /* SWITCH_H_ */
