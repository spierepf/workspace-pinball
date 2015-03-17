/*
 * SolenoidAction.h
 *
 *  Created on: Mar 1, 2015
 *      Author: peter
 */

#ifndef SOLENOIDACTION_H_
#define SOLENOIDACTION_H_

#include <stdint.h>

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

struct SolenoidAction {
	uint8_t enabled : 1;
	uint8_t solenoidIndex : 7;
	uint8_t sustain;
	uint16_t attack;

	SolenoidAction() : enabled(1), solenoidIndex(5), sustain(0), attack(65000) {
	}

	SolenoidAction(bool enabled, uint8_t solenoidIndex, uint16_t attack, uint8_t sustain) : enabled(enabled), solenoidIndex(solenoidIndex), sustain(sustain), attack(attack) {
	}

	void write_to(DataLink& datalink) {
		uint8_t *p = (uint8_t*)this;
		datalink.append_payload(*p++);
		datalink.append_payload(*p++);
		datalink.append_payload(*p++);
		datalink.append_payload(*p++);
	}

	void read_from(DataLink& datalink, uint8_t& i) {
		uint8_t *p = (uint8_t*)this;
		*p++=datalink.peek(i++);
		*p++=datalink.peek(i++);
		*p++=datalink.peek(i++);
		*p++=datalink.peek(i++);
	}
};

#pragma pack(pop)   /* restore original alignment from stack */

#endif /* SOLENOIDACTION_H_ */
