/*
 * AbstractEndPoint.cpp
 *
 *  Created on: Apr 2, 2015
 *      Author: peter
 */

#include "AbstractEndPoint.h"

AbstractEndPoint::AbstractEndPoint(IncomingDataLink &incomingDatalink, OutgoingDataLink &outgoingDatalink, FrameBuffer<64, 4> &incomingFrames) : incomingDatalink(incomingDatalink), outgoingDatalink(outgoingDatalink), incomingFrames(incomingFrames) {
	PT_INIT(&pt);
}

AbstractEndPoint::~AbstractEndPoint() {
	// TODO Auto-generated destructor stub
}

PT_THREAD(AbstractEndPoint::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		PT_WAIT_UNTIL(&pt, incomingDatalink.have_incoming_frame());

		handleIncomingFrame();

		incomingDatalink.next_incoming_frame();
	}
	PT_END(&pt);
}

void AbstractEndPoint::schedule() {
	incomingDatalink.schedule();
	outgoingDatalink.schedule();
	PT_SCHEDULE(run());
}
