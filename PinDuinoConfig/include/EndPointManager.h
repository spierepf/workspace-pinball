/*
 * EndPointManager.h
 *
 *  Created on: Mar 20, 2015
 *      Author: peter
 */

#ifndef ENDPOINTMANAGER_H_
#define ENDPOINTMANAGER_H_

#include <stdio.h>

#include <list>

#include <EndPointWrapper.h>

using namespace std;

class EndPointManager {
	FILE* fifo;
	list<EndPointWrapper*> endPoints;

public:
	EndPointManager();
	virtual ~EndPointManager();

	void setup();
	bool loop();
};

#endif /* ENDPOINTMANAGER_H_ */
