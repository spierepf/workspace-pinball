/*
 * PinDuinoFrameHandler.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: peter
 */

#include <extern.h>

#include "PinDuinoFrameHandler.h"

#include <OpCode.h>

#include <SolenoidAction.h>
#include <Stimulus.h>
#include <Solenoid.h>
#include <StimulusResponse.h>
#include <Switch.h>

extern Solenoid *solenoids[];
extern Switch *switchesC[];
extern Switch *switchesD[];

PinDuinoFrameHandler::PinDuinoFrameHandler() {
}

PinDuinoFrameHandler::~PinDuinoFrameHandler() {
	// TODO Auto-generated destructor stub
}

void PinDuinoFrameHandler::handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) {
	if(payload[0] == OpCode::PONG) {
		pingPong.acceptPong(payload[1]);
	} else if(payload[0] == OpCode::PULSE_COIL) {
		SolenoidAction action;
		uint8_t i = 1;
		action.read_from(payload, i);
		if(action.enabled && action.solenoidIndex < 6 && action.attack <= 65000) {
			if(action.attack == 0) {
				if(solenoids[action.solenoidIndex] != NULL) solenoids[action.solenoidIndex] -> release();
			} else {
				if(solenoids[action.solenoidIndex] != NULL) solenoids[action.solenoidIndex] -> trigger(action.attack, action.sustain);
			}
		}
	} else if(payload[0] == OpCode::SR_INHIBIT) {
		stimulusResponse.inhibit();
	} else if(payload[0] == OpCode::SR_ENABLE) {
		stimulusResponse.enable();
	} else if(payload[0] == OpCode::SR_CONFIG) {
		uint8_t i = 1;
		if(payloadSize >= i+sizeof(Stimulus)) {
			Stimulus stimulus;
			stimulus.read_from(payload, i);
			if(payloadSize >= i+sizeof(SolenoidAction)) {
				SolenoidAction action;
				action.read_from(payload, i);
				stimulusResponse[stimulus] = action;
			} else {
				outgoingFrameBuffer.put(OpCode::SR_CONFIG);
				stimulus.write_to(outgoingFrameBuffer);
				stimulusResponse[stimulus].write_to(outgoingFrameBuffer);
				outgoingFrameBuffer.endFrame();
			}
		}
	} else if(payload[0] == OpCode::SWITCH_DEBOUNCE_CONFIG) {
		uint8_t i = 1;
		if(payloadSize >= i+sizeof(uint8_t)+sizeof(bool)+sizeof(uint32_t)) {
			uint8_t switchId = payload[i++];
			bool newState = payload[i++] ? true : false;
			uint32_t usec;
			uint8_t *p = (uint8_t*)&usec;
			*p++ = payload[i++];
			*p++ = payload[i++];
			*p++ = payload[i++];
			*p++ = payload[i++];

			Switch* s = (switchId < 6 ? switchesC : switchesD)[switchId % 6 + (switchId < 6 ? 0 : 2)];
			s->setDebounceThreshold(newState, usec);
		}
	} else if(payload[0] == OpCode::SWITCH_PULLUP_CONFIG) {
		uint8_t i = 1;
		if(payloadSize >= i+sizeof(uint8_t)+sizeof(bool)) {
			uint8_t switchId = payload[i++];
			bool enablePullup = payload[i++] ? true : false;

			Switch* s = (switchId < 6 ? switchesC : switchesD)[switchId % 6 + (switchId < 6 ? 0 : 2)];
			s->setPullup(enablePullup);
		}
	}
}
