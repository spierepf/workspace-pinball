/*
 * PinDuinoDataLink.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef PINDUINODATALINK_H_
#define PINDUINODATALINK_H_

#include <IncomingDataLink.h>
#include <OutgoingDataLink.h>

class PinDuinoDataLink: public IncomingDataLink, public OutgoingDataLink {
public:
	PinDuinoDataLink(ByteSource&, ByteSink&);
	virtual ~PinDuinoDataLink();

	/** Used to schedule our incoming and outgoing protothreads for execution. */
	void schedule();

	void log(const char*, ...);
};

extern PinDuinoDataLink datalink;

#endif /* PINDUINODATALINK_H_ */
