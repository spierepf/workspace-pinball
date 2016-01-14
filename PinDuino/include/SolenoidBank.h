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

class SolenoidBank : public ItemBank<SolenoidBank> {
public:
	SolenoidBank(Solenoid** solenoids, uint8_t& dirtyList) : ItemBank<SolenoidBank>((Item**)solenoids, dirtyList) {}
	~SolenoidBank() {}

	void updateSelf_impl() {}
	void updateItem_impl(Item* item) {
		((Solenoid*)item) -> update();
	}
};

#endif /* SOLENOIDBANK_H_ */
