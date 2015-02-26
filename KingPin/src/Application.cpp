/*
 * Application.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <Application.h>
#include <OpCode.h>

#include <easylogging++.h>

Application::Application(string device, Hardware* hardware) : device(device), hardware(hardware), datalink(*hardware), id(255) {
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
			char buf[32];
			for(int i = 1; i < datalink.incoming_frame_length(); i++) {
				buf[i-1] = (char)datalink.peek(i);
			}
			buf[datalink.incoming_frame_length() - 1] = '\0';
			LOG(INFO) << "From " << id << ": " << buf;
		} else if(datalink.peek(0) == OpCode::MY_ID) {
			id = datalink.peek(1);
			LOG(INFO) << "Device " << device << " registered id: " << (int)id;
			datalink.begin_outgoing_frame(OpCode::SR_ENABLE);
			datalink.end_outgoing_frame();
		} else if(datalink.peek(0) == OpCode::PIN_LOW) {
			LOG(INFO) << "Pin Low: " << id << ":" << (int)datalink.peek(1);
		} else if(datalink.peek(0) == OpCode::PIN_HIGH) {
			LOG(INFO) << "Pin High: " << id << ":"  << (int)datalink.peek(1);
		}

		datalink.next_incoming_frame();
	}
	PT_END(&pt);
}
