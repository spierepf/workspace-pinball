/*
 * Application.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <Application.h>

Application::Application(PinDuinoDataLink &datalink, PingPong &pingPong) : datalink(datalink), pingPong(pingPong) {
	PT_INIT(&pt);
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
		}

		datalink.next_incoming_frame();
	}
	PT_END(&pt);
}
