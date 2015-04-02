/*
 * EndPoint.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef ENDPOINT_H_
#define ENDPOINT_H_

#include <pt.h>

#include <PinDuinoDataLink.h>
#include <PingPong.h>

class EndPoint {
	struct pt pt;
	PT_THREAD(run());

	PingPong& pingPong;

public:
	EndPoint(PingPong&);
	virtual ~EndPoint();

	void schedule();
};

#endif /* ENDPOINT_H_ */
