/*
 * Solenoid.cpp
 *
 *  Created on: Feb 24, 2015
 *      Author: peter
 */

#include <Arduino.h>

#include "Solenoid.h"

#define TIME_FUNCTION micros()

Solenoid::Solenoid(PinBank& pin, volatile uint8_t& tccra, uint8_t pwmMask, uint8_t pwmEnableBits, volatile uint8_t& ocr) : pin(pin), attack(0), sustain(0), tccra(tccra), pwmMask(pwmMask), pwmEnableBits(pwmEnableBits), ocr(ocr) {
	PT_INIT(&pt);
	pin.dataLow();
	pin.dirOut();
}

Solenoid::~Solenoid() {
	// TODO Auto-generated destructor stub
}

void Solenoid::enablePWM() {
	if(pwmMask != 0) tccra = (tccra & ~pwmMask) | pwmEnableBits;
}

void Solenoid::disablePWM() {
	if(pwmMask != 0) tccra = (tccra & ~pwmMask);
}

void Solenoid::setOCR(uint8_t value) {
	if(pwmMask != 0) ocr = value;
}

void Solenoid::trigger(uint16_t attack, uint8_t sustain) {
	timer.set(TIME_FUNCTION);
	this->attack = attack;
	this->sustain = sustain;
	disablePWM();
	pin.dataHigh();
}

void Solenoid::release() {
	disablePWM();
	pin.dataLow();
	attack = 0;
	sustain = 0;
}

void Solenoid::beginSustain() {
	if(sustain == 0) {
		release();
	} else {
		enablePWM();
		setOCR(sustain);
		pin.dataLow();
		attack = 0;
	}
}

bool Solenoid::inAttack() {
	return attack > 0;
}

bool Solenoid::inSustain() {
	return attack == 0 && sustain > 0;
}

bool Solenoid::inRelease() {
	return attack == 0 && sustain == 0;
}

void Solenoid::schedule() {
	PT_SCHEDULE(run());
}

PT_THREAD(Solenoid::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		// if in attack mode but timer has elapsed go to sustain mode
		if(inAttack() && timer.elapsed(TIME_FUNCTION) > attack) beginSustain();
		// if in sustain mode do nothing
		// if in release mode do nothing
		PT_YIELD(&pt);
	}
	PT_END(&pt);
}
