/*
 * Stimulus.h
 *
 *  Created on: Mar 11, 2015
 *      Author: peter
 */

#ifndef STIMULUS_H_
#define STIMULUS_H_

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

struct Stimulus {
	uint8_t pin : 7;
	uint8_t newState : 1;

	Stimulus() : pin(0), newState(false) {
	}

	Stimulus(uint8_t pin, bool newState) : pin(pin), newState(newState) {
	}

	void write_to(DataLink& datalink) {
		uint8_t *p = (uint8_t*)this;
		datalink.append_payload(*p++);
	}

	void read_from(DataLink& datalink, uint8_t& i) {
		uint8_t *p = (uint8_t*)this;
		*p++=datalink.peek(i++);
	}
};

#pragma pack(pop)   /* restore original alignment from stack */

#endif /* STIMULUS_H_ */
