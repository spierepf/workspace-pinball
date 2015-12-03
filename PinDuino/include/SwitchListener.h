/*
 * SwitchListener.h
 *
 *  Created on: Dec 3, 2015
 *      Author: peter
 */

#ifndef SWITCHLISTENER_H_
#define SWITCHLISTENER_H_

#include <stdint.h>

class SwitchListener {
public:
	SwitchListener() {};
	virtual ~SwitchListener() {};

	virtual void switchChange(uint8_t, bool) const = 0;
};

#endif /* SWITCHLISTENER_H_ */
