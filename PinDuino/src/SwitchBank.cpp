/*
 * SwitchBank.cpp
 *
 *  Created on: Jun 17, 2015
 *      Author: peter
 */

#include "SwitchBank.h"

SwitchBank::SwitchBank(Switch** switches, PinBank& pinBank, uint8_t& dirtyList) : switches(switches), pinBank(pinBank), last(0), current(0), dirtyList(dirtyList) {
	// TODO Auto-generated constructor stub

}

SwitchBank::~SwitchBank() {
	// TODO Auto-generated destructor stub
}

void SwitchBank::schedule() {
	PT_SCHEDULE(run());
}

PT_THREAD(SwitchBank::run()) {
	uint8_t delta;

	PT_BEGIN(&pt);
	for(;;) {
		last = current;
		current = pinBank.read();
		delta = last ^ current;
		dirtyList |= delta;

		if(dirtyList) {
			for(uint8_t i = 0; i < 8; i++) {
				if((switches[i] != NULL) && ((dirtyList & switches[i]->mask) != 0)) {
					switches[i] -> update(current);
				}
			}
		}
		PT_YIELD(&pt);
	}
	PT_END(&pt);
}
