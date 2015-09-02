/*
 * RingBufferTest.cpp
 *
 *  Created on: Sep 1, 2015
 *      Author: peter
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <RingBuffer.h>

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



