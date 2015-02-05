/*
 * PinDuinoDataLink.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef PINDUINODATALINK_H_
#define PINDUINODATALINK_H_

#include <DataLink.h>

class PinDuinoDataLink: public DataLink {
public:
	PinDuinoDataLink(Hardware&);
	virtual ~PinDuinoDataLink();

	void log(const char*, ...);
};

#endif /* PINDUINODATALINK_H_ */
