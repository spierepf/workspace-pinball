/*
 * ByteSource.h
 *
 *  Created on: May 20, 2015
 *      Author: peter
 */

#ifndef INCLUDE_BYTESOURCE_H_
#define INCLUDE_BYTESOURCE_H_

#include <stdint.h>

class ByteSource {
public:
	ByteSource();
	virtual ~ByteSource();

	/** Should return true if a subsequent get() will return without blocking. */
	virtual bool getReady() = 0;

	/** Returns the next byte available from this source. */
	virtual uint8_t get() = 0;
};



#endif /* INCLUDE_BYTESOURCE_H_ */
