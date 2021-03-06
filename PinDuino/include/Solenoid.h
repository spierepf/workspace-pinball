/*
 * Solenoid.h
 *
 *  Created on: Feb 24, 2015
 *      Author: peter
 */

#ifndef SOLENOID_H_
#define SOLENOID_H_

#include <Item.h>

#include <PinBank.h>
#include <Timer.h>

class Solenoid : public Item {
	friend class SolenoidBank;

	PinBank pin;
	uint16_t attack;
	uint8_t sustain;
	Timer<uint16_t> timer;

	volatile uint8_t& tccra;
	uint8_t pwmMask;
	uint8_t pwmEnableBits;
	volatile uint8_t& ocr;

	void enablePWM();
	void disablePWM();
	void setOCR(uint8_t);

	void beginSustain();

	bool inAttack();
	bool inSustain();
	bool inRelease();

public:
	Solenoid(PinBank&, volatile uint8_t&, uint8_t, uint8_t, volatile uint8_t&, uint8_t, uint8_t&);
	~Solenoid();

	void trigger(uint16_t, uint8_t);
	void release();

	void update();
};

#endif /* SOLENOID_H_ */
