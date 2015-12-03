/*
 * SwitchListenerImpl.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: peter
 */

#include "SwitchListenerImpl.h"

#include <OpCode.h>
#include <extern.h>

SwitchListenerImpl::SwitchListenerImpl() {
	// TODO Auto-generated constructor stub

}

SwitchListenerImpl::~SwitchListenerImpl() {
	// TODO Auto-generated destructor stub
}

void SwitchListenerImpl::switchChange(uint8_t id, bool newState) const {
	outgoingFrameBuffer.put(newState ? OpCode::SWITCH_INACTIVE : OpCode::SWITCH_ACTIVE);
	outgoingFrameBuffer.put(id);
	outgoingFrameBuffer.endFrame();
	stimulusResponse.trigger(Stimulus(id, newState));
}
