/*
 * MockHardware.h
 *
 *  Created on: Jan 29, 2015
 *      Author: peter
 */

#ifndef MOCKHARDWARE_H_
#define MOCKHARDWARE_H_

#include <RingBuffer.h>

class MockHardware {
public:
	hdlc::RingBuffer<32>& incoming_bytes;
	hdlc::RingBuffer<32>& outgoing_bytes;

	MockHardware(hdlc::RingBuffer<32> &incoming_bytes, hdlc::RingBuffer<32> &outgoing_bytes) : incoming_bytes(incoming_bytes), outgoing_bytes(outgoing_bytes) {
	}

	virtual ~MockHardware() {
	}

	virtual bool getReady() {
		return !incoming_bytes.isEmpty();
	}

	virtual uint8_t get() {
		return incoming_bytes.get();
	}

	virtual bool putReady() {
		return !outgoing_bytes.isFull();
	}

	virtual void put(uint8_t b) {
		outgoing_bytes.put(b);
	}
};

#endif /* MOCKHARDWARE_H_ */
