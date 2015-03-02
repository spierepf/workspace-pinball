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
#include <SolenoidAction.h>

class StimulusResponse {
	bool enabled;
	SolenoidAction entries[2][12];

public:
	StimulusResponse();
	virtual ~StimulusResponse();

	void config(uint8_t, bool, SolenoidAction);
	void trigger(uint8_t, bool);

	void inhibit();
	void enable();
};

extern StimulusResponse stimulusResponse;

#endif /* STIMULUSRESPONSE_H_ */
