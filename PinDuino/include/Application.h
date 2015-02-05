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
#include <Timer.h>
#include <PingPong.h>

class Application {
	PinDuinoDataLink& datalink;
	struct pt pt;
	PingPong pingPong;

	PT_THREAD(run());

public:
	Application(PinDuinoDataLink&);
	virtual ~Application();

	void schedule();
};

#endif /* APPLICATION_H_ */
