/*
 * DataLink.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: peter
 */

#include <DataLink.h>

DataLink::DataLink(Hardware& hardware) : hardware(hardware), current_frame_length(0) {
	PT_INIT(&outgoing);
	PT_INIT(&incoming);
}

DataLink::~DataLink() {
	// TODO Auto-generated destructor stub
}

void DataLink::put(uint8_t b) {
	if(b == 0x11 || b == 0x13 || b == 0x7d || b == 0x7e) {
		outgoing_bytes.put(0x7d);
		outgoing_bytes.put(b ^ 0x20);
	} else {
		outgoing_bytes.put(b);
	}
}

void DataLink::schedule() {
	PT_SCHEDULE(outgoing_thread());
	PT_SCHEDULE(incoming_thread());
}

PT_THREAD(DataLink::outgoing_thread()) {
	PT_BEGIN(&outgoing);
	for (;;) {
		PT_WAIT_UNTIL(&outgoing, hardware.put_ready() && !outgoing_bytes.empty());
		hardware.put(outgoing_bytes.get());
		PT_YIELD(&outgoing);
	}
	PT_END(&outgoing);
}

PT_THREAD(DataLink::incoming_thread()) {
	uint8_t b;
	PT_BEGIN(&incoming);
	for (;;) {
		PT_WAIT_UNTIL(&incoming, hardware.get_ready());
		b = hardware.get();
		if(b == FLAG) {
			if(current_frame_length != 0) {
				incoming_frame_lengths.put(current_frame_length);
				current_frame_length = 0;
			}
		} else {
			if(b == ESC) {
				PT_WAIT_UNTIL(&incoming, hardware.get_ready());
				b = hardware.get() ^ MASK;
			}
			incoming_bytes.put(b);
			current_frame_length++;
		}
		PT_YIELD(&incoming);
	}
	PT_END(&incoming);
}

uint8_t DataLink::peek(uint8_t position) {
	return incoming_bytes.peek(position);
}

bool DataLink::have_incoming_frame() {
	return !incoming_frame_lengths.empty();
}

uint8_t DataLink::incoming_frame_length() {
	return incoming_frame_lengths.peek(0);
}

void DataLink::next_incoming_frame() {
	incoming_bytes.consume(incoming_frame_lengths.get());
}

void DataLink::begin_outgoing_frame(uint8_t opcode) {
	if(outgoing_bytes.empty()) outgoing_bytes.put(FLAG);
	put(opcode);
}

void DataLink::append_payload(uint8_t payload) {
	put(payload);
}

void DataLink::end_outgoing_frame() {
	outgoing_bytes.put(FLAG);
}
