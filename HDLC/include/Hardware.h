/*
 * Hardware.h
 *
 *  Created on: May 27, 2015
 *      Author: peter
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdint.h>

class Hardware {
public:
	Hardware();
	virtual ~Hardware();

	/** Should return true if a subsequent get() will return without blocking. */
	virtual bool getReady() = 0;

	/** Returns the next byte available from this source. */
	virtual uint8_t get() = 0;

	/** Should return true if a subsequent put() will return without blocking. */
	virtual bool putReady() = 0;

	/** Writes a byte to this sink. */
	virtual void put(uint8_t) = 0;
};

#endif /* HARDWARE_H_ */
