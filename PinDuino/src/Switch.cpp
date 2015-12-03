/*
 * Switch.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: peter
 */

#include "Switch.h"

Switch::Switch(const SwitchListener& switchListener, uint8_t index, uint8_t id, uint8_t& dirtyList) : Item((1 << index), dirtyList), switchListener(switchListener), id(id), state(true) {
	debounceThreshold[0] = debounceThreshold[1] = (500>>2);
}

Switch::~Switch() {
	// TODO Auto-generated destructor stub
}

void Switch::pinChange(bool newState) {
	state = newState;
	switchListener.switchChange(id, newState);
}

void Switch::update(uint32_t usec, uint8_t last, uint8_t current) {
	bool newState = current & mask;

	if(state == newState) {
		clean();
		return;
	}

	if((mask & last) != (mask & current)) {
		debounceTimer.set(usec);
	}

	if(debounceTimer.elapsed(usec) >= (debounceThreshold[newState ? 1 : 0] << 2)) {
		clean();
		pinChange(newState);
	}
}

void Switch::setDebounceThreshold(bool newState, uint32_t usec) {
	debounceThreshold[newState ? 1 : 0] = usec >> 2;
}
