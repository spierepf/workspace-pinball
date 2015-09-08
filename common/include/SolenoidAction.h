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

	SolenoidAction(uint32_t src) {
		*((uint32_t*)this) = src;
	}

	void write_to(FrameBuffer<64, 4>& frameBuffer) {
		uint8_t *p = (uint8_t*)this;
		frameBuffer.put(*p++);
		frameBuffer.put(*p++);
		frameBuffer.put(*p++);
		frameBuffer.put(*p++);
	}

	void read_from(FrameBuffer<64, 4>::Frame frame, uint8_t& i) {
		uint8_t *p = (uint8_t*)this;
		*p++=frame[i++];
		*p++=frame[i++];
		*p++=frame[i++];
		*p++=frame[i++];
	}

	operator uint32_t() {
		return *(uint32_t*)this;
	}
};

#pragma pack(pop)   /* restore original alignment from stack */

#endif /* SOLENOIDACTION_H_ */
