/*
 * Tty.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef AVR

#include <SerialPort.h>
#include <errno.h>

#include "Tty.h"

Tty::Tty(const char* device) : serial(device) {
	serial.Open(SerialPort::BAUD_9600);
}

Tty::~Tty() {
	// TODO Auto-generated destructor stub
}

bool Tty::get_ready() {
	return serial.IsDataAvailable();
}

bool Tty::put_ready() {
	return true;
}

uint8_t Tty::get() {
	return serial.ReadByte();
}

void Tty::put(uint8_t b) {
	bool retry = true;

	do {
		try {
			serial.WriteByte(b);
			retry = false;
		} catch (std::runtime_error& e) {
			if(errno != EINTR) throw e;
		}
	} while(retry);
}

#endif
