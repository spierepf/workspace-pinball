/*
 * Application.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <pt.h>

#include <Tty.h>
#include <DataLink.h>

class Application {
	Tty hardware;
	DataLink datalink;
	struct pt pt;

	PT_THREAD(run());

public:
	Application(const char* device);
	virtual ~Application();

	void schedule();
};

#endif /* APPLICATION_H_ */
