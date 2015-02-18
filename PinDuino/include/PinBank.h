/*
 * PinBank.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef INCLUDE_PINBANK_H_
#define INCLUDE_PINBANK_H_

#include <avr/io.h>

#include <stdint.h>

class PinBank {
	volatile uint8_t *PINR;
	uint8_t MASK;

public:
	enum {B, C, D};

	PinBank(uint8_t bank, uint8_t MASK) : PINR(&PINB + 3*bank), MASK(MASK) { }

	void dirIn() { PINR[1] &= !MASK; }
	void dirOut() { PINR[1] |= MASK; }
	uint8_t read() { return PINR[0] & MASK; }
	void toggle() { PINR[0] = MASK; }
	void dataLow() { PINR[2] &= !MASK; }
	void dataHigh() { PINR[2] |= MASK; }
};

#endif /* INCLUDE_PINBANK_H_ */
