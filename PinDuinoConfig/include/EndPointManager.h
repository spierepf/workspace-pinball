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

#include <EndPoint.h>

using namespace std;

class EndPointManager {
	FILE* fifo;
	list<EndPoint*> endPoints;

public:
	EndPointManager();
	virtual ~EndPointManager();

	void setup();
	bool loop();
};

#endif /* ENDPOINTMANAGER_H_ */
