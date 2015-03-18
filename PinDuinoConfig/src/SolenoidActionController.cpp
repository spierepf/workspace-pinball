/*
 * SolenoidActionController.cpp
 *
 *  Created on: Mar 10, 2015
 *      Author: peter
 */

#include <OpCode.h>

#include "SolenoidActionController.h"

#include <easylogging++.h>

SolenoidActionController::SolenoidActionController(DataLink& datalink, Stimulus stimulus, Gtk::Grid* child, int left, int top) : datalink(datalink), stimulus(stimulus), initialized(false) {
	enable = new Gtk::CheckButton();
	enable->signal_clicked().connect(sigc::mem_fun(this, &SolenoidActionController::update));
	child->attach(*enable, left, top, 1, 1);

	solenoid = new Gtk::SpinButton();
	solenoid->set_range(8, 13);
	solenoid->set_increments(1, 1);
	solenoid->signal_value_changed().connect(sigc::mem_fun(this, &SolenoidActionController::update));
	child->attach_next_to(*solenoid, *enable, Gtk::POS_RIGHT, 1, 1);

	attack = new Gtk::SpinButton();
	attack->set_range(0, 65000);
	attack->set_increments(100, 1000);
	attack->signal_value_changed().connect(sigc::mem_fun(this, &SolenoidActionController::update));
	child->attach_next_to(*attack, *solenoid, Gtk::POS_RIGHT, 1, 1);

	sustain = new Gtk::SpinButton();
	sustain->set_range(0, 255);
	sustain->set_increments(1, 10);
	sustain->signal_value_changed().connect(sigc::mem_fun(this, &SolenoidActionController::update));
	child->attach_next_to(*sustain, *attack, Gtk::POS_RIGHT, 1, 1);

	reset = new Gtk::Button();
	resetImage = new Gtk::Image("sync-16.png");
	reset->set_image(*resetImage);
	reset->signal_clicked().connect(sigc::mem_fun(this, &SolenoidActionController::onResetClicked));
	child->attach_next_to(*reset, *sustain, Gtk::POS_RIGHT, 1, 1);

	apply = new Gtk::Button();
	applyImage = new Gtk::Image("write-16.png");
	apply->set_image(*applyImage);
	apply->signal_clicked().connect(sigc::mem_fun(this, &SolenoidActionController::onApplyClicked));
	child->attach_next_to(*apply, *reset, Gtk::POS_RIGHT, 1, 1);

	update();
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

void SolenoidActionController::update() {
	bool sensitive = initialized;
	bool changed = false;

	enable->set_sensitive(sensitive);
	sensitive = sensitive && enable->get_active();
	changed = changed || enable->get_active() != action.enabled;

	solenoid->set_sensitive(sensitive);
	changed = changed || solenoid->get_value() != action.solenoidIndex + 8;

	attack->set_sensitive(sensitive);
	changed = changed || attack->get_value() != action.attack;

	sustain->set_sensitive(sensitive);
	changed = changed || sustain->get_value() != action.sustain;

	reset->set_sensitive(changed || !initialized);
	apply->set_sensitive(changed && initialized);
}

void SolenoidActionController::onResetClicked() {
	datalink.begin_outgoing_frame(OpCode::SR_CONFIG);
	stimulus.write_to(datalink);
	datalink.end_outgoing_frame();
}

void SolenoidActionController::onApplyClicked() {
	action.enabled = enable->get_active();
	action.solenoidIndex = solenoid->get_value() - 8;
	action.attack = attack->get_value();
	action.sustain = sustain->get_value();

	datalink.begin_outgoing_frame(OpCode::SR_CONFIG);
	stimulus.write_to(datalink);
	action.write_to(datalink);
	datalink.end_outgoing_frame();
}

void SolenoidActionController::set(SolenoidAction action) {
	initialized = true;
	this->action = action;

	enable->set_active(action.enabled);
	solenoid->set_value(action.solenoidIndex + 8);
	attack->set_value(action.attack);
	sustain->set_value(action.sustain);
	update();
}
