/*
 * Hardware.h
 *
 *  Created on: Jan 29, 2015
 *      Author: peter
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdint.h>

/** Serial stream abstraction class.
 *
 * Implementations of this abstract class are used by the DataLink class to communicate
 * with hardware.
 */
class Hardware {
public:
	Hardware();
	virtual ~Hardware();

	/** Should return true if a subsequent get() will return without blocking. */
	virtual bool get_ready() = 0;

	/** Should return true if a subsequent put() will return without blocking. */
	virtual bool put_ready() = 0;

	/** Returns the next byte received by the serial port. */
	virtual uint8_t get() = 0;

	/** Writes a byte to the serial port. */
	virtual void put(uint8_t) = 0;
};

#endif /* HARDWARE_H_ */
