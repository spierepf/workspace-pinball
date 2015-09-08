/*
 * OutgoingDataLink.cpp
 *
 *  Created on: May 20, 2015
 *      Author: peter
 */

#include "OutgoingDataLink.h"

#include <crc.h>

OutgoingDataLink::OutgoingDataLink(Hardware& hardware, FrameBuffer<64, 4>& outgoingFrames) : hardware(hardware), outgoingCRC(0xFFFF), position(0), data(0), outgoingFrames(outgoingFrames) {
	PT_INIT(&outgoing);
}

OutgoingDataLink::~OutgoingDataLink() {
	// TODO Auto-generated destructor stub
}

#define WRITE_HARDWARE(b) PT_WAIT_UNTIL(&outgoing, hardware.putReady()); hardware.put(b)

PT_THREAD(OutgoingDataLink::outgoing_thread()) {
	PT_BEGIN(&outgoing);
	for(;;) {
		PT_WAIT_UNTIL(&outgoing, outgoingFrames.hasFrame());
		currentFrame = outgoingFrames[0];
		outgoingCRC = 0xFFFF;
		WRITE_HARDWARE(FLAG);
		for(position = 0; position < currentFrame.getLength(); position++) {
			data = currentFrame[position];
			crc_ccitt_update(outgoingCRC, data);
			if(data == XON || data == XOFF || data == ESC || data == FLAG) {
				data = data ^ MASK;
				WRITE_HARDWARE(ESC);
			}
			WRITE_HARDWARE(data);
		}

		data = outgoingCRC >> 8;
		if(data == XON || data == XOFF || data == ESC || data == FLAG) {
			data = data ^ MASK;
			WRITE_HARDWARE(ESC);
		}
		WRITE_HARDWARE(data);

		data = outgoingCRC & 0xFF;
		if(data == XON || data == XOFF || data == ESC || data == FLAG) {
			data = data ^ MASK;
			WRITE_HARDWARE(ESC);
		}
		WRITE_HARDWARE(data);
		WRITE_HARDWARE(FLAG);

		outgoingFrames.removeFrame();
	}
	PT_END(&outgoing);
}


void OutgoingDataLink::schedule() {
	PT_SCHEDULE(outgoing_thread());
}
