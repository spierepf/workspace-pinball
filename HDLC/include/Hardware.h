/*
 * Hardware.h
 *
 *  Created on: Jan 29, 2015
 *      Author: peter
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdint.h>

class Hardware {
public:
	Hardware();
	virtual ~Hardware();

	virtual bool get_ready() = 0;
	virtual bool put_ready() = 0;
	virtual uint8_t get() = 0;
	virtual void put(uint8_t) = 0;
};

#endif /* HARDWARE_H_ */
