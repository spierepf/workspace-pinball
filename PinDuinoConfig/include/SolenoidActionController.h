/*
 * SolenoidActionController.h
 *
 *  Created on: Mar 10, 2015
 *      Author: peter
 */

#ifndef SOLENOIDACTIONCONTROLLER_H_
#define SOLENOIDACTIONCONTROLLER_H_

#include <gtkmm.h>

#include <IncomingDataLink.h>
#include <OutgoingDataLink.h>
#include <Stimulus.h>
#include <SolenoidAction.h>

class SolenoidActionController {
	OutgoingDataLink& datalink;
	Stimulus stimulus;
	SolenoidAction action;
	bool initialized;

	Gtk::CheckButton *enable;
	Gtk::SpinButton *solenoid;
	Gtk::SpinButton *attack;
	Gtk::SpinButton *sustain;
	Gtk::Button *reset;
	Gtk::Image *resetImage;
	Gtk::Button *apply;
	Gtk::Image *applyImage;

public:
	SolenoidActionController(OutgoingDataLink&, Stimulus, Gtk::Grid*, int, int);
	virtual ~SolenoidActionController();

	void update();

	void onResetClicked();
	void onApplyClicked();

	void set(SolenoidAction);
};

#endif /* SOLENOIDACTIONCONTROLLER_H_ */
