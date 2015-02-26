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
		}

		datalink.next_incoming_frame();
	}
	PT_END(&pt);
}
