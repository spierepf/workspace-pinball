/*
 * DatalinkTest.cpp
 *
 *  Created on: Sep 1, 2015
 *      Author: peter
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <MockHardware.h>
#include <crc.h>
#include <IncomingDataLink.h>
#include <OutgoingDataLink.h>

BOOST_AUTO_TEST_CASE( datalink_get ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	IncomingDataLink datalink(hardware);

	hardware.incoming_bytes.put(0);

	datalink.schedule();

	BOOST_CHECK( 0 == datalink.peek(0) );
}

BOOST_AUTO_TEST_CASE( datalink_get_escape ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	IncomingDataLink datalink(hardware);

	hardware.incoming_bytes.put(0x7d);
	hardware.incoming_bytes.put(0x11 ^ 0x20);

	datalink.schedule();

	BOOST_CHECK( 0x11 == datalink.peek(0) );
}

BOOST_AUTO_TEST_CASE( datalink_have_incoming_frame ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	IncomingDataLink datalink(hardware);

	BOOST_CHECK( !datalink.have_incoming_frame() );

	uint16_t crc = 0xFFFF;
	hardware.incoming_bytes.put(0x00);
	crc_ccitt_update(crc, 0x00);
	hardware.incoming_bytes.put(crc >> 8);
	hardware.incoming_bytes.put(crc & 0xFF);
	hardware.incoming_bytes.put(0x7e);

	datalink.schedule();
	datalink.schedule();
	datalink.schedule();
	datalink.schedule();

	BOOST_CHECK( datalink.have_incoming_frame() );
}

BOOST_AUTO_TEST_CASE( datalink_incoming_frame_length ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	IncomingDataLink datalink(hardware);

	uint16_t crc = 0xFFFF;
	hardware.incoming_bytes.put(0x00);
	crc_ccitt_update(crc, 0x00);
	hardware.incoming_bytes.put(crc >> 8);
	hardware.incoming_bytes.put(crc & 0xFF);
	hardware.incoming_bytes.put(0x7e);

	datalink.schedule();
	datalink.schedule();
	datalink.schedule();
	datalink.schedule();

	BOOST_CHECK( 1 == datalink.incoming_frame_length() );
}

BOOST_AUTO_TEST_CASE( datalink_next_incoming_frame ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	IncomingDataLink datalink(hardware);

	uint16_t crc = 0xFFFF;
	hardware.incoming_bytes.put(0x00);
	crc_ccitt_update(crc, 0x00);
	hardware.incoming_bytes.put(crc >> 8);
	hardware.incoming_bytes.put(crc & 0xFF);
	hardware.incoming_bytes.put(0x7e);

	crc = 0xFFFF;
	hardware.incoming_bytes.put(0x01);
	crc_ccitt_update(crc, 0x01);
	hardware.incoming_bytes.put(crc >> 8);
	hardware.incoming_bytes.put(crc & 0xFF);
	hardware.incoming_bytes.put(0x7e);

	datalink.schedule();
	datalink.schedule();
	datalink.schedule();
	datalink.schedule();
	datalink.schedule();
	datalink.schedule();
	datalink.schedule();
	datalink.schedule();

	datalink.next_incoming_frame();

	BOOST_CHECK( datalink.have_incoming_frame() );
	BOOST_CHECK( 0x01 == datalink.peek(0) );
}

BOOST_AUTO_TEST_CASE( datalink_corrupt_incoming_frame ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	IncomingDataLink datalink(hardware);

	uint16_t crc = 0xFFFF;
	hardware.incoming_bytes.put(0x00);
	crc_ccitt_update(crc, 0x00);
	hardware.incoming_bytes.put(crc >> 8);
	hardware.incoming_bytes.put((crc & 0xFF) ^ 0x01); // corruption!
	hardware.incoming_bytes.put(0x7e);

	crc = 0xFFFF;
	hardware.incoming_bytes.put(0x01);
	crc_ccitt_update(crc, 0x01);
	hardware.incoming_bytes.put(crc >> 8);
	hardware.incoming_bytes.put(crc & 0xFF);
	hardware.incoming_bytes.put(0x7e);

	datalink.schedule();
	datalink.schedule();
	datalink.schedule();
	datalink.schedule();
	datalink.schedule();
	datalink.schedule();
	datalink.schedule();
	datalink.schedule();

	BOOST_CHECK( datalink.have_incoming_frame() );
	BOOST_CHECK( 0x01 == datalink.peek(0) );
	datalink.next_incoming_frame();
	BOOST_CHECK( !datalink.have_incoming_frame() );
}

BOOST_AUTO_TEST_CASE( datalink_outgoing_frame ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	OutgoingDataLink datalink(hardware);

	datalink.begin_outgoing_frame(0x00);
	datalink.append_payload(0x01);
	datalink.end_outgoing_frame();

	datalink.begin_outgoing_frame(0x02);
	datalink.append_payload(0x03);
	datalink.end_outgoing_frame();

	for(int i = 0; i < 12; i++) datalink.schedule();

	uint16_t crc = 0xFFFF;
	BOOST_CHECK( 0x7e == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x00 == hardware.outgoing_bytes.get() );
	crc_ccitt_update(crc, 0x00);
	BOOST_CHECK( 0x01 == hardware.outgoing_bytes.get() );
	crc_ccitt_update(crc, 0x01);
	BOOST_CHECK( (crc >> 8) == hardware.outgoing_bytes.get() );
	BOOST_CHECK( (crc & 0xFF) == hardware.outgoing_bytes.get() );

	crc = 0xFFFF;
	BOOST_CHECK( 0x7e == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x02 == hardware.outgoing_bytes.get() );
	crc_ccitt_update(crc, 0x02);
	BOOST_CHECK( 0x03 == hardware.outgoing_bytes.get() );
	crc_ccitt_update(crc, 0x03);
	BOOST_CHECK( (crc >> 8) == hardware.outgoing_bytes.get() );
	BOOST_CHECK( (crc & 0xFF) == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x7e == hardware.outgoing_bytes.get() );
	BOOST_CHECK( hardware.outgoing_bytes.empty() );
}

BOOST_AUTO_TEST_CASE( datalink_outgoing_frame_escape ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	OutgoingDataLink datalink(hardware);

	datalink.begin_outgoing_frame(0x11);
	datalink.append_payload(0x13);
	datalink.append_payload(0x7d);
	datalink.append_payload(0x7e);
	datalink.end_outgoing_frame();

	for(int i = 0; i < 12; i++) datalink.schedule();

	BOOST_CHECK( 0x7e == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x7d == hardware.outgoing_bytes.get() );
	BOOST_CHECK( (0x11 ^ 0x20) == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x7d == hardware.outgoing_bytes.get() );
	BOOST_CHECK( (0x13 ^ 0x20) == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x7d == hardware.outgoing_bytes.get() );
	BOOST_CHECK( (0x7d ^ 0x20) == hardware.outgoing_bytes.get() );
	BOOST_CHECK( 0x7d == hardware.outgoing_bytes.get() );
	BOOST_CHECK( (0x7e ^ 0x20) == hardware.outgoing_bytes.get() );
	hardware.outgoing_bytes.get();
	hardware.outgoing_bytes.get();
	BOOST_CHECK( 0x7e == hardware.outgoing_bytes.get() );
	BOOST_CHECK( hardware.outgoing_bytes.empty() );
}

BOOST_AUTO_TEST_CASE( datalink_no_output ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	OutgoingDataLink datalink(hardware);

	datalink.schedule();

	BOOST_CHECK( hardware.outgoing_bytes.empty() );
}

BOOST_AUTO_TEST_CASE( datalink_ping ) {
	RingBuffer<16> a_to_b;
	RingBuffer<16> b_to_a;
	a_to_b.zero();
	b_to_a.zero();

	MockHardware hardware_a(b_to_a, a_to_b);
	MockHardware hardware_b(a_to_b, b_to_a);

	OutgoingDataLink datalink_a(hardware_a);
	IncomingDataLink datalink_b(hardware_b);

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
