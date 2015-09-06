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

/** Handles message framing and flow control.
 *
 * To receive an incoming frame, first ensure that one exists by calling have_incoming_frame().
 * Call incoming_frame_length() to establish the number of bytes in the incoming frame, and peek()
 * to examine the bytes of the incoming frame. peek(0) will return the opcode of the incoming frame.
 * peek(1) through peek(n-1) (where n = incoming_frame_length()) will return any payload bytes of the
 * incoming frame. peek() with an argument n or greater is undefined.
 */
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

	/** Used to schedule our protothread for execution. */
	void schedule();

	/** Returns the nth byte in the incoming frame.
	 *
	 * This function is used by incoming frame handlers to inspect incoming frames.
	 * One should use incoming_frame_length() to determine the length of the next incoming
	 * frame and limit peek() requests accordingly.
	 */
	uint8_t peek(uint8_t);

	/** Returns true if there is an incoming frame available. */
	bool have_incoming_frame();

	/** Returns the length of the next incoming frame (if one exists).
	 *
	 * The return value of this function is undefined if have_incoming_frame() is false.
	 */
	uint8_t incoming_frame_length();

	/** Advance to the next incoming frame. */
	void next_incoming_frame();
};

#endif /* INCOMINGDATALINK_H_ */
