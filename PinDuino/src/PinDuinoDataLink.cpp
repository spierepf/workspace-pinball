/*
 * PinDuinoDataLink.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <stdio.h>
#include <limits.h>
#include <string.h>

#include <PinDuinoDataLink.h>
#include <OpCode.h>

PinDuinoDataLink::PinDuinoDataLink(ByteSource& byteSource, ByteSink& byteSink) : IncomingDataLink(byteSource), OutgoingDataLink(byteSink) {
	// TODO Auto-generated constructor stub
}

PinDuinoDataLink::~PinDuinoDataLink() {
	// TODO Auto-generated destructor stub
}

void PinDuinoDataLink::schedule() {
	PT_SCHEDULE(outgoing_thread());
	PT_SCHEDULE(incoming_thread());
}

void PinDuinoDataLink::log(const char *fmt, ...)
{
	char s[32];

	va_list ap;
	FILE f;

	f.flags = __SWR | __SSTR;
	f.buf = s;
	f.size = INT_MAX;
	va_start(ap, fmt);
	vfprintf(&f, fmt, ap);
	va_end(ap);
	s[f.len] = 0;

	begin_outgoing_frame(OpCode::LOG); // log
	for(size_t i = 0; i < strlen(s); i++) {
		append_payload(s[i]);
	}
	end_outgoing_frame();
}

