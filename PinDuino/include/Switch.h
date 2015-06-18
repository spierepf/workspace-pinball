/*
 * Switch.h
 *
 *  Created on: Feb 19, 2015
 *      Author: peter
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include <EndPoint.h>

#include <Timer.h>

class Switch {
	friend class SwitchBank;

	OutgoingPinDuinoDataLink& outgoingDatalink;
	uint8_t mask;
	uint8_t id;
	uint8_t history;
	bool state;
	uint8_t& dirtyList;

	void pinChange(bool);

public:
	Switch(OutgoingPinDuinoDataLink&, uint8_t, uint8_t, uint8_t&);
	virtual ~Switch();

	void update(uint8_t);
};

#endif /* SWITCH_H_ */
