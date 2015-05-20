/*
 * DataLink.h
 *
 *  Created on: Jan 29, 2015
 *      Author: peter
 */

#ifndef DATALINK_H_
#define DATALINK_H_

#include <pt.h>

#include <ByteSource.h>
#include <ByteSink.h>
#include <RingBuffer.h>

/** Handles message framing and flow control.
 *
 * To receive an incoming frame, first ensure that one exists by calling have_incoming_frame().
 * Call incoming_frame_length() to establish the number of bytes in the incoming frame, and peek()
 * to examine the bytes of the incoming frame. peek(0) will return the opcode of the incoming frame.
 * peek(1) through peek(n-1) (where n = incoming_frame_length()) will return any payload bytes of the
 * incoming frame. peek() with an argument n or greater is undefined.
 *
 * When sending an outgoing frame it is critical to avoid accidentally 'nesting' frames. The call
 * sequence must always start with exactly one call to begin_outgoing_frame(), followed by zero or more
 * calls to append_payload(), and terminate with exactly one call to end_outgoing_frame().
 * Call outside of this sequence (including calls to other functions that may themselves call these
 * frame methods) may lead to undefined behaviour.
 */
class DataLink {
private:
	enum {
		XON=0x11,
		XOFF=0x13,
		MASK=0x20,
		ESC=0x7d,
		FLAG=0x7e
	};
	ByteSource& byteSource;
	ByteSink& byteSink;

	struct pt outgoing;
	struct pt incoming;

	RingBuffer<64> outgoing_bytes;
	RingBuffer<64> incoming_bytes;

	uint8_t current_frame_length;
	RingBuffer<4> incoming_frame_lengths;

	PT_THREAD(outgoing_thread());
	PT_THREAD(incoming_thread());

	void put(uint8_t);

public:
	DataLink(ByteSource&, ByteSink&);
	virtual ~DataLink();

	/** Used to schedule our incoming and outgoing protothreads for execution. */
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

	/** Begins a new outgoing frame with the given opcode. */
	void begin_outgoing_frame(uint8_t);

	/** Appends the given payload byte to the current outgoing frame. */
	void append_payload(uint8_t);

	/** Completes the current outgoing frame. */
	void end_outgoing_frame();

	/** Emits a log message. */
	virtual void log(const char*, ...) = 0;
};

#endif /* DATALINK_H_ */
