/*
 * StimulusResponse.h
 *
 *  Created on: Feb 24, 2015
 *      Author: peter
 */

#ifndef STIMULUSRESPONSE_H_
#define STIMULUSRESPONSE_H_

#include <stdint.h>

#include <Solenoid.h>

struct Entry {
	uint8_t enabled : 1;
	uint8_t solenoidIndex : 7;
	uint16_t attack;
	uint8_t sustain;

	Entry() : enabled(1), solenoidIndex(5), attack(65000), sustain(0) {
	}
};

class StimulusResponse {
	Solenoid *solenoids;
	Entry entries[2][12];

public:
	enum { ON_LOW, ON_HIGH };

	StimulusResponse(Solenoid*);
	virtual ~StimulusResponse();

	void trigger(uint8_t, uint8_t);
};

#endif /* STIMULUSRESPONSE_H_ */
