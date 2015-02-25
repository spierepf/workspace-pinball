/*
 * PingPong.h
 *
 *  Created on: Feb 4, 2015
 *      Author: peter
 */

#ifndef PINGPONG_H_
#define PINGPONG_H_

#include <pt.h>

#include <PinDuinoDataLink.h>
#include <Timer.h>

class PingPong {
	struct pt pt;
	PT_THREAD(run());

	Timer<unsigned long> timer;
	unsigned long total_latency;
	unsigned long wait_time;
	uint8_t counter;
	bool pongAccepted;

	void sendPing();

public:
	PingPong();
	virtual ~PingPong();

	void schedule();

	void acceptPong();
};

#endif /* PINGPONG_H_ */
