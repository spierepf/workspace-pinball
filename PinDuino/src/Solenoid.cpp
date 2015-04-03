/*
 * Solenoid.cpp
 *
 *  Created on: Feb 24, 2015
 *      Author: peter
 */

#include <Arduino.h>

#include "Solenoid.h"

#define TIME_FUNCTION micros()

Solenoid::Solenoid(PinBank& pin) : pin(pin), duration(0) {
	PT_INIT(&pt);
	pin.dataLow();
	pin.dirOut();
}

Solenoid::~Solenoid() {
	// TODO Auto-generated destructor stub
}

void Solenoid::trigger(uint16_t attack, uint8_t sustain) {
	timer.set(TIME_FUNCTION);
	duration = attack;
	pin.dataHigh();
}

void Solenoid::release() {
	pin.dataLow();
	duration = 0;
}

bool Solenoid::triggered() {
	return duration > 0;
}

void Solenoid::schedule() {
	PT_SCHEDULE(run());
}

PT_THREAD(Solenoid::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		PT_WAIT_UNTIL(&pt, triggered() and timer.elapsed(TIME_FUNCTION) > duration);
		release();
	}
	PT_END(&pt);
}
