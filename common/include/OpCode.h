/*
 * OpCode.h
 *
 *  Created on: Feb 25, 2015
 *      Author: peter
 */

#ifndef INCLUDE_OPCODE_H_
#define INCLUDE_OPCODE_H_

namespace OpCode {
	enum {
		MY_ID,
		PING,
		PONG,
		LOG,
		PIN_LOW = 0x10,
		PIN_HIGH,
		SR_INHIBIT = 0x20,
		SR_ENABLE
	};
}

#endif /* INCLUDE_OPCODE_H_ */
