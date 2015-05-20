/*
 * DataLink.h
 *
 *  Created on: Jan 29, 2015
 *      Author: peter
 */

#ifndef DATALINK_H_
#define DATALINK_H_

#include <pt.h>

#include <ByteSource.h>
#include <ByteSink.h>
#include <IncomingDataLink.h>
#include <OutgoingDataLink.h>

class DataLink : public IncomingDataLink, public OutgoingDataLink {
public:
	DataLink(ByteSource&, ByteSink&);
	virtual ~DataLink();

	/** Used to schedule our incoming and outgoing protothreads for execution. */
	void schedule();
};

#endif /* DATALINK_H_ */
