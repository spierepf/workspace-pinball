/*
 * IncomingDataLink.cpp
 *
 *  Created on: May 20, 2015
 *      Author: peter
 */

#include "IncomingDataLink.h"

#include <stdint.h>
#include <crc.h>

IncomingDataLink::IncomingDataLink(Hardware& hardware, FrameBuffer<64, 4>& incomingFrames) : hardware(hardware), incomingCRC(0xFFFF), current_frame_length(0), incomingFrames(incomingFrames) {
	PT_INIT(&incoming);
}

IncomingDataLink::~IncomingDataLink() {
	// TODO Auto-generated destructor stub
}

#define READ_HARDWARE() PT_WAIT_UNTIL(&incoming, hardware.getReady()); b = hardware.get()

PT_THREAD(IncomingDataLink::incoming_thread()) {
	uint8_t b;
	PT_BEGIN(&incoming);
	for (;;) {
		READ_HARDWARE();
		if(b == FLAG) {
			if(current_frame_length != 0) {
				if(incomingCRC == 0) {
					incomingFrames.revert(2);
					incomingFrames.endFrame();
				} else {
					incomingFrames.revertFrame();
				}
				current_frame_length = 0;
			}
			incomingCRC = 0xFFFF;
		} else {
			if(b == ESC) {
				READ_HARDWARE();
				b ^= MASK;
			}
			incomingFrames.put(b);
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
	return incomingFrames[0][position];
}

bool IncomingDataLink::have_incoming_frame() {
	return incomingFrames.hasFrame();
}

uint8_t IncomingDataLink::incoming_frame_length() {
	return incomingFrames[0].getLength();
}

void IncomingDataLink::next_incoming_frame() {
	incomingFrames.removeFrame();
}
