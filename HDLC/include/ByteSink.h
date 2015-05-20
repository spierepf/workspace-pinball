/*
 * ByteSink.h
 *
 *  Created on: May 20, 2015
 *      Author: peter
 */

#ifndef INCLUDE_BYTESINK_H_
#define INCLUDE_BYTESINK_H_

#include <stdint.h>

class ByteSink {
public:
	ByteSink();
	virtual ~ByteSink();

	/** Should return true if a subsequent put() will return without blocking. */
	virtual bool putReady() = 0;

	/** Writes a byte to this sink. */
	virtual void put(uint8_t) = 0;
};



#endif /* INCLUDE_BYTESINK_H_ */
