/*
 * Tty.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef TTY_H_
#define TTY_H_

#include <SerialPort.h>

#include <Hardware.h>

namespace Pin {

class Tty : public Hardware {
	SerialPort serial;

public:
	Tty(const char*);
	virtual ~Tty();

	virtual bool get_ready();
	virtual bool put_ready();
	virtual uint8_t get();
	virtual void put(uint8_t);
};

} /* namespace Pin */

#endif /* TTY_H_ */
