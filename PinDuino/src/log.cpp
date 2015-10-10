/*
 * log.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: peter
 */

#include <extern.h>

#include <stdio.h>
#include <limits.h>
#include <string.h>

#include <OpCode.h>

void LOG(const char *fmt, ...)
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

	outgoingFrameBuffer.put(OpCode::LOG); // log
	for(size_t i = 0; i < strlen(s); i++) {
		outgoingFrameBuffer.put(s[i]);
	}
	outgoingFrameBuffer.endFrame();
}



