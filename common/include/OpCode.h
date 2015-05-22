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
		SWITCH_ACTIVE = 0x10,
		SWITCH_INACTIVE,
		SR_INHIBIT = 0x20,
		SR_ENABLE,
		SR_CONFIG
	};
}

#endif /* INCLUDE_OPCODE_H_ */
