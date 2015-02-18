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
	volatile uint8_t *pinr;
	uint8_t mask;

public:
	enum {B, C, D};

	PinBank(uint8_t port, uint8_t mask) : pinr(&PINB + 3*port), mask(mask) { }

	void dirIn() { pinr[1] &= !mask; }
	void dirOut() { pinr[1] |= mask; }
	uint8_t read() { return pinr[0] & mask; }
	void toggle() { pinr[0] = mask; }
	void dataLow() { pinr[2] &= !mask; }
	void dataHigh() { pinr[2] |= mask; }
};

#endif /* INCLUDE_PINBANK_H_ */
