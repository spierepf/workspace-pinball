/*
 * AbstractEndPoint.h
 *
 *  Created on: Apr 2, 2015
 *      Author: peter
 */

#ifndef ABSTRACTENDPOINT_H_
#define ABSTRACTENDPOINT_H_

#include <pt.h>
#include <IncomingDataLink.h>
#include <OutgoingDataLink.h>

class AbstractEndPoint {
	struct pt pt;
	PT_THREAD(run());

protected:
	IncomingDataLink &incomingDatalink;
	OutgoingDataLink &outgoingDatalink;
	virtual void handleIncomingFrame() = 0;

public:
	AbstractEndPoint(IncomingDataLink&, OutgoingDataLink&);
	virtual ~AbstractEndPoint();

	void schedule();
};

#endif /* ABSTRACTENDPOINT_H_ */
