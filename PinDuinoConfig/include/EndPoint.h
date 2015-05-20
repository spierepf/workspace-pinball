/*
 * EndPoint.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef ENDPOINT_H_
#define ENDPOINT_H_

#include <AbstractEndPoint.h>

#include <gtkmm.h>

#include <Tty.h>
#include <DataLink.h>

#include <string>
using namespace std;

#include <SolenoidActionController.h>

class EndPoint : public AbstractEndPoint {
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
	ByteSource* byteSource;
	ByteSink* byteSink;
	int id;

protected:
	void handleIncomingFrame();

public:
	EndPoint(Gtk::Notebook* notebook, string, ByteSource*, ByteSink*, DataLink*);
	virtual ~EndPoint();
};

#endif /* ENDPOINT_H_ */
