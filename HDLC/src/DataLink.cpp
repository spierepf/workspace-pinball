/*
 * DataLink.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: peter
 */

#include <DataLink.h>

DataLink::DataLink(ByteSource& byteSource, ByteSink& byteSink) : IncomingDataLink(byteSource), OutgoingDataLink(byteSink) {
}

DataLink::~DataLink() {
	// TODO Auto-generated destructor stub
}

void DataLink::schedule() {
	PT_SCHEDULE(outgoing_thread());
	PT_SCHEDULE(incoming_thread());
}

