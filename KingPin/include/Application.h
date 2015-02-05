/*
 * Application.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <pt.h>

#include <DataLink.h>

class Application {
	DataLink datalink;
	struct pt pt;

	PT_THREAD(run());

public:
	Application(DataLink&);
	virtual ~Application();

	void schedule();
};

#endif /* APPLICATION_H_ */
