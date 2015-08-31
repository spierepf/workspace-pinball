/*
 * IncomingDataLink.cpp
 *
 *  Created on: May 20, 2015
 *      Author: peter
 */

#include "IncomingDataLink.h"

#include <stdint.h>
#include <crc.h>

IncomingDataLink::IncomingDataLink(Hardware& hardware) : hardware(hardware), current_frame_length(0), incomingCRC(0xFFFF) {
	PT_INIT(&incoming);
}

IncomingDataLink::~IncomingDataLink() {
	// TODO Auto-generated destructor stub
}

PT_THREAD(IncomingDataLink::incoming_thread()) {
	uint8_t b;
	PT_BEGIN(&incoming);
	for (;;) {
		PT_WAIT_UNTIL(&incoming, hardware.getReady());
		b = hardware.get();
		if(b == FLAG) {
			if(current_frame_length != 0) {
				if(incomingCRC == 0) {
					incoming_bytes.revert(2);
					incoming_frame_lengths.put(current_frame_length - 2);
				} else {
					incoming_bytes.revert(current_frame_length);
				}
				current_frame_length = 0;
			}
			incomingCRC = 0xFFFF;
		} else {
			if(b == ESC) {
				PT_WAIT_UNTIL(&incoming, hardware.getReady());
				b = hardware.get() ^ MASK;
			}
			incoming_bytes.put(b);
			crc_ccitt_update(incomingCRC, b);
			current_frame_length++;
		}
		PT_YIELD(&incoming);
	}
	PT_END(&incoming);
}

void IncomingDataLink::schedule() {
	PT_SCHEDULE(incoming_thread());
}

uint8_t IncomingDataLink::peek(uint8_t position) {
	return incoming_bytes.peek(position);
}

bool IncomingDataLink::have_incoming_frame() {
	return !incoming_frame_lengths.empty();
}

uint8_t IncomingDataLink::incoming_frame_length() {
	return incoming_frame_lengths.peek(0);
}

void IncomingDataLink::next_incoming_frame() {
	incoming_bytes.consume(incoming_frame_lengths.get());
}
