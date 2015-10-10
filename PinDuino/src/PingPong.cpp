/*
 * PingPong.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: peter
 */

#include <Arduino.h>

#include <PingPong.h>
#include <OpCode.h>

#include <log.h>

#define N 128

PingPong::PingPong() : total_latency(0), counter(0), pongAccepted(false), missedPings(0) {
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
			PT_WAIT_UNTIL(&pt, pongAccepted/* || timer.elapsed(micros()) > 200000*/);
			if(!pongAccepted) missedPings++;
			total_latency += timer.elapsed(micros());
			PT_WAIT_UNTIL(&pt, timer.elapsed(micros()) > 100000);
		}

		LOG("Average Latency: %lu us", total_latency / N);
		if(missedPings > 0) LOG("\tMissed Pings: %u", missedPings);
		missedPings = 0;
		total_latency = 0;
	}
	PT_END(&pt);
}

void PingPong::sendPing() {
	pongAccepted = false;
	outgoingFrameBuffer.put(OpCode::PING); // ping
	outgoingFrameBuffer.put(counter);
	outgoingFrameBuffer.endFrame();
}

void PingPong::acceptPong(uint8_t id) {
	pongAccepted = id == counter;
}
