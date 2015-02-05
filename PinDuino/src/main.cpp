/*
 * main.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <Arduino.h>

#include <Uart.h>
#include <DataLink.h>
#include <Application.h>
#include <Blinker.h>

#ifndef BAUD
#define BAUD 115200
#endif

#include <util/setbaud.h>

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

Uart hardware;
PinDuinoDataLink datalink(hardware);
Application application(datalink);
Blinker blinker;

void setup() {
	uart_init();
	pinMode(13, OUTPUT);
}

void loop() {
	application.schedule();
	blinker.schedule();
}
