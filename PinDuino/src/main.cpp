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
#include <SolenoidBank.h>

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

FrameBuffer<64, 4> incomingFrames;
FrameBuffer<64, 4> outgoingFrames;
IncomingDataLink incomingDataLink(hardware, incomingFrames);
OutgoingPinDuinoDataLink outgoingDataLink(hardware, outgoingFrames);

PinBank pinBankB0(PinBank::B, _BV(0));
PinBank pinBankB1(PinBank::B, _BV(1));
PinBank pinBankB2(PinBank::B, _BV(2));
PinBank pinBankB3(PinBank::B, _BV(3));
PinBank pinBankB4(PinBank::B, _BV(4));
PinBank pinBankB5(PinBank::B, _BV(5));

PingPong pingPong(outgoingDataLink);

uint8_t dirtyListB = 0;

Item *solenoids[] = {
		new Solenoid(pinBankB0, TCCR0A, 0, 0, OCR0A, _BV(0), dirtyListB),
		new Solenoid(pinBankB1, TCCR1A, _BV(COM1A1) | _BV(COM1A0), _BV(COM1A1), OCR1AL, _BV(1), dirtyListB),
		new Solenoid(pinBankB2, TCCR1A, _BV(COM1B1) | _BV(COM1B0), _BV(COM1B1), OCR1BL, _BV(2), dirtyListB),
		new Solenoid(pinBankB3, TCCR2A, _BV(COM2A1) | _BV(COM2A0), _BV(COM2A1), OCR2A, _BV(3), dirtyListB),
		new Solenoid(pinBankB4, TCCR0A, 0, 0, OCR0A, _BV(4), dirtyListB),
		new Solenoid(pinBankB5, TCCR0A, 0, 0, OCR0A, _BV(5), dirtyListB),
		NULL,
		NULL
};

SolenoidBank solenoidBankB(solenoids, dirtyListB);

StimulusResponse stimulusResponse;

EndPoint endPoint(incomingDataLink, outgoingDataLink, incomingFrames, outgoingFrames, pingPong);
Blinker blinker(outgoingDataLink);

uint8_t dirtyListC = 0b00111111;
uint8_t dirtyListD = 0b11111100;

PinBank pinBankC(PinBank::C, dirtyListC);
PinBank pinBankD(PinBank::D, dirtyListD);

Item *switchesC[] = {
		new Switch(outgoingDataLink, 0, 0, dirtyListC),
		new Switch(outgoingDataLink, 1, 1, dirtyListC),
		new Switch(outgoingDataLink, 2, 2, dirtyListC),
		new Switch(outgoingDataLink, 3, 3, dirtyListC),
		new Switch(outgoingDataLink, 4, 4, dirtyListC),
		new Switch(outgoingDataLink, 5, 5, dirtyListC),
		NULL,
		NULL
};

Item *switchesD[] = {
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
	solenoidBankB.schedule();
	wdt_reset();
}
