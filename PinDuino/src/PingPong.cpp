/*
 * PingPong.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: peter
 */

#include <Arduino.h>

#include <PingPong.h>
#include <OpCode.h>

#define N 128

PingPong::PingPong(OutgoingPinDuinoDataLink& outgoingDatalink) : outgoingDatalink(outgoingDatalink), total_latency(0), counter(0), pongAccepted(false), missedPings(0) {
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
			timer.set(micros());
			sendPing();
			PT_WAIT_UNTIL(&pt, pongAccepted || timer.elapsed(micros()) > 100000);
			if(!pongAccepted) missedPings++;
			total_latency += timer.elapsed(micros());
			PT_WAIT_UNTIL(&pt, timer.elapsed(micros()) > 100000);
		}

		outgoingDatalink.log("Average Latency: %lu us", total_latency / N);
		if(missedPings > 0) outgoingDatalink.log("\tMissed Pings: %u", missedPings);
		missedPings = 0;
		total_latency = 0;
	}
	PT_END(&pt);
}

void PingPong::sendPing() {
	pongAccepted = false;
	outgoingDatalink.begin_outgoing_frame(OpCode::PING); // ping
	outgoingDatalink.append_payload(counter);
	outgoingDatalink.end_outgoing_frame();
}

void PingPong::acceptPong(uint8_t id) {
	pongAccepted = id == counter;
	if(!pongAccepted) {
		sendPing();
	}
}
