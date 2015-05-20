/*
 * StimulusResponse.h
 *
 *  Created on: Feb 24, 2015
 *      Author: peter
 */

#ifndef STIMULUSRESPONSE_H_
#define STIMULUSRESPONSE_H_

#include <stdint.h>

#include <IncomingDataLink.h>
#include <OutgoingDataLink.h>

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

extern StimulusResponse stimulusResponse;

#endif /* STIMULUSRESPONSE_H_ */
