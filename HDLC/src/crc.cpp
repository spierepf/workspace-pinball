/*
 * crc.cpp
 *
 *  Created on: Aug 30, 2015
 *      Author: peter
 */

#include "crc.h"

#ifdef AVR

#include <util/crc16.h>

void crc_ccitt_update (uint16_t& crc, uint8_t data) {
	crc = _crc_ccitt_update(crc, data);
}

#else
#define lo8(x) ((x)&0xff)
#define hi8(x) ((x)>>8)

void crc_ccitt_update (uint16_t& crc, uint8_t data) {
	unsigned crc_new = (unsigned char)(crc >> 8) | (crc << 8);
	crc_new ^= data;
	crc_new ^= (unsigned char)(crc_new & 0xff) >> 4;
	crc_new ^= crc_new << 12;
	crc_new ^= (crc_new & 0xff) << 5;
	crc = crc_new;
}

#endif
