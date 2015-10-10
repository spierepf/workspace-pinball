/*
 * extern.h
 *
 *  Created on: Oct 7, 2015
 *      Author: peter
 */

#ifndef INCLUDE_EXTERN_H_
#define INCLUDE_EXTERN_H_

#include <FrameBuffer.h>
#include <PingPong.h>
#include <StimulusResponse.h>
#include <Solenoid.h>

extern hdlc::FrameBuffer outgoingFrameBuffer;
extern class PingPong pingPong;
extern class StimulusResponse stimulusResponse;
extern class Solenoid *solenoids[];
extern class PinBank led;

#endif /* INCLUDE_EXTERN_H_ */
