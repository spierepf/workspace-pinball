/*
 * Pins.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef INCLUDE_PINS_H_
#define INCLUDE_PINS_H_

#include <avr/io.h>

#include <stdint.h>

class Pin {
	volatile uint8_t *PINR;
	uint8_t MASK;

public:
	Pin(volatile uint8_t *PINR, uint8_t MASK) : PINR(PINR), MASK(MASK) { }

	void dirIn() { PINR[1] &= !MASK; }
	void dirOut() { PINR[1] |= MASK; }
	uint8_t read() { return PINR[0] & MASK; }
	void toggle() { PINR[0] = MASK; }
	void dataLow() { PINR[2] &= !MASK; }
	void dataHigh() { PINR[2] |= MASK; }
};

#endif /* INCLUDE_PINS_H_ */
