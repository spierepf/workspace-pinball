/*
 * test.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: peter
 */

#define BOOST_TEST_MODULE HLDCTest
#include <boost/test/unit_test.hpp>

#include <RingBuffer.h>
#include <MockHardware.h>
#include <DataLink.h>
#include <Timer.h>

#include <SolenoidAction.h>
#include <Stimulus.h>
#include <StimulusActionMap.h>

BOOST_AUTO_TEST_CASE( ringbuffer_constructor ) {
	RingBuffer<2> ringbuffer;

	BOOST_CHECK( ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );
}

BOOST_AUTO_TEST_CASE( ringbuffer_put ) {
	RingBuffer<2> ringbuffer;

	ringbuffer.put(0);
	BOOST_CHECK( !ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );

	ringbuffer.put(1);
	BOOST_CHECK( !ringbuffer.empty() );
	BOOST_CHECK( ringbuffer.full() );
}

BOOST_AUTO_TEST_CASE( ringbuffer_get ) {
	RingBuffer<2> ringbuffer;

	ringbuffer.put(0);

	BOOST_CHECK(0 == ringbuffer.get() );
	BOOST_CHECK( ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );

	ringbuffer.put(1);
	ringbuffer.put(2);

	BOOST_CHECK(1 == ringbuffer.get() );
	BOOST_CHECK( !ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );

	BOOST_CHECK(2 == ringbuffer.get() );
	BOOST_CHECK( ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );
}

BOOST_AUTO_TEST_CASE( ringbuffer_peek ) {
	RingBuffer<2> ringbuffer;

	ringbuffer.put(0);

	BOOST_CHECK(0 == ringbuffer.peek(0) );

	ringbuffer.put(1);

	BOOST_CHECK(0 == ringbuffer.peek(0) );
	BOOST_CHECK(1 == ringbuffer.peek(1) );
	BOOST_CHECK( !ringbuffer.empty() );
	BOOST_CHECK( ringbuffer.full() );

	ringbuffer.get();
	ringbuffer.put(2);

	BOOST_CHECK(1 == ringbuffer.peek(0) );
	BOOST_CHECK(2 == ringbuffer.peek(1) );
	BOOST_CHECK( !ringbuffer.empty() );
	BOOST_CHECK( ringbuffer.full() );
}

BOOST_AUTO_TEST_CASE( ringbuffer_consume ) {
	RingBuffer<2> ringbuffer;

	ringbuffer.put(0);
	ringbuffer.consume(1);

	BOOST_CHECK( ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );

	ringbuffer.put(1);
	ringbuffer.put(2);
	ringbuffer.consume(1);

	BOOST_CHECK( !ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );

	ringbuffer.consume(1);

	BOOST_CHECK( ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );

	ringbuffer.put(3);
	ringbuffer.put(4);
	ringbuffer.consume(2);

	BOOST_CHECK( ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );
}

BOOST_AUTO_TEST_CASE( ringbuffer_4 ) {
	RingBuffer<4> ringbuffer;

	ringbuffer.put(0);

	BOOST_CHECK(0 == ringbuffer.get() );
	BOOST_CHECK( ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );

	ringbuffer.put(1);
	ringbuffer.put(2);
	ringbuffer.put(3);
	ringbuffer.put(4);

	BOOST_CHECK(1 == ringbuffer.get() );
	BOOST_CHECK( !ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );

	BOOST_CHECK(2 == ringbuffer.get() );
	BOOST_CHECK( !ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );

	BOOST_CHECK(3 == ringbuffer.get() );
	BOOST_CHECK( !ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );

	BOOST_CHECK(4 == ringbuffer.get() );
	BOOST_CHECK( ringbuffer.empty() );
	BOOST_CHECK( !ringbuffer.full() );
}

BOOST_AUTO_TEST_CASE( datalink_get ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	DataLink datalink(hardware);

	hardware.incoming_bytes.put(0);

	datalink.schedule();

	BOOST_CHECK( 0 == datalink.peek(0) );
}

