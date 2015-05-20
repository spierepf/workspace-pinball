/*
 * MockHardware.h
 *
 *  Created on: Jan 29, 2015
 *      Author: peter
 */

#ifndef MOCKHARDWARE_H_
#define MOCKHARDWARE_H_

#include <ByteSource.h>
#include <ByteSink.h>

#include <RingBuffer.h>

class MockHardware: public ByteSource, public ByteSink {
public:
	RingBuffer<16>& incoming_bytes;
	RingBuffer<16>& outgoing_bytes;

	MockHardware(RingBuffer<16> &incoming_bytes, RingBuffer<16> &outgoing_bytes) : incoming_bytes(incoming_bytes), outgoing_bytes(outgoing_bytes) {
	}

	virtual ~MockHardware() {
	}

	virtual bool getReady() {
		return !incoming_bytes.empty();
	}

	virtual uint8_t get() {
		return incoming_bytes.get();
	}

	virtual bool putReady() {
		return !outgoing_bytes.full();
	}

	virtual void put(uint8_t b) {
		outgoing_bytes.put(b);
	}
};

#endif /* MOCKHARDWARE_H_ */
