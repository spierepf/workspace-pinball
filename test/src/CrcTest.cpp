/*
 * CrcTest.cpp
 *
 *  Created on: Sep 1, 2015
 *      Author: peter
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <crc.h>

BOOST_AUTO_TEST_CASE( crc_ccitt_update_test ) {
	uint16_t crc = 0xFFFF;

	crc_ccitt_update(crc, 0x00);
	BOOST_CHECK(0xE1F0 == crc);

	crc_ccitt_update(crc, 0x01);
	BOOST_CHECK(0x0D2E == crc);

	crc_ccitt_update(crc, 0x02);
	BOOST_CHECK(0xDFEF == crc);

	crc_ccitt_update(crc, 0xDF);
	crc_ccitt_update(crc, 0xEF);
	BOOST_CHECK(0x0000 == crc);
}