BOOST_AUTO_TEST_CASE( datalink_get_escape ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	DataLink datalink(hardware);

	hardware.incoming_bytes.put(0x7d);
	hardware.incoming_bytes.put(0x11 ^ 0x20);

	datalink.schedule();

	BOOST_CHECK( 0x11 == datalink.peek(0) );
}

BOOST_AUTO_TEST_CASE( datalink_have_incoming_frame ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	DataLink datalink(hardware);

	BOOST_CHECK( !datalink.have_incoming_frame() );

	hardware.incoming_bytes.put(0x00);
	hardware.incoming_bytes.put(0x7e);

	datalink.schedule();
	datalink.schedule();

	BOOST_CHECK( datalink.have_incoming_frame() );
}

BOOST_AUTO_TEST_CASE( datalink_incoming_frame_length ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	DataLink datalink(hardware);

	hardware.incoming_bytes.put(0x00);
	hardware.incoming_bytes.put(0x7e);

	datalink.schedule();
	datalink.schedule();

	BOOST_CHECK( 1 == datalink.incoming_frame_length() );
}

BOOST_AUTO_TEST_CASE( datalink_next_incoming_frame ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	DataLink datalink(hardware);

	hardware.incoming_bytes.put(0x00);
	hardware.incoming_bytes.put(0x7e);

	hardware.incoming_bytes.put(0x01);
	hardware.incoming_bytes.put(0x7e);

	datalink.schedule();
	datalink.schedule();
	datalink.schedule();
	datalink.schedule();

	datalink.next_incoming_frame();

	BOOST_CHECK( datalink.have_incoming_frame() );
	BOOST_CHECK( 0x01 == datalink.peek(0) );
}

BOOST_AUTO_TEST_CASE( datalink_outgoing_frame ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	DataLink datalink(hardware);

	datalink.begin_outgoing_frame(0x00);
	datalink.append_payload(0x01);
	datalink.end_outgoing_frame();

	datalink.begin_outgoing_frame(0x02);
	datalink.append_payload(0x03);
	datalink.end_outgoing_frame();

	for(int i = 0; i < 7; i++) datalink.schedule();

	BOOST_CHECK( 0x7e == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x00 == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x01 == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x7e == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x02 == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x03 == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x7e == hardware.outgoing_bytes.get() );
	BOOST_CHECK( hardware.outgoing_bytes.empty() );
}

BOOST_AUTO_TEST_CASE( datalink_outgoing_frame_escape ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	DataLink datalink(hardware);

	datalink.begin_outgoing_frame(0x11);
	datalink.append_payload(0x13);
	datalink.append_payload(0x7d);
	datalink.append_payload(0x7e);
	datalink.end_outgoing_frame();

	for(int i = 0; i < 10; i++) datalink.schedule();

	BOOST_CHECK( 0x7e == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x7d == hardware.outgoing_bytes.get() );
	BOOST_CHECK( (0x11 ^ 0x20) == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x7d == hardware.outgoing_bytes.get() );
	BOOST_CHECK( (0x13 ^ 0x20) == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x7d == hardware.outgoing_bytes.get() );
	BOOST_CHECK( (0x7d ^ 0x20) == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x7d == hardware.outgoing_bytes.get() );
	BOOST_CHECK( (0x7e ^ 0x20) == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x7e == hardware.outgoing_bytes.get() );
	BOOST_CHECK( hardware.outgoing_bytes.empty() );
}

BOOST_AUTO_TEST_CASE( datalink_no_output ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	DataLink datalink(hardware);

	datalink.schedule();

	BOOST_CHECK( hardware.outgoing_bytes.empty() );
}

BOOST_AUTO_TEST_CASE( datalink_no_input ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	DataLink datalink(hardware);

	datalink.schedule();
	datalink.end_outgoing_frame();

	BOOST_CHECK( not datalink.have_incoming_frame() );
}

