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
	FrameBuffer<64,4> incomingFrames; IncomingDataLink datalink(hardware, incomingFrames);

	hardware.incoming_bytes.put(0);

	datalink.schedule();

	BOOST_CHECK( 0 == incomingFrames[0][0] );
}

BOOST_AUTO_TEST_CASE( datalink_get_escape ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	FrameBuffer<64,4> incomingFrames; IncomingDataLink datalink(hardware, incomingFrames);

	hardware.incoming_bytes.put(0x7d);
	hardware.incoming_bytes.put(0x11 ^ 0x20);

	datalink.schedule();

	BOOST_CHECK( 0x11 == incomingFrames[0][0] );
}

BOOST_AUTO_TEST_CASE( datalink_have_incoming_frame ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	FrameBuffer<64,4> incomingFrames; IncomingDataLink datalink(hardware, incomingFrames);

	BOOST_CHECK( !incomingFrames.hasFrame() );

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

	BOOST_CHECK( incomingFrames.hasFrame() );
}

BOOST_AUTO_TEST_CASE( datalink_incoming_frame_length ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	FrameBuffer<64,4> incomingFrames; IncomingDataLink datalink(hardware, incomingFrames);

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

	BOOST_CHECK( 1 == incomingFrames[0].getLength() );
}

BOOST_AUTO_TEST_CASE( datalink_next_incoming_frame ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	FrameBuffer<64,4> incomingFrames; IncomingDataLink datalink(hardware, incomingFrames);

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

	incomingFrames.removeFrame();

	BOOST_CHECK( incomingFrames.hasFrame() );
	BOOST_CHECK( 0x01 == incomingFrames[0][0] );
}

BOOST_AUTO_TEST_CASE( datalink_corrupt_incoming_frame ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	FrameBuffer<64,4> incomingFrames; IncomingDataLink datalink(hardware, incomingFrames);

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

	BOOST_CHECK( incomingFrames.hasFrame() );
	BOOST_CHECK( 0x01 == incomingFrames[0][0] );
	incomingFrames.removeFrame();
	BOOST_CHECK( !incomingFrames.hasFrame() );
}

BOOST_AUTO_TEST_CASE( datalink_outgoing_frame ) {
	RingBuffer<16> incoming_bytes;
	RingBuffer<16> outgoing_bytes;
	MockHardware hardware(incoming_bytes, outgoing_bytes);
	FrameBuffer<64, 4> outgoingFrames; OutgoingDataLink datalink(hardware, outgoingFrames);

	outgoingFrames.put(0x00);
	outgoingFrames.put(0x01);
	outgoingFrames.endFrame();

	outgoingFrames.put(0x02);
	outgoingFrames.put(0x03);
	outgoingFrames.endFrame();

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
	FrameBuffer<64, 4> outgoingFrames; OutgoingDataLink datalink(hardware, outgoingFrames);

	outgoingFrames.put(0x11);
	outgoingFrames.put(0x13);
	outgoingFrames.put(0x7d);
	outgoingFrames.put(0x7e);
	outgoingFrames.endFrame();

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
	FrameBuffer<64, 4> outgoingFrames; OutgoingDataLink datalink(hardware, outgoingFrames);

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

	FrameBuffer<64, 4> outgoingFrames;
	OutgoingDataLink datalink_a(hardware_a, outgoingFrames);
	FrameBuffer<64, 4> incomingFrames;
	IncomingDataLink datalink_b(hardware_b, incomingFrames);

	outgoingFrames.put(0x00);
	outgoingFrames.put(0x01);
	outgoingFrames.endFrame();

	for(int i = 0; i < 100; i++) {
		datalink_a.schedule();
		datalink_b.schedule();
	}

	BOOST_CHECK(incomingFrames.hasFrame());
	BOOST_CHECK(2 == incomingFrames[0].getLength());
	BOOST_CHECK(0x00 == incomingFrames[0][0]);
	BOOST_CHECK(0x01 == incomingFrames[0][1]);
	incomingFrames.removeFrame();
	BOOST_CHECK(!incomingFrames.hasFrame());
}
