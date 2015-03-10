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
	Gtk::CheckButton *enable;
	Gtk::SpinButton *solenoid;
	Gtk::SpinButton *attack;
	Gtk::SpinButton *sustain;
	Gtk::Button *reset;
	Gtk::Image *resetImage;
	Gtk::Button *apply;
	Gtk::Image *applyImage;

public:
	SolenoidActionController(Gtk::Grid*, int, int);
	virtual ~SolenoidActionController();
};

#endif /* SOLENOIDACTIONCONTROLLER_H_ */