BOOST_AUTO_TEST_CASE( datalink_ping ) {
	RingBuffer<16> a_to_b;
	RingBuffer<16> b_to_a;

	MockHardware hardware_a(b_to_a, a_to_b);
	MockHardware hardware_b(a_to_b, b_to_a);

	DataLink datalink_a(hardware_a);
	DataLink datalink_b(hardware_b);

	datalink_a.begin_outgoing_frame(0x00);
	datalink_a.append_payload(0x01);
	datalink_a.end_outgoing_frame();

	for(int i = 0; i < 100; i++) {
		datalink_a.schedule();
		datalink_b.schedule();
	}

	BOOST_CHECK(datalink_b.have_incoming_frame());
	BOOST_CHECK(2 == datalink_b.incoming_frame_length());
	BOOST_CHECK(0x00 == datalink_b.peek(0));
	BOOST_CHECK(0x01 == datalink_b.peek(1));
	datalink_b.next_incoming_frame();
	BOOST_CHECK(!datalink_b.have_incoming_frame());
}

BOOST_AUTO_TEST_CASE( timer ) {
	Timer<uint8_t> t;

	t.set(0);

	BOOST_CHECK( 1 == t.elapsed(1) );
}

BOOST_AUTO_TEST_CASE( timer_wraparound ) {
	Timer<uint8_t> t;

	t.set(254);

	BOOST_CHECK( 1 == t.elapsed(255) );
	BOOST_CHECK( 2 == t.elapsed(0) );
	BOOST_CHECK( 3 == t.elapsed(1) );
}

BOOST_AUTO_TEST_CASE( read_write_solenoid_action_through_datalink ) {
	BOOST_CHECK( 4 == sizeof(SolenoidAction) );
	SolenoidAction action(true, 2, 3, 4);

	RingBuffer<16> a_to_b;
	RingBuffer<16> b_to_a;

	MockHardware hardware_a(b_to_a, a_to_b);
	MockHardware hardware_b(a_to_b, b_to_a);

	DataLink datalink_a(hardware_a);
	DataLink datalink_b(hardware_b);

	datalink_a.begin_outgoing_frame(0x00);
	action.write_to(datalink_a);
	datalink_a.end_outgoing_frame();

	for(int i = 0; i < 100; i++) {
		datalink_a.schedule();
		datalink_b.schedule();
	}

	SolenoidAction result;
	uint8_t i = 1;
	result.read_from(datalink_b, i);

	BOOST_CHECK( action.enabled == result.enabled );
	BOOST_CHECK( action.solenoidIndex == result.solenoidIndex );
	BOOST_CHECK( action.attack == result.attack );
	BOOST_CHECK( action.sustain == result.sustain );
}

BOOST_AUTO_TEST_CASE( read_write_stimulus_through_datalink ) {
	BOOST_CHECK( 1 == sizeof(Stimulus) );
	Stimulus stimulus(4, true);

	RingBuffer<16> a_to_b;
	RingBuffer<16> b_to_a;

	MockHardware hardware_a(b_to_a, a_to_b);
	MockHardware hardware_b(a_to_b, b_to_a);

	DataLink datalink_a(hardware_a);
	DataLink datalink_b(hardware_b);

	datalink_a.begin_outgoing_frame(0x00);
	stimulus.write_to(datalink_a);
	datalink_a.end_outgoing_frame();

	for(int i = 0; i < 100; i++) {
		datalink_a.schedule();
		datalink_b.schedule();
	}

	Stimulus result;
	uint8_t i = 1;
	result.read_from(datalink_b, i);

	BOOST_CHECK( stimulus.pin == result.pin );
	BOOST_CHECK( stimulus.newState == result.newState );
}

BOOST_AUTO_TEST_CASE( stimulus_action_map_test ) {
	SolenoidAction action(true, 2, 3, 4);
	Stimulus stimulus(4, true);

	StimulusActionMap map;

	map[stimulus] = action;

	SolenoidAction result = map[stimulus];

	BOOST_CHECK( action.enabled == result.enabled );
	BOOST_CHECK( action.solenoidIndex == result.solenoidIndex );
	BOOST_CHECK( action.attack == result.attack );
	BOOST_CHECK( action.sustain == result.sustain );
}
