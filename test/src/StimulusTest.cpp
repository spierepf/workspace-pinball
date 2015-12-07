/*
 * StimulusTest.cpp
 *
 *  Created on: Sep 1, 2015
 *      Author: peter
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <RingBuffer.h>

#include <Stimulus.h>

#include <EscapingSource.h>
#include <EscapingSink.h>
#include <FrameBuffer.h>
#include <FrameReceiver.h>
#include <FrameTransmitter.h>

class MockSource : public hdlc::Source<uint8_t> {
	hdlc::RingBuffer<32> &buffer;
public:
	MockSource(hdlc::RingBuffer<32> &buffer) : buffer(buffer) {}
    bool isReady() { return !buffer.isEmpty(); }
    uint8_t read() { return buffer.get(); }
};

class MockSink : public hdlc::Sink<uint8_t> {
	hdlc::RingBuffer<32> &buffer;
public:
	MockSink(hdlc::RingBuffer<32> &buffer) : buffer(buffer) {}
    bool isReady() { return !buffer.isFull(); }
    void write(const uint8_t& octet) { buffer.put(octet); }
};

class TestFrameHandler : public hdlc::FrameHandler {
public:
	uint8_t header;
	const uint8_t* payload;
	uint8_t payloadSize;

	virtual void handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) {
		this->header = header;
		this->payload = payload;
		this->payloadSize = payloadSize;
	}
};

BOOST_AUTO_TEST_CASE(stimulus_test)
{
	Stimulus s(2, true);
	uint8_t *b = (uint8_t*)&s;

	BOOST_CHECK_EQUAL(128+2, *b);
}

BOOST_AUTO_TEST_CASE( read_write_stimulus_through_datalink ) {
	BOOST_CHECK( 1 == sizeof(Stimulus) );
	Stimulus stimulus(4, true);

	hdlc::RingBuffer<32> buffer;

	MockSource mockSource(buffer);
	MockSink mockSink(buffer);

	hdlc::EscapingSource source(mockSource);
	hdlc::EscapingSink sink(mockSink);

	hdlc::FrameReceiver receiver(source);
	hdlc::FrameTransmitter transmitter(sink);

	TestFrameHandler handler;

	receiver.setFrameHandler(&handler);

	hdlc::FrameBuffer outgoingFrames;
	stimulus.write_to(outgoingFrames);
	outgoingFrames.endFrame();

	transmitter.transmit(0x00, outgoingFrames[0]);

	for(int i = 0; i < 100; i++) {
		source.schedule();
		receiver.schedule();
		transmitter.schedule();
		sink.schedule();
	}

	Stimulus result;
	uint8_t i = 0;
	result.read_from(handler.payload, i);

	BOOST_CHECK( stimulus.pin == result.pin );
	BOOST_CHECK( stimulus.newState == result.newState );
}
