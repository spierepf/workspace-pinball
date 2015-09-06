/*
 * SolenoidActionTest.cpp
 *
 *  Created on: Sep 1, 2015
 *      Author: peter
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <RingBuffer.h>
#include <MockHardware.h>
#include <OutgoingDataLink.h>
#include <IncomingDataLink.h>

#include <SolenoidAction.h>

BOOST_AUTO_TEST_CASE(solenoid_action_test)
{
	SolenoidAction s(true, 2, 3, 4);
	uint8_t *b = (uint8_t*)&s;

	BOOST_CHECK_EQUAL((2 << 1) | 1, b[0]);
	BOOST_CHECK_EQUAL(4, b[1]);
	BOOST_CHECK_EQUAL(3, b[2]);
	BOOST_CHECK_EQUAL(0, b[3]);
}

BOOST_AUTO_TEST_CASE( read_write_solenoid_action_through_datalink ) {
	BOOST_CHECK( 4 == sizeof(SolenoidAction) );
	SolenoidAction action(true, 2, 3, 4);

	RingBuffer<16> a_to_b;
	RingBuffer<16> b_to_a;

	MockHardware hardware_a(b_to_a, a_to_b);
	MockHardware hardware_b(a_to_b, b_to_a);

	FrameBuffer<64, 4> outgoingFrames;
	OutgoingDataLink datalink_a(hardware_a, outgoingFrames);
	FrameBuffer<64, 4> incomingFrames;
	IncomingDataLink datalink_b(hardware_b, incomingFrames);

	datalink_a.begin_outgoing_frame(0x00);
	action.write_to(datalink_a);
	datalink_a.end_outgoing_frame();

	for(int i = 0; i < 100; i++) {
		datalink_a.schedule();
		datalink_b.schedule();
	}

	SolenoidAction result;
	uint8_t i = 1;
	result.read_from(incomingFrames[0], i);

	BOOST_CHECK( action.enabled == result.enabled );
	BOOST_CHECK( action.solenoidIndex == result.solenoidIndex );
	BOOST_CHECK( action.attack == result.attack );
	BOOST_CHECK( action.sustain == result.sustain );
}
