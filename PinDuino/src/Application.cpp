/*
 * Application.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <avr/eeprom.h>

#include <Application.h>
#include <OpCode.h>
#include <StimulusResponse.h>

Application::Application(PingPong &pingPong) : pingPong(pingPong) {
	PT_INIT(&pt);

	datalink.begin_outgoing_frame(OpCode::MY_ID);
	datalink.append_payload(eeprom_read_byte(0x0000));
	datalink.end_outgoing_frame();
}

Application::~Application() {
	// TODO Auto-generated destructor stub
}

void Application::schedule() {
	PT_SCHEDULE(run());
}

PT_THREAD(Application::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		PT_WAIT_UNTIL(&pt, datalink.have_incoming_frame());

		if(datalink.peek(0) == OpCode::PONG) {
			pingPong.acceptPong();
		} else if(datalink.peek(0) == OpCode::SR_INHIBIT) {
			stimulusResponse.inhibit();
		} else if(datalink.peek(0) == OpCode::SR_ENABLE) {
			stimulusResponse.enable();
		} else if(datalink.peek(0) == OpCode::SR_CONFIG) {
			SolenoidAction action;
			uint8_t *p = (uint8_t*)&action;
			uint8_t i = 1;
			uint8_t pin = datalink.peek(i++);
			bool newState = datalink.peek(i++);
			*p++=datalink.peek(i++);
			*p++=datalink.peek(i++);
			*p++=datalink.peek(i++);
			*p++=datalink.peek(i++);
			stimulusResponse.config(pin, newState, action);
			datalink.log("Received SR_CONFIG: %i", action.solenoidIndex);
		}

		datalink.next_incoming_frame();
	}
	PT_END(&pt);
}
