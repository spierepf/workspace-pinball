/*
 * EndPoint.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <avr/eeprom.h>
extern uint8_t eeprom_id;

#include <EndPoint.h>
#include <OpCode.h>
#include <StimulusResponse.h>
extern Solenoid *solenoids[];

EndPoint::EndPoint(IncomingDataLink& _incomingDatalink, OutgoingPinDuinoDataLink& _outgoingDatalink, FrameBuffer<64, 4>& _incomingFrames, FrameBuffer<64, 4>& _outgoingFrames, PingPong &pingPong) : AbstractEndPoint(_incomingDatalink, _outgoingDatalink, _incomingFrames, _outgoingFrames), pingPong(pingPong), logger(_outgoingDatalink) {
	eeprom_busy_wait();
	outgoingFrames.put(OpCode::MY_ID);
	outgoingFrames.put(eeprom_read_byte(&eeprom_id));
	outgoingFrames.endFrame();
}

EndPoint::~EndPoint() {
	// TODO Auto-generated destructor stub
}

void EndPoint::handleIncomingFrame() {
	if(incomingFrames[0][0] == OpCode::PONG) {
		pingPong.acceptPong(incomingFrames[0][1]);
	} else if(incomingFrames[0][0] == OpCode::PULSE_COIL) {
		SolenoidAction action;
		uint8_t i = 1;
		action.read_from(incomingFrames[0], i);
		if(action.enabled && action.solenoidIndex < 6 && action.attack <= 65000) {
			if(action.attack == 0) {
				if(solenoids[action.solenoidIndex] != NULL) solenoids[action.solenoidIndex] -> release();
			} else {
				if(solenoids[action.solenoidIndex] != NULL) solenoids[action.solenoidIndex] -> trigger(action.attack, action.sustain);
			}
		}
	} else if(incomingFrames[0][0] == OpCode::SR_INHIBIT) {
		stimulusResponse.inhibit();
	} else if(incomingFrames[0][0] == OpCode::SR_ENABLE) {
		stimulusResponse.enable();
	} else if(incomingFrames[0][0] == OpCode::SR_CONFIG) {
		uint8_t i = 1;
		if(incomingFrames[0].getLength() >= i+sizeof(Stimulus)) {
			Stimulus stimulus;
			stimulus.read_from(incomingFrames[0], i);
			if(incomingFrames[0].getLength() >= i+sizeof(SolenoidAction)) {
				SolenoidAction action;
				action.read_from(incomingFrames[0], i);
				stimulusResponse[stimulus] = action;
			} else {
				outgoingFrames.put(OpCode::SR_CONFIG);
				stimulus.write_to(outgoingFrames);
				stimulusResponse[stimulus].write_to(outgoingFrames);
				outgoingFrames.endFrame();
			}
		}
	}
}

