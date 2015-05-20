/*
 * HostDataLink.h
 *
 *  Created on: May 1, 2015
 *      Author: peter
 */

#ifndef HOSTDATALINK_H_
#define HOSTDATALINK_H_

#include <IncomingDataLink.h>
#include <OutgoingDataLink.h>

class HostDataLink: public IncomingDataLink, public OutgoingDataLink {
public:
	HostDataLink(ByteSource&, ByteSink&);
	virtual ~HostDataLink();

	/** Used to schedule our incoming and outgoing protothreads for execution. */
	void schedule();

	virtual void log(const char*, ...);
};

#endif /* HOSTDATALINK_H_ */
