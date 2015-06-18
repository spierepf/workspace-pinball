/*
 * SolenoidBank.cpp
 *
 *  Created on: Jun 18, 2015
 *      Author: peter
 */

#include "SolenoidBank.h"

#include <stdlib.h>

SolenoidBank::SolenoidBank(Solenoid** solenoids, uint8_t& dirtyList) : solenoids(solenoids), dirtyList(dirtyList) {
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
		if(dirtyList) {
			for(int i = 0; i < 8; i++) {
				if((solenoids[i] != NULL) && ((dirtyList & solenoids[i]->mask) != 0)) {
					solenoids[i] -> update();
				}
			}
		}
		PT_YIELD(&pt);
	}
	PT_END(&pt);
}
