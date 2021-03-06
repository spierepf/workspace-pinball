/*
 * Switch.h
 *
 *  Created on: Feb 19, 2015
 *      Author: peter
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include <Item.h>
#include <SwitchListener.h>
#include <Timer.h>

class Switch : public Item {
	const SwitchListener& switchListener;
	uint8_t port;
	uint8_t id;
	bool state;

	Timer<uint32_t> debounceTimer;
	uint32_t debounceThreshold[2];

	void pinChange(bool);

public:
	Switch(const SwitchListener&, uint8_t, uint8_t, uint8_t, uint8_t&);
	~Switch();

	void update(uint32_t, uint8_t, uint8_t);

	void setDebounceThreshold(bool, uint32_t);
	void setPullup(bool);
};

#endif /* SWITCH_H_ */
