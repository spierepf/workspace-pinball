/*
 * Blinker.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: peter
 */

#include "Blinker.h"

Blinker::Blinker() : count(0) {
	led.dataLow();
	led.dirOut();
	PT_INIT(&pt);
}

Blinker::~Blinker() {
	// TODO Auto-generated destructor stub
}

void Blinker::schedule() {
	PT_SCHEDULE(run());
}

PT_THREAD(Blinker::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		PT_YIELD(&pt);
		if(count++ == 0xFFFF) led.toggle();
	}
	PT_END(&pt);
}
