/*
 * SolenoidBank.h
 *
 *  Created on: Jun 18, 2015
 *      Author: peter
 */

#ifndef SOLENOIDBANK_H_
#define SOLENOIDBANK_H_

#include <ItemBank.h>

#include "Solenoid.h"

class SolenoidBank : public ItemBank {
	virtual void updateSelf() {}
	virtual void updateItem(Item* item) {
		((Solenoid*)item) -> update();
	}

public:
	SolenoidBank(Item** solenoids, uint8_t& dirtyList) : ItemBank(solenoids, dirtyList) {}
	virtual ~SolenoidBank() {}
};

#endif /* SOLENOIDBANK_H_ */
