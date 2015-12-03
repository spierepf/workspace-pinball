/*
 * SwitchListenerImpl.h
 *
 *  Created on: Dec 3, 2015
 *      Author: peter
 */

#ifndef SWITCHLISTENERIMPL_H_
#define SWITCHLISTENERIMPL_H_

#include <SwitchListener.h>

class SwitchListenerImpl: public SwitchListener {
public:
	SwitchListenerImpl();
	virtual ~SwitchListenerImpl();

	virtual void switchChange(uint8_t, bool) const;
};

#endif /* SWITCHLISTENERIMPL_H_ */
