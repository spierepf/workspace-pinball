/*
 * Uart.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef UART_H_
#define UART_H_

#include <Hardware.h>

class Uart : public Hardware {
public:
	Uart();
	virtual ~Uart();

	virtual bool get_ready();
	virtual bool put_ready();
	virtual uint8_t get();
	virtual void put(uint8_t);
};

#endif /* UART_H_ */
