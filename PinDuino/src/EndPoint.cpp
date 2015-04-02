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

EndPoint::EndPoint(DataLink& _datalink, PingPong &pingPong) : AbstractEndPoint(_datalink), pingPong(pingPong) {
	eeprom_busy_wait();
	datalink.begin_outgoing_frame(OpCode::MY_ID);
	datalink.append_payload(eeprom_read_byte(&eeprom_id));
	datalink.end_outgoing_frame();
}

EndPoint::~EndPoint() {
	// TODO Auto-generated destructor stub
}

void EndPoint::handleIncomingFrame() {
	if(datalink.peek(0) == OpCode::PONG) {
		pingPong.acceptPong();
	} else if(datalink.peek(0) == OpCode::SR_INHIBIT) {
		stimulusResponse.inhibit();
	} else if(datalink.peek(0) == OpCode::SR_ENABLE) {
		stimulusResponse.enable();
	} else if(datalink.peek(0) == OpCode::SR_CONFIG) {
		uint8_t i = 1;
		if(datalink.incoming_frame_length() >= i+sizeof(Stimulus)) {
			Stimulus stimulus;
			stimulus.read_from(datalink, i);
			if(datalink.incoming_frame_length() >= i+sizeof(SolenoidAction)) {
				SolenoidAction action;
				action.read_from(datalink, i);
				stimulusResponse[stimulus] = action;
				eeprom_busy_wait();
				eeprom_write_dword(&eeprom_actions[stimulus.pin][stimulus.newState], action);
			} else {
				datalink.begin_outgoing_frame(OpCode::SR_CONFIG);
				stimulus.write_to(datalink);
				stimulusResponse[stimulus].write_to(datalink);
			}
		}
	}
}

