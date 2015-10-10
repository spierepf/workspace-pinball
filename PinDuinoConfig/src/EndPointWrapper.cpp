/*
 * EndPointWrapper.cpp
 *
 *  Created on: Oct 9, 2015
 *      Author: peter
 */

#include "EndPointWrapper.h"

#include <easylogging++.h>

#include <OpCode.h>

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

EndPointWrapper::EndPointWrapper(Gtk::Notebook* notebook, char* device) :
	device(device),
	serialPort(device),
	serialSource(serialPort),
	serialSink(serialPort),
	escapingSource(serialSource),
	escapingSink(serialSink),
	frameReceiver(escapingSource),
	frameTransmitter(escapingSink),
	endPoint(escapingSource, frameReceiver, *this, outgoingFrameBuffer, frameTransmitter, escapingSink)
{
	serialPort.Open(SerialPort::BAUD_115200);
	frameReceiver.setFrameHandler(&endPoint);

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
		solenoidActionControllers[i][0] = new SolenoidActionController(outgoingFrameBuffer, Stimulus(i, false), grid, 1, i+2);
		solenoidActionControllers[i][1] = new SolenoidActionController(outgoingFrameBuffer, Stimulus(i, true), grid, 7, i+2);
	}

	notebook -> append_page(*grid, *label);
	notebook -> show_all();
}

EndPointWrapper::~EndPointWrapper() {
	for(int i = 0; i < 12; i++) {
		delete solenoidActionControllers[i][0];
		delete solenoidActionControllers[i][1];
	}
}

void EndPointWrapper::handle(const uint8_t, const uint8_t* payload, const uint8_t payloadSize) {
	if(payload[0] == OpCode::PING) { // ping
		outgoingFrameBuffer.put(OpCode::PONG); // pong
		outgoingFrameBuffer.put(payload[1]); // return the payload of the ping
		outgoingFrameBuffer.endFrame();
	} else if(payload[0] == OpCode::LOG) { // log
		char buf[32];
		for(int i = 1; i < payloadSize; i++) {
			buf[i-1] = (char)payload[i];
		}
		buf[payloadSize - 1] = '\0';
		LOG(INFO) << "From " << id << ": " << buf;
	} else if(payload[0] == OpCode::MY_ID) {
		id = payload[1];
		LOG(INFO) << "Device " << device << " registered id: " << (int)id;
		outgoingFrameBuffer.put(OpCode::SR_ENABLE);
		outgoingFrameBuffer.endFrame();
			std::ostringstream convert;
			convert << id;
			label->set_text(convert.str());
	} else if(payload[0] == OpCode::SWITCH_ACTIVE) {
		LOG(INFO) << "Switch Active: " << id << ":" << (int)payload[1];
	} else if(payload[0] == OpCode::SWITCH_INACTIVE) {
		LOG(INFO) << "Switch Inactive: " << id << ":"  << (int)payload[1];
	} else if(payload[0] == OpCode::SR_CONFIG) {
			uint8_t i = 1;
			if(payloadSize >= i+sizeof(Stimulus)) {
				Stimulus stimulus;
				stimulus.read_from(payload, i);
				if(payloadSize >= i+sizeof(SolenoidAction)) {
					SolenoidAction action;
					action.read_from(payload, i);
					solenoidActionControllers[stimulus.pin][stimulus.newState]->set(action);
				}
			}
	}
}

PT_THREAD(EndPointWrapper::run()) {
	PT_BEGIN(&pt);
	endPoint.schedule();
	PT_END(&pt);
}
