/*
 * IncomingDataLink.h
 *
 *  Created on: May 20, 2015
 *      Author: peter
 */

#ifndef INCOMINGDATALINK_H_
#define INCOMINGDATALINK_H_

#include <pt.h>

#include <Hardware.h>
#include <FrameBuffer.h>

class IncomingDataLink {
	enum {
		XON=0x11,
		XOFF=0x13,
		MASK=0x20,
		ESC=0x7d,
		FLAG=0x7e
	};

protected:
	struct pt incoming;
	Hardware& hardware;
	uint16_t incomingCRC;
	uint8_t current_frame_length;
	FrameBuffer<64, 4>& incomingFrames;

	PT_THREAD(incoming_thread());

public:
	IncomingDataLink(Hardware&, FrameBuffer<64, 4>&);
	virtual ~IncomingDataLink();

	void schedule();
};

#endif /* INCOMINGDATALINK_H_ */
