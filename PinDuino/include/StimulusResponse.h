/*
 * StimulusResponse.h
 *
 *  Created on: Feb 24, 2015
 *      Author: peter
 */

#ifndef STIMULUSRESPONSE_H_
#define STIMULUSRESPONSE_H_

#include <extern.h>

#include <stdint.h>

#include <Stimulus.h>
#include <Solenoid.h>
#include <SolenoidAction.h>
#include <StimulusActionMap.h>

class StimulusResponse : public StimulusActionMap {
	bool enabled;

public:
	StimulusResponse();
	virtual ~StimulusResponse();

	void trigger(Stimulus);

	void inhibit();
	void enable();
};

#endif /* STIMULUSRESPONSE_H_ */
