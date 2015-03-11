/*
 * StimulusActionMap.h
 *
 *  Created on: Mar 11, 2015
 *      Author: peter
 */

#ifndef INCLUDE_STIMULUSACTIONMAP_H_
#define INCLUDE_STIMULUSACTIONMAP_H_

#include <Stimulus.h>
#include <SolenoidAction.h>

class StimulusActionMap {
	SolenoidAction actions[12][2];

public:
	SolenoidAction& operator[](Stimulus stimulus) {
		return actions[stimulus.pin][stimulus.newState];
	}
};

#endif /* INCLUDE_STIMULUSACTIONMAP_H_ */
