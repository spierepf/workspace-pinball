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

class Application {
	struct pt pt;
	PT_THREAD(run());

	PinDuinoDataLink& datalink;
	PingPong& pingPong;

public:
	Application(PinDuinoDataLink&, PingPong&);
	virtual ~Application();

	void schedule();

	void pin_low(uint8_t);
	void pin_high(uint8_t);
};

#endif /* APPLICATION_H_ */
