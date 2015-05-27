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

class Tty : public Hardware {
	SerialPort serial;

public:
	Tty(const char*);
	virtual ~Tty();

	virtual bool getReady();
	virtual bool putReady();
	virtual uint8_t get();
	virtual void put(uint8_t);
};

#endif /* TTY_H_ */
