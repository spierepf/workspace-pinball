/*
 * main.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <Arduino.h>

#include <Uart.h>
#include <IncomingDataLink.h>
#include <OutgoingDataLink.h>
#include <Solenoid.h>
#include <Blinker.h>
#include <EndPoint.h>
#include <InputPin.h>
#include <StimulusResponse.h>

#ifndef BAUD
#define BAUD 115200
#endif

#include <util/setbaud.h>
#include <avr/wdt.h>
#include <eeprom.h>

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

IncomingDataLink incomingDataLink(hardware);
OutgoingPinDuinoDataLink outgoingDataLink(hardware);

PinBank pinBankB0(PinBank::B, _BV(0));
PinBank pinBankB1(PinBank::B, _BV(1));
PinBank pinBankB2(PinBank::B, _BV(2));
PinBank pinBankB3(PinBank::B, _BV(3));
PinBank pinBankB4(PinBank::B, _BV(4));
PinBank pinBankB5(PinBank::B, _BV(5));

PingPong pingPong(outgoingDataLink);

Solenoid solenoids[6] = {
		Solenoid(pinBankB0, TCCR0A, 0, 0, OCR0A),
		Solenoid(pinBankB1, TCCR1A, _BV(COM1A1) | _BV(COM1A0), _BV(COM1A1), OCR1AL),
		Solenoid(pinBankB2, TCCR1A, _BV(COM1B1) | _BV(COM1B0), _BV(COM1B1), OCR1BL),
		Solenoid(pinBankB3, TCCR2A, _BV(COM2A1) | _BV(COM2A0), _BV(COM2A1), OCR2A),
		Solenoid(pinBankB4, TCCR0A, 0, 0, OCR0A),
		Solenoid(pinBankB5, TCCR0A, 0, 0, OCR0A)
};

StimulusResponse stimulusResponse;

EndPoint endPoint(incomingDataLink, outgoingDataLink, pingPong);
Blinker blinker;

PinBank pinBankC(PinBank::C, 0b00111111);
PinBank pinBankD(PinBank::D, 0b11111100);

InputPin inputPins[12] = {
		InputPin(outgoingDataLink, pinBankC, 0, 0),
		InputPin(outgoingDataLink, pinBankC, 1, 1),
		InputPin(outgoingDataLink, pinBankC, 2, 2),
		InputPin(outgoingDataLink, pinBankC, 3, 3),
		InputPin(outgoingDataLink, pinBankC, 4, 4),
		InputPin(outgoingDataLink, pinBankC, 5, 5),

		InputPin(outgoingDataLink, pinBankD, 2, 6),
		InputPin(outgoingDataLink, pinBankD, 3, 7),
		InputPin(outgoingDataLink, pinBankD, 4, 8),
		InputPin(outgoingDataLink, pinBankD, 5, 9),
		InputPin(outgoingDataLink, pinBankD, 6, 10),
		InputPin(outgoingDataLink, pinBankD, 7, 11)
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

	for(int i = 0; i < 12; i++) {
		eeprom_busy_wait();
		stimulusResponse[Stimulus(i, false)] = eeprom_read_dword(&eeprom_actions[i][0]);
		eeprom_busy_wait();
		stimulusResponse[Stimulus(i, true)] = eeprom_read_dword(&eeprom_actions[i][1]);
	}
}

void loop() {
	while(sizeof(SolenoidAction)!=4);
	while(sizeof(Stimulus)!=1);
	endPoint.schedule();
	pingPong.schedule();
	blinker.schedule();
	for(size_t i = 0; i < sizeof(inputPins) / sizeof(InputPin); i++) {
		inputPins[i].schedule();
	}
	for(size_t i = 0; i < sizeof(solenoids) / sizeof(Solenoid); i++) {
		solenoids[i].schedule();
	}
	wdt_reset();
}
