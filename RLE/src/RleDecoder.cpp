/*
 * RleDecoder.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: peter
 */

#include "RleDecoder.h"

#define MASK1(bit_position) (0x80 >> (bit_position))
#define MASK0(bit_position) ~MASK1(bit_position)

RleDecoder::RleDecoder(uint8_t *dst) : dst(dst) {
	// TODO Auto-generated constructor stub

}

RleDecoder::~RleDecoder() {
	// TODO Auto-generated destructor stub
}

void RleDecoder::write(const uint8_t data) {
	for(int count = (data == 0 ? 256 : data); count >= 1 ; --count) {
		dst[byte_position] = bit == 1 ? dst[byte_position] | MASK1(bit_position) : dst[byte_position] & MASK0(bit_position);

		if(++bit_position == 8) {
			bit_position = 0;
			byte_position++;
		}
	}
	if(data) bit = bit == 0 ? 1 : 0;
}

size_t RleDecoder::length() {
	return byte_position;
}
