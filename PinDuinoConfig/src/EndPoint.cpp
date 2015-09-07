/*
 * EndPoint.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <OpCode.h>

#include <easylogging++.h>
#include <EndPoint.h>
#include <SolenoidAction.h>

char* labels[] = {
		"A0",
		"A1",
		"A2",
		"A3",
		"A4",
		"A5",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
};

EndPoint::EndPoint(Gtk::Notebook* notebook, string device, Hardware* hardware, IncomingDataLink* incomingDatalink, OutgoingDataLink* outgoingDatalink, FrameBuffer<64, 4>* incomingFrames, FrameBuffer<64, 4>* outgoingFrames) : AbstractEndPoint(*incomingDatalink, *outgoingDatalink, *incomingFrames, *outgoingFrames), notebook(notebook), device(device), hardware(hardware), id(255) {
	grid = new Gtk::Grid();
	label = new Gtk::Label(device);

	inputLabel = new Gtk::Label("Input");
	grid->attach(*inputLabel, 0, 0, 1, 1);
	onLowLabel = new Gtk::Label("On Active");
	grid->attach_next_to(*onLowLabel, *inputLabel, Gtk::POS_RIGHT, 6, 1);
	onHighLabel = new Gtk::Label("On Inactive");
	grid->attach_next_to(*onHighLabel, *onLowLabel, Gtk::POS_RIGHT, 6, 1);

	onLowEnableLabel = new Gtk::Label("Enable");
	grid->attach(*onLowEnableLabel, 1, 1, 1, 1);
	onLowSolenoidLabel = new Gtk::Label("Solenoid");
	grid->attach_next_to(*onLowSolenoidLabel, *onLowEnableLabel, Gtk::POS_RIGHT, 1, 1);
	onLowAttackLabel = new Gtk::Label("Attack");
	grid->attach_next_to(*onLowAttackLabel, *onLowSolenoidLabel, Gtk::POS_RIGHT, 1, 1);
	onLowSustainLabel = new Gtk::Label("Sustain");
	grid->attach_next_to(*onLowSustainLabel, *onLowAttackLabel, Gtk::POS_RIGHT, 1, 1);

	onHighEnableLabel = new Gtk::Label("Enable");
	grid->attach(*onHighEnableLabel, 7, 1, 1, 1);
	onHighSolenoidLabel = new Gtk::Label("Solenoid");
	grid->attach_next_to(*onHighSolenoidLabel, *onHighEnableLabel, Gtk::POS_RIGHT, 1, 1);
	onHighAttackLabel = new Gtk::Label("Attack");
	grid->attach_next_to(*onHighAttackLabel, *onHighSolenoidLabel, Gtk::POS_RIGHT, 1, 1);
	onHighSustainLabel = new Gtk::Label("Sustain");
	grid->attach_next_to(*onHighSustainLabel, *onHighAttackLabel, Gtk::POS_RIGHT, 1, 1);

	for(int i = 0; i < 12; i++) {
		rowLabels[i] = new Gtk::Label(labels[i]);
		grid->attach(*rowLabels[i], 0, i+2, 1, 1);
		solenoidActionControllers[i][0] = new SolenoidActionController(*outgoingDatalink, Stimulus(i, false), grid, 1, i+2);
		solenoidActionControllers[i][1] = new SolenoidActionController(*outgoingDatalink, Stimulus(i, true), grid, 7, i+2);
	}

	notebook -> append_page(*grid, *label);
	notebook -> show_all();
}

EndPoint::~EndPoint() {
	delete label;

	delete inputLabel;
	delete onLowLabel;
	delete onHighLabel;

	delete onLowEnableLabel;
	delete onLowSolenoidLabel;
	delete onLowAttackLabel;
	delete onLowSustainLabel;

	delete onHighEnableLabel;
	delete onHighSolenoidLabel;
	delete onHighAttackLabel;
	delete onHighSustainLabel;

	for(int i = 0; i < 12; i++) {
		delete rowLabels[i];
		delete solenoidActionControllers[i][0];
		delete solenoidActionControllers[i][1];
	}

	delete grid;
}

void EndPoint::handleIncomingFrame() {
	if(incomingFrames[0][0] == OpCode::PING) { // ping
		outgoingDatalink.begin_outgoing_frame(OpCode::PONG); // pong
		outgoingDatalink.append_payload(incomingFrames[0][1]); // return the payload of the ping
		outgoingDatalink.end_outgoing_frame();
	} else if(incomingFrames[0][0] == OpCode::LOG) { // log
		char buf[32];
		for(int i = 1; i < incomingFrames[0].getLength(); i++) {
			buf[i-1] = (char)incomingFrames[0][i];
		}
		buf[incomingFrames[0].getLength() - 1] = '\0';
		LOG(INFO) << "From " << id << ": " << buf;
	} else if(incomingFrames[0][0] == OpCode::MY_ID) {
		id = incomingFrames[0][1];
		LOG(INFO) << "Device " << device << " registered id: " << (int)id;
		outgoingDatalink.begin_outgoing_frame(OpCode::SR_ENABLE);
		outgoingDatalink.end_outgoing_frame();
		ostringstream convert;
		convert << id;
		label->set_text(convert.str());
	} else if(incomingFrames[0][0] == OpCode::SWITCH_ACTIVE) {
		LOG(INFO) << "Switch Active: " << id << ":" << (int)incomingFrames[0][1];
	} else if(incomingFrames[0][0] == OpCode::SWITCH_INACTIVE) {
		LOG(INFO) << "Switch Inactive: " << id << ":"  << (int)incomingFrames[0][1];
	} else if(incomingFrames[0][0] == OpCode::SR_CONFIG) {
		uint8_t i = 1;
		if(incomingFrames[0].getLength() >= i+sizeof(Stimulus)) {
			Stimulus stimulus;
			stimulus.read_from(incomingFrames[0], i);
			if(incomingFrames[0].getLength() >= i+sizeof(SolenoidAction)) {
				SolenoidAction action;
				action.read_from(incomingFrames[0], i);
				solenoidActionControllers[stimulus.pin][stimulus.newState]->set(action);
			}
		}
	}
}
