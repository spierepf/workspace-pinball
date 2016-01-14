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

class SwitchBank : public ItemBank<SwitchBank> {
	PinBank& pinBank;
	uint8_t last;
	uint8_t current;

public:
	SwitchBank(Switch** switches, PinBank& pinBank, uint8_t& dirtyList) : ItemBank<SwitchBank>((Item**)switches, dirtyList), pinBank(pinBank), last(0), current(0) {}
	~SwitchBank() {}

	void updateSelf_impl() {
		last = current;
		current = pinBank.read();
		dirtyList |= last ^ current;
	}

	void updateItem_impl(Item* item) {
		((Switch*)item)->update(micros(), last, current);
	}
};

#endif /* SWITCHBANK_H_ */
