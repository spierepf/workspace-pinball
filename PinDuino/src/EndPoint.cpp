/*
 * EndPoint.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <avr/eeprom.h>
extern uint8_t eeprom_id;
extern uint32_t eeprom_actions[12][2];

#include <EndPoint.h>
#include <OpCode.h>
#include <StimulusResponse.h>
extern Solenoid solenoids[6];

EndPoint::EndPoint(IncomingDataLink& _incomingDatalink, OutgoingPinDuinoDataLink& _outgoingDatalink, PingPong &pingPong) : AbstractEndPoint(_incomingDatalink, _outgoingDatalink), pingPong(pingPong), logger(_outgoingDatalink) {
	eeprom_busy_wait();
	outgoingDatalink.begin_outgoing_frame(OpCode::MY_ID);
	outgoingDatalink.append_payload(eeprom_read_byte(&eeprom_id));
	outgoingDatalink.end_outgoing_frame();
}

EndPoint::~EndPoint() {
	// TODO Auto-generated destructor stub
}

void EndPoint::handleIncomingFrame() {
	if(incomingDatalink.peek(0) == OpCode::PONG) {
		pingPong.acceptPong(incomingDatalink.peek(1));
	} else if(incomingDatalink.peek(0) == OpCode::PULSE_COIL) {
		SolenoidAction action;
		uint8_t i = 1;
		action.read_from(incomingDatalink, i);
		if(action.enabled && action.solenoidIndex < 6 && action.attack <= 65000) {
			if(action.attack == 0) {
				solenoids[action.solenoidIndex].release();
			} else {
				solenoids[action.solenoidIndex].trigger(action.attack, action.sustain);
			}
		}
	} else if(incomingDatalink.peek(0) == OpCode::SR_INHIBIT) {
		stimulusResponse.inhibit();
	} else if(incomingDatalink.peek(0) == OpCode::SR_ENABLE) {
		stimulusResponse.enable();
	} else if(incomingDatalink.peek(0) == OpCode::SR_CONFIG) {
		uint8_t i = 1;
		if(incomingDatalink.incoming_frame_length() >= i+sizeof(Stimulus)) {
			Stimulus stimulus;
			stimulus.read_from(incomingDatalink, i);
			if(incomingDatalink.incoming_frame_length() >= i+sizeof(SolenoidAction)) {
				SolenoidAction action;
				action.read_from(incomingDatalink, i);
				stimulusResponse[stimulus] = action;
			} else {
				outgoingDatalink.begin_outgoing_frame(OpCode::SR_CONFIG);
				stimulus.write_to(outgoingDatalink);
				stimulusResponse[stimulus].write_to(outgoingDatalink);
				outgoingDatalink.end_outgoing_frame();
			}
		}
	}
}

