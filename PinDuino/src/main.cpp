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
#include <Switch.h>
#include <StimulusResponse.h>
#include <SwitchBank.h>

#ifndef BAUD
#define BAUD 9600
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

uint8_t dirtyListC = 0;
uint8_t dirtyListD = 0;

PinBank pinBankC(PinBank::C, 0b00111111);
PinBank pinBankD(PinBank::D, 0b11111100);

Switch *switchesC[] = {
		new Switch(outgoingDataLink, 0, 0, dirtyListC),
		new Switch(outgoingDataLink, 1, 1, dirtyListC),
		new Switch(outgoingDataLink, 2, 2, dirtyListC),
		new Switch(outgoingDataLink, 3, 3, dirtyListC),
		new Switch(outgoingDataLink, 4, 4, dirtyListC),
		new Switch(outgoingDataLink, 5, 5, dirtyListC),
		NULL,
		NULL
};

Switch *switchesD[] = {
		NULL,
		NULL,
		new Switch(outgoingDataLink, 2, 6, dirtyListD),
		new Switch(outgoingDataLink, 3, 7, dirtyListD),
		new Switch(outgoingDataLink, 4, 8, dirtyListD),
		new Switch(outgoingDataLink, 5, 9, dirtyListD),
		new Switch(outgoingDataLink, 6, 10, dirtyListD),
		new Switch(outgoingDataLink, 7, 11, dirtyListD)
};

SwitchBank switchBankC = SwitchBank(switchesC, pinBankC, dirtyListC);
SwitchBank switchBankD = SwitchBank(switchesD, pinBankD, dirtyListD);

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
	while(sizeof(SolenoidAction)!=4);
	while(sizeof(Stimulus)!=1);
	endPoint.schedule();
	pingPong.schedule();
	blinker.schedule();
	switchBankC.schedule();
	switchBankD.schedule();
	for(size_t i = 0; i < sizeof(solenoids) / sizeof(Solenoid); i++) {
		solenoids[i].schedule();
	}
	wdt_reset();
}
