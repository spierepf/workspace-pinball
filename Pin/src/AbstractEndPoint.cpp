/*
 * AbstractEndPoint.cpp
 *
 *  Created on: Apr 2, 2015
 *      Author: peter
 */

#include "AbstractEndPoint.h"

AbstractEndPoint::AbstractEndPoint(DataLink &datalink) : datalink(datalink) {
	PT_INIT(&pt);
}

AbstractEndPoint::~AbstractEndPoint() {
	// TODO Auto-generated destructor stub
}

PT_THREAD(AbstractEndPoint::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		PT_WAIT_UNTIL(&pt, datalink.have_incoming_frame());

		handleIncomingFrame();

		datalink.next_incoming_frame();
	}
	PT_END(&pt);
}

void AbstractEndPoint::schedule() {
	datalink.schedule();
	PT_SCHEDULE(run());
}
