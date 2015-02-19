/*
 * Application.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <iostream>

#include <Application.h>

using namespace std;

Application::Application(DataLink &datalink) : datalink(datalink) {
	PT_INIT(&pt);
}

Application::~Application() {
	// TODO Auto-generated destructor stub
}

void Application::schedule() {
	datalink.schedule();
	PT_SCHEDULE(run());
}

PT_THREAD(Application::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		PT_WAIT_UNTIL(&pt, datalink.have_incoming_frame());

		if(datalink.peek(0) == OpCode::PING) { // ping
			datalink.begin_outgoing_frame(OpCode::PONG); // pong
			datalink.append_payload(datalink.peek(1)); // return the payload of the ping
			datalink.end_outgoing_frame();
		} else if(datalink.peek(0) == OpCode::LOG) { // log
			for(int i = 1; i < datalink.incoming_frame_length(); i++) {
				cout << (char)datalink.peek(i);
			}
			cout << endl;
		} else if(datalink.peek(0) == 0x10) {
			cout << "Pin Low" << endl;
		} else if(datalink.peek(0) == 0x11) {
			cout << "Pin High" << endl;
		}

		datalink.next_incoming_frame();
	}
	PT_END(&pt);
}
