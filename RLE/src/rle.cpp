/*
 * rle.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: peter
 */

#include <rle.h>

#define MASK1(bit_position) (0x80 >> (bit_position))
#define MASK0(bit_position) ~MASK1(bit_position)

size_t rle_compress(uint8_t src[], size_t src_size, uint8_t buf[]) {
	size_t byte_position = 0;
	size_t bit_position = 0;
	uint8_t bit = 1;
	int count = 0;

	size_t retval = 0;

	while(byte_position < src_size)
	{
		if(
				((bit == 0) && (uint8_t)(src[byte_position] | MASK0(bit_position)) != 0xFF) ||
				((bit == 1) && (uint8_t)(src[byte_position] & MASK1(bit_position)) != 0x00)
			) {
			count++;
		} else {
			buf[retval++] = count;
			count = 1;
			bit = bit == 0 ? 1 : 0;
		}

		if(count == 256) {
			buf[retval++] = count = 0;
		}

		if(++bit_position == 8) {
			bit_position = 0;
			byte_position++;
		}
	}
	buf[retval++] = count;

	return retval;
}

size_t rle_decompress(uint8_t buf[], size_t buf_size, uint8_t dst[]) {
	size_t byte_position = 0;
	size_t bit_position = 0;
	uint8_t bit = 1;

	for(size_t i = 0; i < buf_size; i++) {
		for(int count = (buf[i] == 0 ? 256 : buf[i]); count >= 1 ; --count) {
			dst[byte_position] = bit == 1 ? dst[byte_position] | MASK1(bit_position) : dst[byte_position] & MASK0(bit_position);

			if(++bit_position == 8) {
				bit_position = 0;
				byte_position++;
			}
		}
		if(buf[i]) bit = bit == 0 ? 1 : 0;
	}
	return byte_position;
}
