/*
 * RleDecoder.h
 *
 *  Created on: Apr 14, 2015
 *      Author: peter
 */

#ifndef RLEDECODER_H_
#define RLEDECODER_H_

#include <stdint.h>
#include <stdlib.h>

class RleDecoder {
private:
	uint8_t* dst;

	size_t byte_position = 0;
	size_t bit_position = 0;
	uint8_t bit = 1;

public:
	RleDecoder(uint8_t*);
	virtual ~RleDecoder();

	void write(const uint8_t);
	size_t length();
};

#endif /* RLEDECODER_H_ */
