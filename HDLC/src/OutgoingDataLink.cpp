/*
 * OutgoingDataLink.cpp
 *
 *  Created on: May 20, 2015
 *      Author: peter
 */

#include "OutgoingDataLink.h"

OutgoingDataLink::OutgoingDataLink(ByteSink& byteSink) : byteSink(byteSink) {
	PT_INIT(&outgoing);
}

OutgoingDataLink::~OutgoingDataLink() {
	// TODO Auto-generated destructor stub
}

void OutgoingDataLink::put(uint8_t b) {
	if(b == XON || b == XOFF || b == ESC || b == FLAG) {
		outgoing_bytes.put(ESC);
		outgoing_bytes.put(b ^ MASK);
	} else {
		outgoing_bytes.put(b);
	}
}

PT_THREAD(OutgoingDataLink::outgoing_thread()) {
	PT_BEGIN(&outgoing);
	for (;;) {
		PT_WAIT_UNTIL(&outgoing, byteSink.putReady() && !outgoing_bytes.empty());
		byteSink.put(outgoing_bytes.get());
		PT_YIELD(&outgoing);
	}
	PT_END(&outgoing);
}


void OutgoingDataLink::schedule() {
	PT_SCHEDULE(outgoing_thread());
}

void OutgoingDataLink::begin_outgoing_frame(uint8_t opcode) {
	if(outgoing_bytes.empty()) outgoing_bytes.put(FLAG);
	put(opcode);
}

void OutgoingDataLink::append_payload(uint8_t payload) {
	put(payload);
}

void OutgoingDataLink::end_outgoing_frame() {
	outgoing_bytes.put(FLAG);
}