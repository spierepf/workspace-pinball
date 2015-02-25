/*
 * Application.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <pt.h>

#include <PinDuinoDataLink.h>
#include <PingPong.h>
#include <StimulusResponse.h>

class Application {
	struct pt pt;
	PT_THREAD(run());

	PingPong& pingPong;
	StimulusResponse& stimulusResponse;

public:
	Application(PingPong&, StimulusResponse&);
	virtual ~Application();

	void schedule();

	void pinChange(uint8_t, bool);
};

#endif /* APPLICATION_H_ */
