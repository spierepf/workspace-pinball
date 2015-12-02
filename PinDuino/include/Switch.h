/*
 * Switch.h
 *
 *  Created on: Feb 19, 2015
 *      Author: peter
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include <extern.h>

#include <Item.h>

class Switch : public Item {
	friend class SwitchBank;

	uint8_t id;
	bool state;

	Timer<uint32_t> debounceTimer;
	uint16_t debounceThreshold;

	void pinChange(bool);

public:
	Switch(uint8_t, uint8_t, uint8_t&);
	virtual ~Switch();

	void update(uint8_t, uint8_t);
};

#endif /* SWITCH_H_ */
