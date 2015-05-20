/*
 * OutgoingPinDuinoDataLink.cpp
 *
 *  Created on: May 20, 2015
 *      Author: peter
 */

#include "OutgoingPinDuinoDataLink.h"

#include <stdio.h>
#include <limits.h>
#include <string.h>

#include <OpCode.h>

OutgoingPinDuinoDataLink::OutgoingPinDuinoDataLink(ByteSink& byteSink) : OutgoingDataLink(byteSink) {
	// TODO Auto-generated constructor stub
}

OutgoingPinDuinoDataLink::~OutgoingPinDuinoDataLink() {
	// TODO Auto-generated destructor stub
}

void OutgoingPinDuinoDataLink::log(const char *fmt, ...)
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
