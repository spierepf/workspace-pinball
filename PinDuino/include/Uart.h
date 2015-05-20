/*
 * Uart.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef UART_H_
#define UART_H_

#include <ByteSource.h>
#include <ByteSink.h>

class Uart : public ByteSource, public ByteSink {
public:
	Uart();
	virtual ~Uart();

	virtual bool getReady();
	virtual bool putReady();
	virtual uint8_t get();
	virtual void put(uint8_t);
};

#endif /* UART_H_ */
