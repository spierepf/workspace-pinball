/*
 * EndPoint.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef ENDPOINT_H_
#define ENDPOINT_H_

#include <gtkmm.h>

#include <pt.h>

#include <Tty.h>
#include <DataLink.h>

#include <string>
using namespace std;

#include <SolenoidActionController.h>

class EndPoint {
	Gtk::Notebook* notebook;

	Gtk::Grid *grid;
	Gtk::Label *label;

	Gtk::Label *inputLabel;
	Gtk::Label *onLowLabel;
	Gtk::Label *onHighLabel;

	Gtk::Label *onLowEnableLabel;
	Gtk::Label *onLowSolenoidLabel;
	Gtk::Label *onLowAttackLabel;
	Gtk::Label *onLowSustainLabel;

	Gtk::Label *onHighEnableLabel;
	Gtk::Label *onHighSolenoidLabel;
	Gtk::Label *onHighAttackLabel;
	Gtk::Label *onHighSustainLabel;

	Gtk::Label *rowLabels[12];
	SolenoidActionController* solenoidActionControllers[12][2];

	string device;
	Hardware* hardware;
	DataLink datalink;
	int id;
	struct pt pt;

	PT_THREAD(run());

public:
	EndPoint(Gtk::Notebook* notebook, string, Hardware*);
	virtual ~EndPoint();

	void schedule();
};

#endif /* ENDPOINT_H_ */
