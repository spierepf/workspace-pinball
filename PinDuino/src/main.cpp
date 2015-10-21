#include <Arduino.h>

#include <Source.h>
#include <Sink.h>
#include <EscapingSource.h>
#include <EscapingSink.h>
#include <FrameReceiver.h>
#include <FrameTransmitter.h>
#include <EndPoint.h>
#include <FrameBufferUserFrameHandler.h>
#include <PingPong.h>
#include <Blinker.h>
#include <SolenoidBank.h>
#include <SolenoidAction.h>
#include <SwitchBank.h>
#include <Stimulus.h>
#include <StimulusResponse.h>
#include <OpCode.h>
#include <eeprom.h>
#include <PinDuinoFrameHandler.h>

#ifndef BAUD
#define BAUD 9600
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

class UartSource : public hdlc::Source<uint8_t> {
public:
	bool isReady() { return bit_is_set(UCSR0A, RXC0); }
	uint8_t read() { return UDR0; }
};

class UartSink : public hdlc::Sink<uint8_t> {
public:
	bool isReady() { return bit_is_set(UCSR0A, UDRE0); }
	void write(const uint8_t& octet) { UDR0 = octet; }
};

UartSource ttySource;
UartSink ttySink;

hdlc::EscapingSource source(ttySource);
hdlc::EscapingSink sink(ttySink);

hdlc::FrameBuffer outgoingFrameBuffer;

hdlc::FrameReceiver receiver(source);
hdlc::FrameTransmitter transmitter(sink);

PinDuinoFrameHandler frameHandler;
hdlc::EndPoint endPoint(source, receiver, frameHandler, outgoingFrameBuffer, transmitter, sink);

PingPong pingPong;
Blinker blinker;

/******************************************************************************************************/

uint8_t dirtyListB = 0;

PinBank pinBankB0(PinBank::B, _BV(0));
PinBank pinBankB1(PinBank::B, _BV(1));
PinBank pinBankB2(PinBank::B, _BV(2));
PinBank pinBankB3(PinBank::B, _BV(3));
PinBank pinBankB4(PinBank::B, _BV(4));
PinBank led(PinBank::B, _BV(5));

Solenoid *solenoids[] = {
		new Solenoid(pinBankB0, TCCR0A, 0, 0, OCR0A, _BV(0), dirtyListB),
		new Solenoid(pinBankB1, TCCR1A, _BV(COM1A1) | _BV(COM1A0), _BV(COM1A1), OCR1AL, _BV(1), dirtyListB),
		new Solenoid(pinBankB2, TCCR1A, _BV(COM1B1) | _BV(COM1B0), _BV(COM1B1), OCR1BL, _BV(2), dirtyListB),
		new Solenoid(pinBankB3, TCCR2A, _BV(COM2A1) | _BV(COM2A0), _BV(COM2A1), OCR2A, _BV(3), dirtyListB),
		new Solenoid(pinBankB4, TCCR0A, 0, 0, OCR0A, _BV(4), dirtyListB),
		new Solenoid(led, TCCR0A, 0, 0, OCR0A, _BV(5), dirtyListB),
		NULL,
		NULL
};

SolenoidBank solenoidBankB(solenoids, dirtyListB);

/******************************************************************************************************/

uint8_t dirtyListC = 0b00111111;
uint8_t dirtyListD = 0b11111100;

PinBank pinBankC(PinBank::C, dirtyListC);
PinBank pinBankD(PinBank::D, dirtyListD);

Item *switchesC[] = {
		new Switch(0, 0, dirtyListC),
		new Switch(1, 1, dirtyListC),
		new Switch(2, 2, dirtyListC),
		new Switch(3, 3, dirtyListC),
		new Switch(4, 4, dirtyListC),
		new Switch(5, 5, dirtyListC),
		NULL,
		NULL
};

Item *switchesD[] = {
		NULL,
		NULL,
		new Switch(2, 6, dirtyListD),
		new Switch(3, 7, dirtyListD),
		new Switch(4, 8, dirtyListD),
		new Switch(5, 9, dirtyListD),
		new Switch(6, 10, dirtyListD),
		new Switch(7, 11, dirtyListD)
};

SwitchBank switchBankC = SwitchBank(switchesC, pinBankC, dirtyListC);
SwitchBank switchBankD = SwitchBank(switchesD, pinBankD, dirtyListD);

/******************************************************************************************************/

StimulusResponse stimulusResponse;

/******************************************************************************************************/

void setup() {
	pinBankC.dataLow();
	pinBankC.dirIn();
	pinBankC.dataHigh();
	pinBankD.dataLow();
	pinBankD.dirIn();
	pinBankD.dataHigh();

	receiver.setFrameHandler(&endPoint);
	uart_init();
	endPoint.connect();
	while(!endPoint.isConnected()) endPoint.schedule();

	eeprom_busy_wait();
	outgoingFrameBuffer.put(OpCode::MY_ID);
	outgoingFrameBuffer.put(eeprom_read_byte(&eeprom_id));
	outgoingFrameBuffer.endFrame();

	wdt_enable(WDTO_15MS);
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
