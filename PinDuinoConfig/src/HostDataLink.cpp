/*
 * HostDataLink.cpp
 *
 *  Created on: May 1, 2015
 *      Author: peter
 */

#include "HostDataLink.h"

#include <stdio.h>
#include <stdarg.h>

#include <easylogging++.h>

HostDataLink::HostDataLink(ByteSource& byteSource, ByteSink& byteSink) : IncomingDataLink(byteSource), OutgoingDataLink(byteSink) {
}

HostDataLink::~HostDataLink() {
}

void HostDataLink::schedule() {
	PT_SCHEDULE(outgoing_thread());
	PT_SCHEDULE(incoming_thread());
}

void HostDataLink::log(const char* fmt, ...) {
	va_list ap;

	char f[256];

	va_start(ap, fmt);
	vsprintf(f, fmt, ap);
	va_end(ap);

	LOG(INFO) << f;
}
