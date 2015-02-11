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
	PingPong pingPong;

public:
	Application(PinDuinoDataLink&);
	virtual ~Application();

	void schedule();
};

#endif /* APPLICATION_H_ */
