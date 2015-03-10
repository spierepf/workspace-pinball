/*
 * SolenoidActionController.h
 *
 *  Created on: Mar 10, 2015
 *      Author: peter
 */

#ifndef SOLENOIDACTIONCONTROLLER_H_
#define SOLENOIDACTIONCONTROLLER_H_

#include <gtkmm.h>

class SolenoidActionController {
public:
	SolenoidActionController(Gtk::Grid*, int, int);
	virtual ~SolenoidActionController();
};

#endif /* SOLENOIDACTIONCONTROLLER_H_ */
