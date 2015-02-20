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
#include <InputPin.h>

#ifndef BAUD
#define BAUD 115200
#endif

#include <util/setbaud.h>
#include <avr/wdt.h>

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
PingPong pingPong(datalink);
Application application(datalink, pingPong);
Blinker blinker;

PinBank pinBankC(PinBank::C, 0b00111111);
PinBank pinBankD(PinBank::D, 0b11111100);

InputPin inputPins[12] = {
		InputPin(application, pinBankC, 0, 0),
		InputPin(application, pinBankC, 1, 1),
		InputPin(application, pinBankC, 2, 2),
		InputPin(application, pinBankC, 3, 3),
		InputPin(application, pinBankC, 4, 4),
		InputPin(application, pinBankC, 5, 5),

		InputPin(application, pinBankD, 2, 6),
		InputPin(application, pinBankD, 3, 7),
		InputPin(application, pinBankD, 4, 8),
		InputPin(application, pinBankD, 5, 9),
		InputPin(application, pinBankD, 6, 10),
		InputPin(application, pinBankD, 7, 11)
};

void setup() {
	uart_init();
	pinMode(13, OUTPUT);
	wdt_enable(WDTO_15MS);
	pinBankC.dataLow();
	pinBankC.dirIn();
	pinBankC.dataHigh();
	pinBankD.dataLow();
	pinBankD.dirIn();
	pinBankD.dataHigh();
}

void loop() {
	datalink.schedule();
	application.schedule();
	pingPong.schedule();
	blinker.schedule();
	for(size_t i = 0; i < sizeof(inputPins) / sizeof(InputPin); i++) {
		inputPins[i].schedule();
	}
	wdt_reset();
}
