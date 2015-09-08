/*
 * OutgoingDataLink.h
 *
 *  Created on: May 20, 2015
 *      Author: peter
 */

#ifndef OUTGOINGDATALINK_H_
#define OUTGOINGDATALINK_H_

#include <pt.h>

#include <Hardware.h>
#include <FrameBuffer.h>

class OutgoingDataLink {
	enum {
		XON=0x11,
		XOFF=0x13,
		MASK=0x20,
		ESC=0x7d,
		FLAG=0x7e
	};

	struct pt outgoing;
	Hardware& hardware;
	uint16_t outgoingCRC;
	FrameBuffer<64, 4>::Frame currentFrame;
	uint8_t position;
	uint8_t data;

protected:
	FrameBuffer<64, 4>& outgoingFrames;

	PT_THREAD(outgoing_thread());

public:
	OutgoingDataLink(Hardware&, FrameBuffer<64, 4>& outgoingFrames);
	virtual ~OutgoingDataLink();

	/** Used to schedule our protothread for execution. */
	void schedule();
};

#endif /* OUTGOINGDATALINK_H_ */
