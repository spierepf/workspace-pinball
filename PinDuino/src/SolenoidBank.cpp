/*
 * SolenoidBank.cpp
 *
 *  Created on: Jun 18, 2015
 *      Author: peter
 */

#include "SolenoidBank.h"

#include <stdlib.h>

SolenoidBank::SolenoidBank(Solenoid** solenoids, uint8_t& dirtyList) : items(solenoids), dirtyList(dirtyList) {
	PT_INIT(&pt);
}

SolenoidBank::~SolenoidBank() {
	// TODO Auto-generated destructor stub
}

void SolenoidBank::schedule() {
	PT_SCHEDULE(run());
}

PT_THREAD(SolenoidBank::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		updateSelf();
		if(dirtyList) {
			for(int i = 0; i < 8; i++) {
				if((items[i] != NULL) && ((dirtyList & items[i]->mask) != 0)) {
					updateItem(items[i]);
				}
			}
		}
		PT_YIELD(&pt);
	}
	PT_END(&pt);
}
