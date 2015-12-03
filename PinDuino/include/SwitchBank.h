/*
 * SwitchBank.h
 *
 *  Created on: Jun 17, 2015
 *      Author: peter
 */

#ifndef SWITCHBANK_H_
#define SWITCHBANK_H_

#include <ItemBank.h>

#include <Switch.h>
#include <PinBank.h>

class SwitchBank : public ItemBank {
	PinBank& pinBank;
	uint8_t last;
	uint8_t current;

	virtual void updateSelf() {
		last = current;
		current = pinBank.read();
		dirtyList |= last ^ current;
	}

	virtual void updateItem(Item* item) {
		((Switch*)item)->update(micros(), last, current);
	}

public:
	SwitchBank(Item** switches, PinBank& pinBank, uint8_t& dirtyList) : ItemBank(switches, dirtyList), pinBank(pinBank), last(0), current(0) {}
	virtual ~SwitchBank() {}
};

#endif /* SWITCHBANK_H_ */
