/*
 * TimerTest.cpp
 *
 *  Created on: Sep 1, 2015
 *      Author: peter
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <stdint.h>

#include <Timer.h>

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
