/*
 * StimulusActionMapTest.cpp
 *
 *  Created on: Sep 1, 2015
 *      Author: peter
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <IncomingDataLink.h>
#include <OutgoingDataLink.h>

#include <StimulusActionMap.h>

BOOST_AUTO_TEST_CASE( stimulus_action_map_test ) {
	SolenoidAction action(true, 2, 3, 4);
	Stimulus stimulus(4, true);

	StimulusActionMap map;

	map[stimulus] = action;

	SolenoidAction result = map[stimulus];

	BOOST_CHECK( action.enabled == result.enabled );
	BOOST_CHECK( action.solenoidIndex == result.solenoidIndex );
	BOOST_CHECK( action.attack == result.attack );
	BOOST_CHECK( action.sustain == result.sustain );
}
