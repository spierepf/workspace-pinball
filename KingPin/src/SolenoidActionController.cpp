/*
 * SolenoidActionController.cpp
 *
 *  Created on: Mar 10, 2015
 *      Author: peter
 */

#include "SolenoidActionController.h"

SolenoidActionController::SolenoidActionController(Gtk::Grid* child, int left, int top) {
	enable = new Gtk::CheckButton();
	child->attach(*enable, left, top, 1, 1);

	solenoid = new Gtk::SpinButton();
	solenoid->set_range(8, 13);
	solenoid->set_increments(1, 1);
	child->attach_next_to(*solenoid, *enable, Gtk::POS_RIGHT, 1, 1);

	attack = new Gtk::SpinButton();
	attack->set_range(0, 65000);
	attack->set_increments(100, 1000);
	child->attach_next_to(*attack, *solenoid, Gtk::POS_RIGHT, 1, 1);

	sustain = new Gtk::SpinButton();
	sustain->set_range(0, 255);
	sustain->set_increments(1, 10);
	child->attach_next_to(*sustain, *attack, Gtk::POS_RIGHT, 1, 1);

	reset = new Gtk::Button();
	resetImage = new Gtk::Image("sync-16.png");
	reset->set_image(*resetImage);
	child->attach_next_to(*reset, *sustain, Gtk::POS_RIGHT, 1, 1);

	apply = new Gtk::Button();
	applyImage = new Gtk::Image("write-16.png");
	apply->set_image(*applyImage);
	child->attach_next_to(*apply, *reset, Gtk::POS_RIGHT, 1, 1);
}

SolenoidActionController::~SolenoidActionController() {
	delete enable;
	delete solenoid;
	delete attack;
	delete sustain;
	delete reset;
	delete resetImage;
	delete apply;
	delete applyImage;
}

