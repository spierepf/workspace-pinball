/*
 * Uart.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <avr/io.h>

#include <Uart.h>

Uart::Uart() {
	// TODO Auto-generated constructor stub

}

Uart::~Uart() {
	// TODO Auto-generated destructor stub
}

bool Uart::get_ready() {
	return bit_is_set(UCSR0A, RXC0);
}

bool Uart::put_ready() {
	return bit_is_set(UCSR0A, UDRE0);
}

uint8_t Uart::get() {
	return UDR0;
}

void Uart::put(uint8_t b) {
	UDR0 = b;
}
