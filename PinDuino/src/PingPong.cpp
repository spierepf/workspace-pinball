/*
 * PingPong.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: peter
 */

#include <Arduino.h>

#include "PingPong.h"

#define N 128

PingPong::PingPong(PinDuinoDataLink &datalink) : datalink(datalink), total_latency(0), wait_time(0), counter(0), pongAccepted(false) {
	PT_INIT(&pt);
}

PingPong::~PingPong() {
	// TODO Auto-generated destructor stub
}

void PingPong::schedule() {
	PT_SCHEDULE(run());
}

PT_THREAD(PingPong::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		for(counter = 0; counter < N; counter++) {
			// random delay
			wait_time = random(100000);
			timer.set(micros());
			PT_WAIT_UNTIL(&pt, timer.elapsed(micros()) > wait_time);

			timer.set(micros());

			pongAccepted = false;
			sendPing();

			PT_WAIT_UNTIL(&pt, pongAccepted);

			total_latency += timer.elapsed(micros());
		}

		datalink.log("Average Latency: %lu us", total_latency / N);
		total_latency = 0;
	}
	PT_END(&pt);
}

void PingPong::sendPing() {
	datalink.begin_outgoing_frame(OpCode::PING); // ping
	datalink.append_payload(counter);
	datalink.end_outgoing_frame();
}

void PingPong::acceptPong() {
	pongAccepted = datalink.peek(1) == counter;
	if(!pongAccepted) {
		sendPing();
	}
}
