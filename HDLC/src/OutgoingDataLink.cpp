/*
 * OutgoingDataLink.cpp
 *
 *  Created on: May 20, 2015
 *      Author: peter
 */

#include "OutgoingDataLink.h"

#include <crc.h>

OutgoingDataLink::OutgoingDataLink(Hardware& hardware) : hardware(hardware) {
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
		PT_WAIT_UNTIL(&outgoing, hardware.putReady() && !outgoing_bytes.empty());
		hardware.put(outgoing_bytes.get());
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
	outgoingCRC = 0xFFFF;
	crc_ccitt_update(outgoingCRC, opcode);
}

void OutgoingDataLink::append_payload(uint8_t payload) {
	put(payload);
	crc_ccitt_update(outgoingCRC, payload);
}

void OutgoingDataLink::end_outgoing_frame() {
	put(outgoingCRC >> 8);
	put(outgoingCRC & 0xFF);
	outgoing_bytes.put(FLAG);
}
