/*
 * AbstractEndPoint.h
 *
 *  Created on: Apr 2, 2015
 *      Author: peter
 */

#ifndef ABSTRACTENDPOINT_H_
#define ABSTRACTENDPOINT_H_

#include <pt.h>
#include <DataLink.h>

class AbstractEndPoint {
	struct pt pt;
	PT_THREAD(run());

protected:
	DataLink &datalink;
	virtual void handleIncomingFrame() = 0;

public:
	AbstractEndPoint(DataLink&);
	virtual ~AbstractEndPoint();

	void schedule();
};

#endif /* ABSTRACTENDPOINT_H_ */
