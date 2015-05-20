/*
 * OutgoingPinDuinoDataLink.h
 *
 *  Created on: May 20, 2015
 *      Author: peter
 */

#ifndef OUTGOINGPINDUINODATALINK_H_
#define OUTGOINGPINDUINODATALINK_H_

#include <IncomingDataLink.h>
#include <OutgoingDataLink.h>

class OutgoingPinDuinoDataLink: public OutgoingDataLink {
public:
	OutgoingPinDuinoDataLink(ByteSink&);
	virtual ~OutgoingPinDuinoDataLink();

	void log(const char*, ...);
};

#endif /* OUTGOINGPINDUINODATALINK_H_ */
