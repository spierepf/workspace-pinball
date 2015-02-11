/*
 * Pins.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef INCLUDE_PINS_H_
#define INCLUDE_PINS_H_

#include <avr/io.h>

class Pin {
public:
    virtual void dirIn() = 0;
	virtual void dirOut() = 0;
	virtual bool read() = 0;
	virtual void toggle() = 0;
	virtual void dataLow() = 0;
	virtual void dataHigh() = 0;

	void setDir(bool b) { b ? dirOut() : dirIn(); }
    void write(bool b) { b ? dataHigh() : dataLow(); }
};

#define PIN(NAME, NUM, PORTR, DDR, PINR) \
class Pin##NAME : public Pin { \
public: \
    void dirIn() { DDR &= ~_BV(NUM); } \
    void dirOut() { DDR |= _BV(NUM); } \
    bool read() { return (PINR >> NUM) & 1; } \
    void toggle() { PINR |= _BV(NUM); } \
    void dataLow() { PORTR &= ~_BV(NUM); } \
    void dataHigh() { PORTR|= _BV(NUM); } \
}

PIN(B0, 0, PORTB, DDRB, PINB);
PIN(B1, 1, PORTB, DDRB, PINB);
PIN(B2, 2, PORTB, DDRB, PINB);
PIN(B3, 3, PORTB, DDRB, PINB);
PIN(B4, 4, PORTB, DDRB, PINB);
PIN(B5, 5, PORTB, DDRB, PINB);

PIN(C0, 0, PORTC, DDRC, PINC);
PIN(C1, 1, PORTC, DDRC, PINC);
PIN(C2, 2, PORTC, DDRC, PINC);
PIN(C3, 3, PORTC, DDRC, PINC);
PIN(C4, 4, PORTC, DDRC, PINC);
PIN(C5, 5, PORTC, DDRC, PINC);

PIN(D0, 0, PORTB, DDRB, PIND);
PIN(D1, 1, PORTB, DDRB, PIND);
PIN(D2, 2, PORTB, DDRB, PIND);
PIN(D3, 3, PORTB, DDRB, PIND);
PIN(D4, 4, PORTB, DDRB, PIND);
PIN(D5, 5, PORTB, DDRB, PIND);
PIN(D6, 6, PORTB, DDRB, PIND);
PIN(D7, 7, PORTB, DDRB, PIND);

PIN(0, 0, PORTB, DDRB, PIND);
PIN(1, 1, PORTB, DDRB, PIND);
PIN(2, 2, PORTB, DDRB, PIND);
PIN(3, 3, PORTB, DDRB, PIND);
PIN(4, 4, PORTB, DDRB, PIND);
PIN(5, 5, PORTB, DDRB, PIND);
PIN(6, 6, PORTB, DDRB, PIND);
PIN(7, 7, PORTB, DDRB, PIND);

PIN(8, 0, PORTB, DDRB, PINB);
PIN(9, 1, PORTB, DDRB, PINB);
PIN(10, 2, PORTB, DDRB, PINB);
PIN(11, 3, PORTB, DDRB, PINB);
PIN(12, 4, PORTB, DDRB, PINB);
PIN(13, 5, PORTB, DDRB, PINB);

PIN(A0, 0, PORTC, DDRC, PINC);
PIN(A1, 1, PORTC, DDRC, PINC);
PIN(A2, 2, PORTC, DDRC, PINC);
PIN(A3, 3, PORTC, DDRC, PINC);
PIN(A4, 4, PORTC, DDRC, PINC);
PIN(A5, 5, PORTC, DDRC, PINC);

#undef PIN

#endif /* INCLUDE_PINS_H_ */
