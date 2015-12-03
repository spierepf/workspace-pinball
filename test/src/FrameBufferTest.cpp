/*
 * FrameBufferTest.cpp
 *
 *  Created on: Sep 2, 2015
 *      Author: peter
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <FrameBuffer.h>

BOOST_AUTO_TEST_CASE( constructed_framebuffer_has_no_frame ) {
	hdlc::FrameBuffer framebuffer;

	BOOST_CHECK(!(framebuffer.size() > 0));
	BOOST_CHECK(framebuffer.size() == 0);
}

BOOST_AUTO_TEST_CASE( framebuffer_with_ended_frame_has_one_frame ) {
	hdlc::FrameBuffer framebuffer;

	framebuffer.endFrame();
	BOOST_CHECK(framebuffer.size() > 0);
	BOOST_CHECK(framebuffer.size() == 1);
}

BOOST_AUTO_TEST_CASE( framebuffer_with_removed_frame_has_no_frame ) {
	hdlc::FrameBuffer framebuffer;

	framebuffer.endFrame();
	framebuffer.removeFrame();
	BOOST_CHECK(!(framebuffer.size() > 0));
	BOOST_CHECK(framebuffer.size() == 0);
}

BOOST_AUTO_TEST_CASE( framebuffer_with_two_ended_and_one_removed_frame_has_one_frame ) {
	hdlc::FrameBuffer framebuffer;

	framebuffer.endFrame();
	framebuffer.endFrame();
	framebuffer.removeFrame();
	BOOST_CHECK(framebuffer.size() > 0);
	BOOST_CHECK(framebuffer.size() == 1);
}

BOOST_AUTO_TEST_CASE( framebuffer_frame_length ) {
	hdlc::FrameBuffer framebuffer;

	framebuffer.endFrame();
	framebuffer.put(0);
	framebuffer.endFrame();

	BOOST_CHECK(framebuffer[0].size() == 0);
	BOOST_CHECK(framebuffer[1].size() == 1);
}

BOOST_AUTO_TEST_CASE( framebuffer_frame_data ) {
	hdlc::FrameBuffer framebuffer;

	framebuffer.put(31);
	framebuffer.endFrame();
	framebuffer.put(42);
	framebuffer.put(57);
	framebuffer.endFrame();

	BOOST_CHECK(framebuffer[0][0] == 31);
	BOOST_CHECK(framebuffer[1][0] == 42);
	BOOST_CHECK(framebuffer[1][1] == 57);
}

BOOST_AUTO_TEST_CASE( framebuffer_put_wraparound ) {
	hdlc::FrameBuffer framebuffer;

	framebuffer.put(0);
	framebuffer.put(1);
	framebuffer.put(2);
	framebuffer.put(3);
	framebuffer.put(4);
	framebuffer.put(5);
	framebuffer.put(6);
	framebuffer.endFrame();
	framebuffer.removeFrame();
	framebuffer.put(7);
	framebuffer.put(8);
	framebuffer.put(9);
	framebuffer.put(10);
	framebuffer.put(11);
	framebuffer.put(12);
	framebuffer.put(13);
	framebuffer.endFrame();

	BOOST_CHECK(framebuffer[0].size() == 7);
	BOOST_CHECK(framebuffer[0][0] == 7);
	BOOST_CHECK(framebuffer[0][1] == 8);
	BOOST_CHECK(framebuffer[0][2] == 9);
	BOOST_CHECK(framebuffer[0][3] == 10);
	BOOST_CHECK(framebuffer[0][4] == 11);
	BOOST_CHECK(framebuffer[0][5] == 12);
	BOOST_CHECK(framebuffer[0][6] == 13);
}

BOOST_AUTO_TEST_CASE( framebuffer_revert_incomplete_frame ) {
	hdlc::FrameBuffer framebuffer;

	framebuffer.put(0);
	framebuffer.endFrame();
	framebuffer.put(1);
	framebuffer.put(2);
	framebuffer.revertIncompleteFrame();
	framebuffer.put(3);
	framebuffer.endFrame();

	BOOST_CHECK(framebuffer[0].size() == 1);
	BOOST_CHECK(framebuffer[0][0] == 0);
	BOOST_CHECK(framebuffer[1].size() == 1);
	BOOST_CHECK(framebuffer[1][0] == 3);
}
