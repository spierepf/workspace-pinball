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

/** Handles message framing and flow control.
 *
 * When sending an outgoing frame it is critical to avoid accidentally 'nesting' frames. The call
 * sequence must always start with exactly one call to begin_outgoing_frame(), followed by zero or more
 * calls to append_payload(), and terminate with exactly one call to end_outgoing_frame().
 * Call outside of this sequence (including calls to other functions that may themselves call these
 * frame methods) may lead to undefined behaviour.
 */
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
	FrameBuffer<64, 4> outgoingFrames;
	FrameBuffer<64, 4>::Frame currentFrame;
	uint8_t position;
	uint8_t data;

protected:
	PT_THREAD(outgoing_thread());

public:
	OutgoingDataLink(Hardware&, FrameBuffer<64, 4>& outgoingFrames);
	virtual ~OutgoingDataLink();

	/** Used to schedule our protothread for execution. */
	void schedule();

	/** Begins a new outgoing frame with the given opcode. */
	void begin_outgoing_frame(uint8_t);

	/** Appends the given payload byte to the current outgoing frame. */
	void append_payload(uint8_t);

	/** Completes the current outgoing frame. */
	void end_outgoing_frame();
};

#endif /* OUTGOINGDATALINK_H_ */
