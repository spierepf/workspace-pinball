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

#include <string>
using namespace std;

class Application {
	string device;
	Hardware* hardware;
	DataLink datalink;
	int id;
	struct pt pt;

	PT_THREAD(run());

public:
	Application(string, Hardware*);
	virtual ~Application();

	void schedule();
};

#endif /* APPLICATION_H_ */
