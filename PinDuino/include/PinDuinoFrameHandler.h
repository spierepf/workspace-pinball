/*
 * PinDuinoFrameHandler.h
 *
 *  Created on: Oct 7, 2015
 *      Author: peter
 */

#ifndef PINDUINOFRAMEHANDLER_H_
#define PINDUINOFRAMEHANDLER_H_

#include <FrameHandler.h>

#include <FrameBuffer.h>

#include <PingPong.h>

class PinDuinoFrameHandler: public hdlc::FrameHandler {
public:
	PinDuinoFrameHandler();
	virtual ~PinDuinoFrameHandler();

	virtual void handle(const uint8_t, const uint8_t*, const uint8_t);
};

#endif /* PINDUINOFRAMEHANDLER_H_ */
