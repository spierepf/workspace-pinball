/*
 * SwitchBank.cpp
 *
 *  Created on: Jun 17, 2015
 *      Author: peter
 */

#include "SwitchBank.h"

#include <stdlib.h>

SwitchBank::SwitchBank(Switch** switches, PinBank& pinBank, uint8_t& dirtyList) : items(switches), pinBank(pinBank), last(0), current(0), dirtyList(dirtyList) {
	PT_INIT(&pt);
}

SwitchBank::~SwitchBank() {
	// TODO Auto-generated destructor stub
}

void SwitchBank::schedule() {
	PT_SCHEDULE(run());
}

PT_THREAD(SwitchBank::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		updateSelf();
		if(dirtyList) {
			for(uint8_t i = 0; i < 8; i++) {
				if((items[i] != NULL) && ((dirtyList & items[i]->mask) != 0)) {
					updateItem(items[i]);
				}
			}
		}
		PT_YIELD(&pt);
	}
	PT_END(&pt);
}
