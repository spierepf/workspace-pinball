/*
 * DataLink.h
 *
 *  Created on: Jan 29, 2015
 *      Author: peter
 */

#ifndef DATALINK_H_
#define DATALINK_H_

#include <pt.h>

#include <Hardware.h>
#include <RingBuffer.h>

namespace OpCode {
	enum {
		PING,
		PONG,
		LOG
	};
}

class DataLink {
private:
	enum {
		XON=0x10,
		XOFF=0x13,
		MASK=0x20,
		ESC=0x7d,
		FLAG=0x7e
	};
	Hardware& hardware;

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
	DataLink(Hardware&);
	virtual ~DataLink();

	void schedule();

	uint8_t peek(uint8_t);
	bool have_incoming_frame();
	uint8_t incoming_frame_length();
	void next_incoming_frame();

	void begin_outgoing_frame(uint8_t);
	void append_payload(uint8_t);
	void end_outgoing_frame();

	void log(const char*, ...);
};

#endif /* DATALINK_H_ */
