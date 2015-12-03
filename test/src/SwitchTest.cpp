/*
 * SwitchTest.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: peter
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <stdint.h>

#include <Switch.h>

bool currentState = false;

class SwitchListenerImpl : public SwitchListener {
	virtual void switchChange(uint8_t, bool newState) const {
		currentState = newState;
	}
};

BOOST_AUTO_TEST_CASE( switch_no_bounce ) {
	currentState = true;

	SwitchListenerImpl switchListener;
	uint8_t dirtyList = 0;

	Switch s(switchListener, 0, 0, dirtyList);

	dirtyList = 1;						// dirty the switch
	s.update(0, 1, 0); 					// @ t=0 the pin goes from high to low

	BOOST_CHECK(currentState == true);	// the current state is still high
	BOOST_CHECK(dirtyList != 0);		// the switch is still dirty

	s.update(499, 0, 0); 				// @ t=499 the pin is still low

	BOOST_CHECK(currentState == true);	// the current state is still high
	BOOST_CHECK(dirtyList != 0);		// the switch is still dirty

	s.update(500, 0, 0); 				// @ t=500 the pin is still low

	BOOST_CHECK(currentState == false);	// the current state is now low
	BOOST_CHECK(dirtyList == 0);		// the switch is clean

	dirtyList = 1;						// dirty the switch
	s.update(1000, 0, 1);				// @ t=1000 the pin goes from low to high

	BOOST_CHECK(currentState == false);	// the current state is still low
	BOOST_CHECK(dirtyList != 0);		// the switch is dirty

	s.update(1499, 1, 1);				// @ t=1499 the pin is still high

	BOOST_CHECK(currentState == false);	// the current state is still low
	BOOST_CHECK(dirtyList != 0);		// the switch is still dirty

	s.update(1500, 1, 1);				// @ t=1500 the pin is still high

	BOOST_CHECK(currentState == true);	// the current state is now high
	BOOST_CHECK(dirtyList == 0);		// the switch is clean
}

BOOST_AUTO_TEST_CASE( switch_bounce ) {
	currentState = true;

	SwitchListenerImpl switchListener;
	uint8_t dirtyList = 0;

	Switch s(switchListener, 0, 0, dirtyList);

	dirtyList = 1;						// dirty the switch
	s.update(0, 1, 0); 					// @ t=0 the pin goes from high to low

	BOOST_CHECK(currentState == true);	// the current state is still high
	BOOST_CHECK(dirtyList != 0);		// the switch is still dirty

	s.update(499, 0, 1); 				// @ t=499 the pin bounces from low to high

	BOOST_CHECK(currentState == true);	// the current state is still high
	BOOST_CHECK(dirtyList == 0);		// the switch is clean

	dirtyList = 1;						// dirty the switch
	s.update(500, 1, 0); 				// @ t=500 the pin returns from high to low

	BOOST_CHECK(currentState == true);	// the current state is still high
	BOOST_CHECK(dirtyList == 1);		// the switch is still dirty

	s.update(999, 0, 0);				// @ t=999 the pin is still low

	BOOST_CHECK(currentState == true);	// the current state is still high
	BOOST_CHECK(dirtyList == 1);		// the switch is still dirty

	s.update(1000, 0, 0);				// @ t=1000 the pin is still low

	BOOST_CHECK(currentState == false);	// the current state is now low
	BOOST_CHECK(dirtyList == 0);		// the switch is clean
}

BOOST_AUTO_TEST_CASE( setDebounceThreshold ) {
	currentState = true;

	SwitchListenerImpl switchListener;
	uint8_t dirtyList = 0;

	Switch s(switchListener, 0, 0, dirtyList);

	s.setDebounceThreshold(262140);

	dirtyList = 1;						// dirty the switch
	s.update(0, 1, 0); 					// @ t=0 the pin goes from high to low

	BOOST_CHECK(currentState == true);	// the current state is still high
	BOOST_CHECK(dirtyList != 0);		// the switch is still dirty

	s.update(262139, 0, 0); 			// @ t=262139 the pin is still low

	BOOST_CHECK(currentState == true);	// the current state is still high
	BOOST_CHECK(dirtyList != 0);		// the switch is still dirty

	s.update(262140, 0, 0); 			// @ t=262140 the pin is still low

	BOOST_CHECK(currentState == false);	// the current state is now low
	BOOST_CHECK(dirtyList == 0);		// the switch is clean
}
