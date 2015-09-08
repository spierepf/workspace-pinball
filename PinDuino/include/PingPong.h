/*
 * PingPong.h
 *
 *  Created on: Feb 4, 2015
 *      Author: peter
 */

#ifndef PINGPONG_H_
#define PINGPONG_H_

#include <pt.h>

#include <OutgoingPinDuinoDataLink.h>
#include <Timer.h>

class PingPong {
	struct pt pt;
	PT_THREAD(run());

	OutgoingPinDuinoDataLink& outgoingDatalink;
	FrameBuffer<64, 4>& outgoingFrames;
	Timer<unsigned long> timer;
	unsigned long total_latency;
	uint8_t counter;
	bool pongAccepted;
	uint16_t missedPings;

	void sendPing();

public:
	PingPong(OutgoingPinDuinoDataLink&, FrameBuffer<64, 4>&);
	virtual ~PingPong();

	void schedule();

	void acceptPong(uint8_t);
};

#endif /* PINGPONG_H_ */
