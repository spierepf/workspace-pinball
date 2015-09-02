/*
 * StimulusTest.cpp
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

#include <Stimulus.h>

BOOST_AUTO_TEST_CASE(stimulus_test)
{
	Stimulus s(2, true);
	uint8_t *b = (uint8_t*)&s;

	BOOST_CHECK_EQUAL(128+2, *b);
}

BOOST_AUTO_TEST_CASE( read_write_stimulus_through_datalink ) {
	BOOST_CHECK( 1 == sizeof(Stimulus) );
	Stimulus stimulus(4, true);

	RingBuffer<16> a_to_b;
	RingBuffer<16> b_to_a;

	MockHardware hardware_a(b_to_a, a_to_b);
	MockHardware hardware_b(a_to_b, b_to_a);

	OutgoingDataLink datalink_a(hardware_a);
	IncomingDataLink datalink_b(hardware_b);

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
